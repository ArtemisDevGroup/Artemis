#ifndef __ARTEMIS_API_MEMORY_HXX__
#define __ARTEMIS_API_MEMORY_HXX__

#include <type_traits>	// std::remove_reference_t
#include <concepts>		// std::derived_from, std::integral
#include <ranges>		// std::ranges::range
#include <vector>		// std::vector
#include <array>		// std::array
#include <utility>		// std::move, std::forward

#include <cstdint>		// std::uintptr_t, std::size_t

#include <Windows.h>	// VirtualQuery, MEMORY_BASIC_INFORMATION
#include <sal.h>

#include "Definitions.hxx"

#include "Exception.hxx"

namespace Artemis::API {
	/**
	 * @brief Describes a type that is trivially copyable; e.g. whose raw data can be directly copied to make a copy of the object.
	 */
	template<typename _Ty>
	concept trivially_copyable = std::is_trivially_copyable_v<_Ty>;

	/**
	 * @brief Describes a container type that can be appended to by calling push_back.
	 */
	template<typename _Ty, typename _VTy>
	concept appendable = requires (_Ty _X, const _VTy& _Y) {
		_X.push_back(_Y);
	};

	/**
	 * @brief Describes a container type that can be appended to by calling insert.
	 */
	template<typename _Ty, typename _VTy>
	concept insertable = requires (_Ty _X, const _VTy& _Y) {
		_X.insert(std::end(_X), _Y);
	};

	/**
	 * @brief Represents a type of memory operation.
	 */
	enum class memory_operation {
		/**
		 * @brief Represents the action of reading from memory.
		 */
		read,

		/**
		 * @brief Represents the action of writing to memory.
		 */
		write,

		/**
		 * @brief Represents the action of executing memory.
		 */
		execute
	};

	/**
	 * @brief An exception type thrown when a memory access violation has happened.
	 */
	class access_violation_exception : public system_exception {
		address_t _Address;
		size_t _Size;
		memory_operation _Operation;

		MEMORY_BASIC_INFORMATION _MBI;

		ARTEMIS_API static std::string format_message(address_t _Address, size_t _Size, memory_operation _Operation);

	public:
		/**
		 * @brief Constructs an access violation exception with a location, size and type of operation.
		 * @param[in] _Address The address where the memory operation took place.
		 * @param[in] _Size The number of bytes past the address that were accessed.
		 * @param[in] _Operation The memory operation that took place.
		 */
		ARTEMIS_API access_violation_exception(address_t _Address, size_t _Size, memory_operation _Operation);

		/**
		 * @brief Constructs an access violation exception with a location, size, type of operation, and an underlying exception.
		 * @tparam _Ty The inner exception type. Must be derived from `Artemis::API::exception`.
		 * @param[in] _Address The address where the memory operation took place.
		 * @param[in] _Size The number of bytes past the address that were accessed.
		 * @param[in] _Operation The memory operation that took place.
		 * @param[in] _InnerException A reference to the underlying exception.
		 */
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline access_violation_exception(address_t _Address, size_t _Size, memory_operation _Operation, _Ty&& _InnerException) : system_exception(format_message(_Address, _Size, _Operation), std::forward<_Ty>(_InnerException)), _Address(_Address), _Size(_Size), _Operation(_Operation) {
			VirtualQuery(_Address, &this->_MBI, sizeof(this->_MBI));
		}

		/**
		 * @brief Gets the address where the exception occured.
		 * @return The instance address.
		 */
		ARTEMIS_API address_t address() const noexcept;

		/**
		 * @brief Gets the size of the operation in bytes.
		 * @return The operation size.
		 */
		ARTEMIS_API size_t size() const noexcept;

		/**
		 * @brief Gets the type of memory operation that threw an exception.
		 * @return The memory operation.
		 */
		ARTEMIS_API memory_operation operation() const noexcept;

		/**
		 * @brief Gets information about the memory region at the time of the exception.
		 * @return The memory region information.
		 */
		ARTEMIS_API const MEMORY_BASIC_INFORMATION* mbi() const noexcept;
	};

