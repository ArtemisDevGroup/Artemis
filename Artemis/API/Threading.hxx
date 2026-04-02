#ifndef __ARTEMIS_API_THREADING_HXX__
#define __ARTEMIS_API_THREADING_HXX__

#include <queue>        // std::queue
#include <thread>       // std::thread, std::this_thread::sleep_for
#include <chrono>       // std::chrono::duration, std::chrono::steady_clock, std::chrono::time_point, std::chrono::nanoseconds
#include <concepts>     // std::constructible_from, std::copyable, std::movable
#include <utility>      // std::unique_ptr

#include "Definitions.hxx"

#include "Exception.hxx"

namespace Artemis::API {
    /**
     * @brief A utility class for awaiting a resource.
     */
    class resource_awaiter {
        std::thread::id _ThreadId;
        bool _IsAwaiting;

        /**
         * @brief Constructs a resource awaiter for the provided thread id.
         * @param[in] _ThreadId The id of the thread waiting for the resource.
         */
        resource_awaiter(std::thread::id _ThreadId) noexcept;

    public:
        resource_awaiter() = delete;
        resource_awaiter(const resource_awaiter&)   = delete;
        resource_awaiter(resource_awaiter&&)        = delete;

        /**
         * @brief Gets the id of the thread awaiting the resource.
         * @return The id of the thread awaiting the resource.
         */
        ARTEMIS_API std::thread::id awaiting_thread() const noexcept;

        /**
         * @brief Determines whether the thread is still awaiting the resource, or if it has cancelled its await.
         * @return `true` if the thread is awaiting the resource, otherwise `false`.
         */
        ARTEMIS_API bool is_awaiting() const noexcept;

        /**
         * @brief Cancels the await of the resource for the contained thread.
         */
        ARTEMIS_API void cancel_await() noexcept;

        friend std::shared_ptr<resource_awaiter> make_awaiter() noexcept;
    };

    /**
     * @brief Makes a new resource awaiter.
     * @return A pointer to a new resource awaiter.
     */
    ARTEMIS_API std::shared_ptr<resource_awaiter> make_awaiter() noexcept;

    /**
     * @brief A container type that protects the contained value from concurrent reads and writes.
     * @tparam _Ty The contained type.
     * @tparam _AlwaysAllowImmutableAccess Whether to allow const access to the contained data without awaiting access. Defaults to `true`.
     */
    template<class _Ty, bool _AlwaysAllowImmutableAccess = true>
    class thread_safe {
        _Ty _Data;
        std::thread::id _LockHolder;
        std::queue<std::shared_ptr<resource_awaiter>> _ResourceQueue;

    public:
        inline thread_safe(const thread_safe<_Ty>&) = delete;
        inline thread_safe(thread_safe<_Ty>&&)      = delete;

        /**
         * @brief Constructs a protected object of type `_Ty` in-place.
         * @tparam _TyArgs The constructor argument types.
         * @param[in] _Args The constructor arguments.
         */
        template<typename... _TyArgs>
            requires std::constructible_from<_Ty, _TyArgs...>
        inline thread_safe(_TyArgs&&... _Args) : _Data(_Ty(std::forward<_TyArgs>(_Args)...)), _LockHolder(), _ResourceQueue() {}

        /**
         * @brief Constructs a protected object of type `_Ty` and copy-initializes it from the referenced object.
         * @param[in] _DataFrom A reference to the object to copy.
         */
        inline thread_safe(const _Ty& _DataFrom) noexcept requires std::copyable<_Ty> : _Data(_DataFrom), _LockHolder(), _ResourceQueue() {}

        /**
         * @brief Constructs a protected object of type `_Ty` and move-initializes it from the referenced object.
         * @param[in] _DataFrom A reference to the object to move.
         */
        inline thread_safe(_Ty&& _DataFrom) noexcept requires std::movable<_Ty> : _Data(std::move(_DataFrom)), _LockHolder(), _ResourceQueue() {}

