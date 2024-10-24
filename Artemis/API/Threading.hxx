#ifndef __ARTEMIS_API_THREADING_HXX__
#define __ARTEMIS_API_THREADING_HXX__

#include <utility>
#include <queue>
#include <thread>
#include <chrono>

#include "Definitions.hxx"

#include "Exception.hxx"

namespace Artemis::API {
    template<typename _Ty>
    concept is_copyable = requires (_Ty _A, _Ty _B) {
        _A = _B;
        _Ty(_B);
    };

    template<typename _Ty>
    concept is_movable = requires (_Ty _A, _Ty _B) {
        _A = std::move(_B);
        _Ty(std::move(_B));
    };

    template<typename _Ty, typename... _Args>
    concept is_constructible_with = requires (_Args... _A) { _Ty(_A...); };

    class resource_awaiter {
        std::thread::id _ThreadId;
        bool _IsAwaiting;

        resource_awaiter(std::thread::id _ThreadId) noexcept;

    public:
        resource_awaiter() = delete;
        resource_awaiter(const resource_awaiter&) = delete;
        resource_awaiter(resource_awaiter&&) = delete;

        ARTEMIS_API std::thread::id awaiting_thread() const noexcept;

        ARTEMIS_API bool is_awaiting() const noexcept;
        ARTEMIS_API void cancel_await() noexcept;

        friend ARTEMIS_API resource_awaiter* make_awaiter() noexcept;
    };

    ARTEMIS_API resource_awaiter* make_awaiter() noexcept;

    template<class _Ty, bool _AlwaysAllowImmutableAccess = true>
    class thread_safe {
        _Ty _Data;
        std::thread::id _LockHolder;
        std::queue<resource_awaiter*> _ResourceQueue;

    public:
        inline thread_safe(const thread_safe<_Ty>&) = delete;
        inline thread_safe(thread_safe<_Ty>&&) = delete;

        template<typename... _TyArgs, typename = typename std::enable_if<is_constructible_with<_Ty, _TyArgs...>>::type>
        inline thread_safe(_TyArgs... _Args) : _Data(_Ty(_Args...)), _LockHolder(), _ResourceQueue() {}

        template<typename = typename std::enable_if<is_copyable<_Ty>>::type>
        inline thread_safe(const _Ty& _DataFrom) noexcept : _Data(_DataFrom), _LockHolder(), _ResourceQueue() {}

        template<typename = typename std::enable_if<is_movable<_Ty>>::type>
        inline thread_safe(_Ty&& _DataFrom) noexcept : _Data(std::move(_DataFrom)), _LockHolder(), _ResourceQueue() {}

        inline ~thread_safe() noexcept {
            while (!this->_ResourceQueue.empty()) {
                delete this->_ResourceQueue.front();
                this->_ResourceQueue.pop();
            }
        }

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

            resource_awaiter* awaiter = make_awaiter();
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

            resource_awaiter* awaiter = this->_ResourceQueue.front();

            if (awaiter->is_awaiting())
                this->_LockHolder = awaiter->awaiting_thread();

            this->_ResourceQueue.pop();
            delete awaiter;
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