	/**
	 * @brief Represents a memory address.
	 */
	class address_t {
	public:
		/**
		 * @brief The instance value type.
		 */
		using value_type = std::uintptr_t;

		/**
		 * @brief The size type.
		 */
		using size_type = std::size_t;

	private:
		static constexpr std::size_t _StringMaxLength = 256;

		value_type _Value;

	public:
		/**
		 * @brief Constructs a null instance of address_t.
		 */
		constexpr address_t() : _Value(0) {}

		/**
		 * @brief Constructs a null instance of address_t.
		 */
		constexpr address_t(_Reserved_ std::nullptr_t) : _Value(0) {}

		/**
		 * @brief Constructs an instance of address_t from an integral value.
		 * @param[in] _Value The instance value.
		 */
		constexpr address_t(value_type _Value) : _Value(_Value) {}

		/**
		 * @brief Constructs an instance of address_t from a pointer of any type.
		 * @tparam _Ty The pointer type.
		 * @param[in] _Value The instance value.
		 */
		template<typename _Ty>
		inline address_t(_In_ _Ty* _Value) : _Value(reinterpret_cast<value_type>(_Value)) {}

		/**
		 * @brief Gets the current instance value.
		 * @return The instance value.
		 */
		[[nodiscard]] constexpr value_type value() const noexcept { return this->_Value; }

		/**
		 * @brief Gets the current instance value as a pointer of type _Ty.
		 * @tparam _Ty The pointer type.
		 * @return A pointer of type _Ty.
		 */
		template<typename _Ty>
		[[nodiscard]] _Ret_ inline _Ty* ptr() const noexcept { return reinterpret_cast<_Ty*>(this->_Value); }

		/**
		 * @brief Gets a pointer to the current value buffer.
		 * @return A pointer to the current value.
		 */
		[[nodiscard]] _Ret_ constexpr value_type* buffer() noexcept { return &this->_Value; }

		/**
		 * @brief Implicit conversion from instance value to value_type.
		 */
		[[nodiscard]] constexpr operator value_type () const noexcept { return this->_Value; }

		/**
		 * @brief Implicit conversion from instance value to pointer of type _Ty.
		 * @tparam _Ty The pointer type.
		 */
		template<typename _Ty>
		[[nodiscard]] _Ret_ inline operator _Ty* () const noexcept { return reinterpret_cast<_Ty*>(this->_Value); }

		/**
		 * @brief Implicit conversion from instance value to bool. Returns true if the instance value is not null, otherwise false.
		 */
		[[nodiscard]] constexpr operator bool() const noexcept { return !!(this->_Value); }

		/**
		 * @brief Overload of the = operator.
		 * @param[in] _Value The value to set.
		 * @return A reference to the current instance.
		 */
		constexpr address_t& operator=(value_type _Value) noexcept { this->_Value = _Value; }

		/**
		 * @brief Overload of the + operator.
		 * @tparam _Ty Integral type.
		 * @param[in] _Right The value to add.
		 * @return A new object as the result of the operation.
		 */
		template<std::integral _Ty>
		[[nodiscard]] constexpr address_t operator+(_Ty _Right) const noexcept { return address_t(this->_Value + _Right); }

		/**
		 * @brief Overload of the += operator.
		 * @tparam _Ty Integral type.
		 * @param[in] _Right The value to add to the current instance.
		 * @return A reference to the current instance.
		 */
		template<std::integral _Ty>
		constexpr address_t& operator+=(_Ty _Right) noexcept {
			this->_Value += _Right;
			return *this;
		}

		/**
		 * @brief Overload of the - operator.
		 * @tparam _Ty Integral type.
		 * @param[in] _Right The value to subtract.
		 * @return A new object as the result of the operation.
		 */
		template<std::integral _Ty>
		[[nodiscard]] constexpr address_t operator-(_Ty _Right) const noexcept { return address_t(this->_Value - _Right); }