        template<
            typename _Rep1, typename _Period1,
            typename _Rep2, typename _Period2
        >
        inline void request(
            std::chrono::duration<_Rep1, _Period1> _Timeout,
            std::chrono::duration<_Rep2, _Period2> _RefreshInterval
        ) {
            std::thread::id currentThread = std::this_thread::get_id();

            if (this->_LockHolder == std::thread::id()) {
                this->_LockHolder = currentThread;
                return;
            }

            std::shared_ptr<resource_awaiter> awaiter = make_awaiter();
            this->_ResourceQueue.push(awaiter);

            if (_Timeout == std::chrono::duration<_Rep1, _Period1>::zero()) {
                while (this->_LockHolder != currentThread)
                    std::this_thread::sleep_for(_RefreshInterval);
                return;
            }

            std::chrono::time_point start = std::chrono::steady_clock::now();

            while (this->_LockHolder != currentThread) {
                std::chrono::time_point current = std::chrono::steady_clock::now();
                std::chrono::nanoseconds delta = current - start;

                if (delta > _Timeout) {
                    awaiter->cancel_await();
                    throw lock_exception("Lock request timed out.");
                }

                std::this_thread::sleep_for(_RefreshInterval);
            }
        }

        template<typename _Rep, typename _Period>
        inline void request(std::chrono::duration<_Rep, _Period> _Timeout) { return this->request(_Timeout, std::chrono::milliseconds(10)); }

        inline void request() { return this->request(std::chrono::milliseconds::zero(), std::chrono::milliseconds(10)); }

        inline void release() {
            if (this->_LockHolder != std::this_thread::get_id())
                throw lock_exception("The current thread does not have access to this resource at this time.");

            if (this->_ResourceQueue.empty()) {
                this->_LockHolder = std::thread::id();
                return;
            }

            std::shared_ptr<resource_awaiter> awaiter = this->_ResourceQueue.front();

            if (awaiter->is_awaiting())
                this->_LockHolder = awaiter->awaiting_thread();
            this->_ResourceQueue.pop();
        }

        inline const thread_safe<_Ty, _AlwaysAllowImmutableAccess>& constant() noexcept { return *this; }

        inline _Ty* data_ptr() noexcept {
            if (this->_LockHolder != std::this_thread::get_id())
                throw lock_access_exception();
            return &this->_Data;
        }

        inline const _Ty* data_ptr() const noexcept {
            if constexpr (!_AlwaysAllowImmutableAccess)
                if (this->_LockHolder != std::this_thread::get_id())
                    throw lock_access_exception();
            return &this->_Data;
        }

        inline thread_safe<_Ty>& operator=(const thread_safe<_Ty>&) = delete;

        inline thread_safe<_Ty>& operator=(thread_safe<_Ty>&& _From) noexcept {
            this->_Data = std::move(_From._Data);
            this->_LockHolder = std::thread::id();
            this->_ResourceQueue = std::queue<resource_awaiter*>();
        }

        inline thread_safe<_Ty>& operator=(const _Ty&) = delete;
        inline thread_safe<_Ty>& operator=(_Ty&&) = delete;

        inline _Ty& operator*() {
            if (this->_LockHolder != std::this_thread::get_id())
                throw lock_access_exception();
            return this->_Data;
        }

        inline const _Ty& operator*() const {
            if constexpr (!_AlwaysAllowImmutableAccess)
                if (this->_LockHolder != std::this_thread::get_id())
                    throw lock_access_exception();
            return this->_Data;
        }

        inline std::remove_pointer_t<_Ty>* operator->() {
            if (this->_LockHolder != std::this_thread::get_id())
                throw lock_access_exception();

            if constexpr (std::is_pointer_v<_Ty>)
                return this->_Data;
            else
                return &this->_Data;
        }

        inline const std::remove_pointer_t<_Ty>* operator->() const {
            if constexpr (!_AlwaysAllowImmutableAccess)
                if (this->_LockHolder != std::this_thread::get_id())
                    throw lock_access_exception();
            
            if constexpr (std::is_pointer_v<_Ty>)
                return this->_Data;
            else
                return &this->_Data;
        }
    };
}

#endif // !__ARTEMIS_API_THREADING_HXX__