		/**
		 * @brief Overload of the -= operator.
		 * @tparam _Ty Integral type.
		 * @param[in] _Right The value to subtract from the current instance.<
		 * @return A reference to the current instance.
		 */
		template<std::integral _Ty>
		constexpr address_t& operator-=(_Ty _Right) noexcept {
			this->_Value -= _Right;
			return *this;
		}

		/**
		 * @brief Reads a single value from the current address.
		 * @tparam _Ty The type to read. Must satisfy `trivially_copyable`.
		 * @param[out] _Return A reference to a variable to receive the read value.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<trivially_copyable _Ty>
		inline void read_one(_Ty& _Return) const {
			invalid_state_exception::throw_on_condition_not_met(ISE_CONDITION(this->_Value != 0));

			__try {
				_Return = *this->ptr<_Ty>();
			}
			__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
				throw access_violation_exception(*this, sizeof(_Ty), memory_operation::read);
			}
		}

		/**
		 * @brief Reads a single value from the current address.
		 * @tparam _Ty The type to read. Must satisfy `trivially_copyable`.
		 * @return The read value.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<trivially_copyable _Ty>
		[[nodiscard]] inline _Ty read_one() const {
			_Ty ret;
			read_one(ret);
			return ret;
		}

		/**
		 * @brief Reads multiple values from the current address contiguously.
		 * @tparam _Ty The type to read. Must satisfy `trivially_copyable`.
		 * @param[out] _Return A pointer to a buffer to receive the read values.
		 * @param[in] _Count The number of values to read.
		 * @throws invalid_state_exception
		 * @throws argument_exception
		 * @throws access_violation_exception
		 */
		template<trivially_copyable _Ty>
		inline void read_many(_Out_writes_(_Count) _Ty* _Return, size_type _Count) const {
			invalid_state_exception::throw_on_condition_not_met(ISE_CONDITION(this->_Value != 0));
			argument_exception::throw_if_null(AE_ARGUMENT(_Return));

			__try {
				for (size_type i = 0; i < _Count; i++)
					_Return[i] = this->ptr<_Ty>()[i];
			}
			__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
				throw access_violation_exception(*this, sizeof(_Ty) * _Count, memory_operation::read);
			}
		}

		/**
		 * @brief Reads multiple values from the current address contiguously.
		 * @tparam _Ty The type to read. Must satisfy `trivially_copyable`.
		 * @tparam _Count The number of values to read.
		 * @param[out] _Return A reference to a buffer to receive the read values.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<trivially_copyable _Ty, size_type _Count>
		inline void read_many(_Out_writes_(_Count) _Ty(&_Return)[_Count]) const {
			read_many(_Return, _Count);
		}

		/**
		 * @brief Reads multiple values from the current address contiguously.
		 * @tparam _Ty The type to read. Must satisfy `trivially_copyable`.
		 * @tparam _Count The number of values to read.
		 * @param[out] _Return A reference to an array to receive the read values.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<trivially_copyable _Ty, size_type _Count>
		inline void read_many(std::array<_Ty, _Count>& _Return) const {
			read_many(_Return.data(), _Count);
		}

		/**
		 * @brief Reads multiple values from the current address contiguously.
		 * @tparam _Ty The type to read. Must satisfy `trivially_copyable`.
		 * @tparam _Count The number of values to read.
		 * @return An array containing the read values.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<trivially_copyable _Ty, size_type _Count>
		[[nodiscard]] inline std::array<_Ty, _Count> read_many() const {
			std::array<_Ty, _Count> ret;
			read_many(ret);
			return ret;
		}

		/**
		 * @brief Reads multiple values from the current address contiguously.
		 * @tparam _Ty The containing type. Must satisfy `appendable`.
		 * @tparam _VTy The type to read. Must satisfy `trivially_copyable`. Defaults to `_Ty::value_type`.
		 * @param[out] _Return A reference to a container to receive the read values.
		 * @param[in] _Count The number of values to read.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
			requires appendable<_Ty, _VTy>
		inline void read_many(_Ty& _Return, size_type _Count) const {
			invalid_state_exception::throw_on_condition_not_met(ISE_CONDITION(this->_Value != 0));

			__try {
				for (size_type i = 0; i < _Count; i++)
					_Return.push_back(this->ptr<_VTy>()[i]);
			}
			__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
				throw access_violation_exception(*this, sizeof(_VTy) * _Count, memory_operation::read);
			}
		}

		/**
		 * @brief Reads multiple values from the current address contiguously.
		 * @tparam _Ty The type to read. Must satisfy `trivially_copyable`.
		 * @param[in] _Count The number of values to read.
		 * @return A vector containing the read values.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<trivially_copyable _Ty>
		[[nodiscard]] inline std::vector<_Ty> read_many(size_type _Count) const {
			std::vector<_Ty> ret;
			read_many(ret, _Count);
			return ret;
		}

		/**
		 * @brief Reads multiple values from the current address contiguously.
		 * @tparam _Ty The containing type. Must satisfy `appendable`.
		 * @tparam _VTy The type to read. Must satisfy `trivially_copyable`. Defaults to `_Ty::value_type`.
		 * @param[in] _Count The number of values to read.
		 * @return A container containing the read values.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
			requires appendable<_Ty, _VTy>
		[[nodiscard]] inline _Ty read_many(size_type _Count) const {
			_Ty ret;
			read_many(ret, _Count);
			return ret;
		}

		/**
		 * @brief Reads multiple values from the current address contiguously.
		 * @tparam _Ty The containing type. Must satisfy `insertable`.
		 * @tparam _VTy The type to read. Must satisfy `trivially_copyable`. Defaults to `_Ty::value_type`.
		 * @param[out] _Return A reference to a container to receive the read values.
		 * @param[in] _Count The number of values to read.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
			requires (!appendable<_Ty, _VTy>) && insertable<_Ty, _VTy>
		inline void read_many(_Ty& _Return, size_type _Count) const {
			invalid_state_exception::throw_on_condition_not_met(ISE_CONDITION(this->_Value != 0));

			__try {
				for (size_type i = 0; i < _Count; i++)
					_Return.insert(std::end(_Return), this->ptr<_VTy>()[i]);
			}
			__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
				throw access_violation_exception(*this, sizeof(_VTy) * _Count, memory_operation::read);
			}
		}

		/**
		 * @brief Reads multiple values from the current address contiguously.
		 * @tparam _Ty The containing type. Must satisfy `insertable`.
		 * @tparam _VTy The type to read. Must satisfy `trivially_copyable`. Defaults to `_Ty::value_type`.
		 * @param[in] _Count The number of values to read.
		 * @return A container containing the read values.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
			requires (!appendable<_Ty, _VTy>) && insertable<_Ty, _VTy>
		[[nodiscard]] inline _Ty read_many(size_type _Count) const {
			_Ty ret;
			read_many(ret, _Count);
			return ret;
		}

		/**
		 * @brief Reads a string of contiguous characters from the current address.
		 * @tparam _Ty The character type to read. Must satisfy `std::integral`.
		 * @param[out] _Return A reference to a variable to receive the read string.
		 * @return The number of characters read.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<std::integral _Ty>
		inline size_type read_string(std::basic_string<_Ty>& _Return) const {
			invalid_state_exception::throw_on_condition_not_met(ISE_CONDITION(this->_Value != 0));

			_Ty buffer[_StringMaxLength + 1];
			buffer[_StringMaxLength] = static_cast<_Ty>(0);

			size_type number_read = _StringMaxLength;
			__try {
				for (size_type i = 0; i < _StringMaxLength; i++) {
					buffer[i] = this->ptr<_Ty>()[i];

					if (!buffer[i]) {
						number_read = i;
						break;
					}
				}
			}
			__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
				throw access_violation_exception(*this, sizeof(_Ty) * number_read, memory_operation::read);
			}

			_Return = std::basic_string<_Ty>(buffer);
			return number_read;
		}

		/**
		 * @brief Reads a string of contiguous characters from the current address.
		 * @tparam _Ty The character type to read. Must satisfy `std::integral`.
		 * @param[out] _Return A reference to a variable to receive the read string.
		 * @param[in] _MaxCount The maximum number of characters to read.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<std::integral _Ty>
		inline void read_string(std::basic_string<_Ty>& _Return, size_type _MaxCount) {
			invalid_state_exception::throw_on_condition_not_met(ISE_CONDITION(this->_Value != 0));

			_Ty* buffer = new _Ty[_MaxCount + 1];
			buffer[_MaxCount] = static_cast<_Ty>(0);

			size_type number_read = _MaxCount;
			__try {
				for (size_type i = 0; i < _MaxCount; i++) {
					buffer[i] = this->ptr<_Ty>()[i];

					if (!buffer[i]) {
						number_read = i;
						break;
					}
				}
			}
			__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
				throw access_violation_exception(*this, sizeof(_Ty) * number_read, memory_operation::read);
			}

			_Return = std::basic_string<_Ty>(buffer);
			delete[] buffer;

			return number_read;
		}

		/**
		 * @brief Reads a string of contiguous characters from the current address.
		 * @tparam _Ty The character type to read. Must satisfy `std::integral`.
		 * @return The read string.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<std::integral _Ty>
		[[nodiscard]] inline std::basic_string<_Ty> read_string() {
			std::basic_string<_Ty> ret;
			read_string(ret);
			return ret;
		}

		/**
		 * @brief Reads a string of contiguous characters from the current address.
		 * @tparam _Ty The character type to read. Must satisfy `std::integral`.
		 * @param[in] _MaxCount The maximum number of characters to read.
		 * @return The read string.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<std::integral _Ty>
		[[nodiscard]] inline std::basic_string<_Ty> read_string(size_type _MaxCount) {
			std::basic_string<_Ty> ret;
			read_string(ret, _MaxCount);
			return ret;
		}

		/**
		 * @brief Advances the contained address by the passed pointer offsets and returns the result in a new instance.
		 * @tparam _Ty The containing type.
		 * @tparam _TTy The containing type without references. Must satisfy `std::ranges::range`. Defaults to `std::remove_reference_t<_Ty>`.
		 * @tparam _VTy The offset type. Must satisfy `std::integral`. Defaults to `_TTy::value_type`.
		 * @param[in] _Offsets The pointer chain offsets.
		 * @return An instance containing the new address.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<typename _Ty, typename _TTy = std::remove_reference_t<_Ty>, std::integral _VTy = typename _TTy::value_type>
			requires std::ranges::range<_TTy>
		[[nodiscard]] inline address_t walk_ptr(_Ty&& _Offsets) const {
			address_t a = *this;
			for (_VTy v : _Offsets) {
				a = a.read_one<value_type>();
				a += v;
			}
		}

		/**
		 * @brief Advances the contained address by the passed pointer offsets and overrides the contained pointer by the result.
		 * @tparam _Ty The containing type.
		 * @tparam _TTy The containing type without references. Must satisfy `std::ranges::range`. Defaults to `std::remove_reference_t<_Ty>`.
		 * @tparam _VTy The offset type. Must satisfy `std::integral`. Defaults to `_TTy::value_type`.
		 * @param[in] _Offsets The pointer chain offsets.
		 * @return A reference to the current instance.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<typename _Ty, typename _TTy = std::remove_reference_t<_Ty>, std::integral _VTy = typename _TTy::value_type>
			requires std::ranges::range<_TTy>
		inline address_t& walk_this_ptr(_Ty&& _Offsets) {
			for (_VTy v : _Offsets) {
				this->read_one(this->_Value);
				this->_Value += v;
			}

			return *this;
		}

		/**
		 * @brief Writes a single value to the address.
		 * @tparam _Ty The type to write.
		 * @tparam _TTy The type to write witout references. Must satisfy `trivially_copyable`.
		 * @param[in] _Value The value to write.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<typename _Ty, trivially_copyable _TTy = std::remove_reference_t<_Ty>>
		inline void write_one(_Ty&& _Value) {
			invalid_state_exception::throw_on_condition_not_met(ISE_CONDITION(this->_Value != 0));

			__try {
				*this->ptr<_TTy>() = std::forward<_Ty>(_Value);
			}
			__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
				throw access_violation_exception(*this, sizeof(_Ty), memory_operation::write);
			}
		}

		/**
		 * @brief Writes multiple values to the current address contiguously.
		 * @tparam _Ty The type to write. Must satisfy `trivially_copyable`.
		 * @param[in] _Values A pointer to a buffer containing the values to write.
		 * @param[in] _Count The number of values to write.
		 * @throws invalid_state_exception
		 * @throws argument_exception
		 * @throws access_violation_exception
		 */
		template<trivially_copyable _Ty>
		inline void write_many(_In_reads_(_Count) const _Ty* _Values, size_type _Count) {
			invalid_state_exception::throw_on_condition_not_met(ISE_CONDITION(this->_Value != 0));
			argument_exception::throw_if_null(AE_ARGUMENT(_Values));

			__try {
				for (size_type i = 0; i < _Count; i++)
					this->ptr<_Ty>()[i] = _Values[i];
			}
			__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
				throw access_violation_exception(*this, sizeof(_Ty) * _Count, memory_operation::write);
			}
		}

		/**
		 * @brief Writes multiple values to the current address contiguously.
		 * @tparam _Ty The type to write. Must satisfy `trivially_copyable`.
		 * @tparam _Count The number of values to write.
		 * @param[in] _Values A referece to a buffer containing the values to write.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<trivially_copyable _Ty, size_type _Count>
		inline void write_many(_In_reads_(_Count) const _Ty(&_Values)[_Count]) {
			write_many(_Values, _Count);
		}

		/**
		 * @brief Writes multiple values to the current address contiguously.
		 * @tparam _Ty The containing type.
		 * @tparam _TTy The containing type without references. Must satisfy `std::ranges::range`. Defaults to `std::remove_reference_t<_Ty>`.
		 * @tparam _VTy The type to write. Must satisfy `trivially_copyable`. Defaults to `_TTy::value_type`.
		 * @param[in] _Values A range containing the values to write.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<typename _Ty, std::ranges::range _TTy = std::remove_reference_t<_Ty>, trivially_copyable _VTy = typename _TTy::value_type>
		inline void write_many(_Ty&& _Values) {
			invalid_state_exception::throw_on_condition_not_met(ISE_CONDITION(this->_Value != 0));

			size_type i = 0;

			__try {
				for (auto&& v : _Values) {
					this->ptr<_VTy>()[i] = std::forward<decltype(v)>(v);
					i++;
				}
			}
			__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
				throw access_violation_exception(*this, sizeof(_VTy) * i, memory_operation::write);
			}
		}

		/**
		 * @brief Writes a string of contiguous characters to the current address.
		 * @tparam _STy The string type.
		 * @tparam _STTy The string type without references. Must satisfy `std::same_as<_STTy, std::basic_string<_Ty>>`. Defaults to `std::remove_reference_t<_STy>`.
		 * @tparam _Ty The character type to write. Must satisfy `std::integral`. Defaults to `_STTy::value_type`.
		 * @param[in] _Value The string to write.
		 * @throws invalid_state_exception
		 * @throws access_violation_exception
		 */
		template<typename _STy, typename _STTy = std::remove_reference_t<_STy>, std::integral _Ty = typename _STTy::value_type>
			requires std::same_as<_STTy, std::basic_string<_Ty>>
		inline void write_string(_STy&& _Value) {
			invalid_state_exception::throw_on_condition_not_met(ISE_CONDITION(this->_Value != 0));

			size_type i = 0;

			__try {
				for (_Ty c : _Value) {
					this->ptr<_Ty>()[i] = c;
					i++;
				}

				this->ptr<_Ty>()[i] = static_cast<_Ty>(0);
			}
			__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
				throw access_violation_exception(*this, sizeof(_Ty) * i, memory_operation::write);
			}
		}
	};
}

#endif // !__ARTEMIS_API_MEMORY_HXX__