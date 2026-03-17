#ifndef __ARTEMIS_API_MEMORY_HXX__
#define __ARTEMIS_API_MEMORY_HXX__

#include <type_traits>	// std::remove_reference_t
#include <concepts>		// std::derived_from, std::integral
#include <ranges>		// std::ranges::range
#include <vector>		// std::vector
#include <array>		// std::array
#include <utility>		// std::move

#include <cstdint>		// std::uintptr_t, std::size_t

#include <Windows.h>	// VirtualQuery, MEMORY_BASIC_INFORMATION

#include "Definitions.hxx"

#include "Exception.hxx"

namespace Artemis::API {
	template<typename _Ty>
	concept trivially_copyable = std::is_trivially_copyable_v<_Ty>;

	template<typename _Ty, typename _VTy>
	concept appendable = requires (_Ty _X, const _VTy & _Y) {
		_X.push_back(_Y);
	};

	template<typename _Ty, typename _VTy>
	concept insertable = requires (_Ty _X, const _VTy & _Y) {
		_X.insert(std::end(_X), _Y);
	};

	/// <summary>
	/// Represents a type of memory operation.
	/// </summary>
	enum class memory_operation {
		/// <summary>
		/// Represents the action of reading from memory.
		/// </summary>
		read,

		/// <summary>
		/// Represents the action of writing to memory.
		/// </summary>
		write,

		/// <summary>
		/// Represents the action of executing memory.
		/// </summary>
		execute
	};

	/// <summary>
	/// An exception type for memory access violations.
	/// </summary>
	class access_violation_exception : public system_exception {
		address_t _Address;
		size_t _Size;
		memory_operation _Operation;

		MEMORY_BASIC_INFORMATION _MBI;

		ARTEMIS_API static std::string format_message(address_t _Address, size_t _Size, memory_operation _Operation);

	public:
		/// <summary>
		/// Constructs an instance of access violation exception.
		/// </summary>
		/// <param name="_Address">- The address where the operation took place.</param>
		/// <param name="_Size">- The number of bytes past the address that were accessed.</param>
		/// <param name="_Operation">- The memory operation that took place.</param>
		ARTEMIS_API access_violation_exception(address_t _Address, size_t _Size, memory_operation _Operation);

		/// <summary>
		/// Constructs an instance of access violation exception.
		/// </summary>
		/// <typeparam name="T">The inner exception type.</typeparam>
		/// <param name="_Address">- The address where the operation took place.</param>
		/// <param name="_Size">- The number of bytes past the address that were accessed.</param>
		/// <param name="_Operation">- The memory operation that took place.</param>
		/// <param name="_InnerException">- The inner exception of the instance.</param>
		template<typename _Ty>
			requires std::derived_from<std::remove_reference_t<_Ty>, exception>
		inline access_violation_exception(address_t _Address, size_t _Size, memory_operation _Operation, _Ty&& _InnerException) : system_exception(format_message(_Address, _Size, _Operation), std::forward<_Ty>(_InnerException)), _Address(_Address), _Size(_Size), _Operation(_Operation) {
			VirtualQuery(_Address, &this->_MBI, sizeof(this->_MBI));
		}

		/// <summary>
		/// Gets the address where the exception occured.
		/// </summary>
		/// <returns>The instance address.</returns>
		ARTEMIS_API address_t address() const noexcept;

		/// <summary>
		/// Gets the size of the operation in bytes.
		/// </summary>
		/// <returns>The operation size.</returns>
		ARTEMIS_API size_t size() const noexcept;

		/// <summary>
		/// Gets the type of memory operation that threw an exception.
		/// </summary>
		/// <returns>The memory operation.</returns>
		ARTEMIS_API memory_operation operation() const noexcept;

		/// <summary>
		/// Gets information about the memory region at the time of the exception.
		/// </summary>
		/// <returns>The memory region information.</returns>
		ARTEMIS_API const MEMORY_BASIC_INFORMATION* mbi() const noexcept;
	};

	/// <summary>
	/// Represents an x64 memory address.
	/// </summary>
	class address_t {
	public:
		/// <summary>
		/// The instance value type.
		/// </summary>
		using value_type = std::uintptr_t;

		/// <summary>
		/// The size type.
		/// </summary>
		using size_type = std::size_t;

	private:
		static constexpr std::size_t _StringMaxLength = 256;

		value_type _Value;

	public:
		/// <summary>
		/// Constructs a null instance of address_t.
		/// </summary>
		constexpr address_t() : _Value(0) {}

		/// <summary>
		/// Constructs an instance of address_t from an integral value.
		/// </summary>
		/// <param name="_Value">- The instance value.</param>
		constexpr address_t(value_type _Value) : _Value(_Value) {}

		/// <summary>
		/// Constructs a null instance of address_t.
		/// </summary>
		constexpr address_t(std::nullptr_t) : _Value(0) {}

		/// <summary>
		/// Constructs an instance of address_t from a pointer of any type.
		/// </summary>
		/// <typeparam name="T">The pointer type.</typeparam>
		/// <param name="_Value">- The instance value.</param>
		template<typename _Ty>
		inline address_t(_Ty* _Value) : _Value(reinterpret_cast<value_type>(_Value)) {}

		/// <summary>
		/// Gets the current instance value.
		/// </summary>
		/// <returns>The instance value.</returns>
		constexpr value_type value() const noexcept { return this->_Value; }

		/// <summary>
		/// Gets the current instance value as a pointer of type T.
		/// </summary>
		/// <typeparam name="T">The pointer type.</typeparam>
		/// <returns>A pointer of type T.</returns>
		template<typename _Ty>
		inline _Ty* ptr() const noexcept { return reinterpret_cast<_Ty*>(this->_Value); }

		/// <summary>
		/// Gets a pointer to the current value buffer.
		/// </summary>
		/// <returns>A pointer to the current value.</returns>
		constexpr value_type* buffer() noexcept { return &this->_Value; }

		/// <summary>
		/// Implicit conversion from instance value to value_type.
		/// </summary>
		constexpr operator value_type () const noexcept { return this->_Value; }

		/// <summary>
		/// Implicit conversion from instance value to pointer of type T.
		/// </summary>
		/// <typeparam name="T">The pointer type.</typeparam>
		template<typename _Ty>
		inline operator _Ty* () const noexcept { return reinterpret_cast<_Ty*>(this->_Value); }

		/// <summary>
		/// Implicit conversion from instance value to bool. Returns true if the instance value is not null, otherwise false.
		/// </summary>
		constexpr operator bool() const noexcept { return !!(this->_Value); }

		/// <summary>
		/// Overload of the = operator.
		/// </summary>
		/// <param name="_Value">- The value to set.</param>
		/// <returns>A reference to the current instance.</returns>
		constexpr address_t& operator=(value_type _Value) noexcept { this->_Value = _Value; }

		/// <summary>
		/// Overload of the + operator.
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <param name="_Right">- The value to add.</param>
		/// <returns>A new object as the result of the operation.</returns>
		template<std::integral _Ty>
		constexpr address_t operator+(_Ty _Right) const noexcept { return address_t(this->_Value + _Right); }

		/// <summary>
		/// Overload of the += operator.
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <param name="_Right">- The value to add to the current instance.</param>
		/// <returns>A reference to the current instance.</returns>
		template<std::integral _Ty>
		constexpr address_t& operator+=(_Ty _Right) noexcept {
			this->_Value += _Right;
			return *this;
		}

		/// <summary>
		/// Overload of the - operator.
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <param name="_Right">- The value to subtract.</param>
		/// <returns>A new object as the result of the operation.</returns>
		template<std::integral _Ty>
		constexpr address_t operator-(_Ty _Right) const noexcept { return address_t(this->_Value - _Right); }

		/// <summary>
		/// Overload of the -= operator.
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <param name="_Right">- The value to subtract from the current instance.</param>
		/// <returns>A reference to the current instance.</returns>
		template<std::integral _Ty>
		constexpr address_t& operator-=(_Ty _Right) noexcept {
			this->_Value -= _Right;
			return *this;
		}

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

		template<trivially_copyable _Ty>
		inline _Ty read_one() const {
			_Ty ret;
			read_one(ret);
			return ret;
		}

		template<trivially_copyable _Ty>
		inline void read_many(_Ty* _Return, size_type _Count) const {
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

		template<trivially_copyable _Ty, size_type _Count>
		inline void read_many(_Ty(&_Return)[_Count]) const {
			read_many(_Return, _Count);
		}

		template<trivially_copyable _Ty, size_type _Count>
		inline void read_many(std::array<_Ty, _Count>& _Return) const {
			read_many(_Return.data(), _Count);
		}

		template<trivially_copyable _Ty, size_type _Count>
		inline std::array<_Ty, _Count> read_many() const {
			std::array<_Ty, _Count> ret;
			read_many(ret);
			return ret;
		}

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

		template<trivially_copyable _Ty>
		inline std::vector<_Ty> read_many(size_type _Count) const {
			std::vector<_Ty> ret;
			read_many(ret, _Count);
			return ret;
		}

		template<typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
			requires appendable<_Ty, _VTy>
		inline _Ty read_many(size_type _Count) const {
			_Ty ret;
			read_many(ret, _Count);
			return ret;
		}

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

		template<typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
			requires (!appendable<_Ty, _VTy>) && insertable<_Ty, _VTy>
		inline _Ty read_many(size_type _Count) const {
			_Ty ret;
			read_many(ret, _Count);
			return ret;
		}

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

		template<std::integral _Ty>
		inline std::basic_string<_Ty> read_string() {
			std::basic_string<_Ty> ret;
			read_string(ret);
			return ret;
		}

		template<std::integral _Ty>
		inline std::basic_string<_Ty> read_string(size_type _MaxCount) {
			std::basic_string<_Ty> ret;
			read_string(ret, _MaxCount);
			return ret;
		}

		template<typename _Ty, typename _TTy = std::remove_reference_t<_Ty>, std::integral _VTy = typename _TTy::value_type>
			requires std::ranges::range<_TTy>
		inline address_t walk_ptr(_Ty&& _Offsets) const {
			address_t a = *this;
			for (_VTy v : _Offsets) {
				a = a.read_one<value_type>();
				a += v;
			}
		}

		template<typename _Ty, typename _TTy = std::remove_reference_t<_Ty>, std::integral _VTy = typename _TTy::value_type>
			requires std::ranges::range<_TTy>
		inline address_t& walk_this_ptr(_Ty&& _Offsets) {
			for (_VTy v : _Offsets) {
				this->read_one(this->_Value);
				this->_Value += v;
			}

			return *this;
		}

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

		template<trivially_copyable _Ty>
		inline void write_many(const _Ty* _Values, size_type _Count) {
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

		template<trivially_copyable _Ty, size_type _Count>
		inline void write_many(const _Ty(&_Values)[_Count]) {
			write_many(_Values, _Count);
		}

		template<typename _Ty, typename _TTy = std::remove_reference_t<_Ty>, trivially_copyable _VTy = typename _TTy::value_type>
			requires std::ranges::range<_TTy>
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

		template<std::integral _Ty, typename _STy>
			requires std::same_as<std::remove_reference_t<_STy>, std::basic_string<_Ty>>
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

	template<trivially_copyable _Ty>
	inline void read_one(address_t _Address, _Ty& _Return) {
		_Address.read_one(_Return);
	}

	template<typename _RTy, trivially_copyable _Ty>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline void read_one(address_t _Address, _RTy&& _Offsets, _Ty& _Return) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_one(_Return);
	}

	template<trivially_copyable _Ty>
	inline _Ty read_one(address_t _Address) {
		return _Address.read_one<_Ty>();
	}

	template<typename _RTy, trivially_copyable _Ty>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline _Ty read_one(address_t _Address, _RTy&& _Offsets) {
		return _Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_one<_Ty>();
	}

	template<trivially_copyable _Ty>
	inline void read_many(address_t _Address, _Ty* _Return, address_t::size_type _Count) {
		_Address.read_many(_Return, _Count);
	}

	template<typename _RTy, trivially_copyable _Ty>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline void read_many(address_t _Address, _RTy&& _Offsets, _Ty* _Return, address_t::size_type _Count) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_many(_Return, _Count);
	}

	template<trivially_copyable _Ty, address_t::size_type _Count>
	inline void read_many(address_t _Address, _Ty(&_Return)[_Count]) {
		_Address.read_many(_Return);
	}

	template<typename _RTy, trivially_copyable _Ty, address_t::size_type _Count>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline void read_many(address_t _Address, _RTy&& _Offsets, _Ty(&_Return)[_Count]) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_many(_Return);
	}

	template<trivially_copyable _Ty, address_t::size_type _Count>
	inline void read_many(address_t _Address, std::array<_Ty, _Count>& _Return) {
		_Address.read_many(_Return);
	}

	template<typename _RTy, trivially_copyable _Ty, address_t::size_type _Count>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline void read_many(address_t _Address, _RTy&& _Offsets, std::array<_Ty, _Count>& _Return) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_many(_Return);
	}

	template<trivially_copyable _Ty, address_t::size_type _Count>
	inline std::array<_Ty, _Count> read_many(address_t _Address) {
		return _Address.read_many<_Ty, _Count>();
	}

	template<typename _RTy, trivially_copyable _Ty, address_t::size_type _Count>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline std::array<_Ty, _Count> read_many(address_t _Address, _RTy&& _Offsets) {
		return _Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_many<_Ty, _Count>();
	}

	template<typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
		requires appendable<_Ty, _VTy>
	inline void read_many(address_t _Address, _Ty& _Return, address_t::size_type _Count) {
		_Address.read_many(_Return, _Count);
	}

	template<typename _RTy, typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
		requires std::ranges::range<std::remove_reference_t<_RTy>> && appendable<_Ty, _VTy>
	inline void read_many(address_t _Address, _RTy&& _Offsets, _Ty& _Return, address_t::size_type _Count) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_many(_Return, _Count);
	}

	template<trivially_copyable _Ty>
	inline std::vector<_Ty> read_many(address_t _Address, address_t::size_type _Count) {
		return _Address.read_many<_Ty>(_Count);
	}

	template<typename _RTy, trivially_copyable _Ty>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline std::vector<_Ty> read_many(address_t _Address, _RTy&& _Offsets, address_t::size_type _Count) {
		return _Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_many<_Ty>(_Count);
	}

	template<typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
		requires appendable<_Ty, _VTy>
	inline _Ty read_many(address_t _Address, address_t::size_type _Count) {
		return _Address.read_many<_Ty>(_Count);
	}

	template<typename _RTy, typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
		requires std::ranges::range<std::remove_reference_t<_RTy>> && appendable<_Ty, _VTy>
	inline _Ty read_many(address_t _Address, _RTy&& _Offsets, address_t::size_type _Count) {
		return _Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_many<_Ty>(_Count);
	}

	template<typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
		requires (!appendable<_Ty, _VTy>) && insertable<_Ty, _VTy>
	inline void read_many(address_t _Address, _Ty& _Return, address_t::size_type _Count) {
		_Address.read_many(_Return, _Count);
	}

	template<typename _RTy, typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
		requires std::ranges::range<std::remove_reference_t<_RTy>> && (!appendable<_Ty, _VTy>) && insertable<_Ty, _VTy>
	inline void read_many(address_t _Address, _RTy&& _Offsets, _Ty& _Return, address_t::size_type _Count) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_many(_Return, _Count);
	}

	template<typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
		requires (!appendable<_Ty, _VTy>) && insertable<_Ty, _VTy>
	inline _Ty read_many(address_t _Address, address_t::size_type _Count) {
		return _Address.read_many<_Ty>(_Count);
	}

	template<typename _RTy, typename _Ty, trivially_copyable _VTy = typename _Ty::value_type>
		requires std::ranges::range<std::remove_reference_t<_RTy>> && (!appendable<_Ty, _VTy>) && insertable<_Ty, _VTy>
	inline _Ty read_many(address_t _Address, _RTy&& _Offsets, address_t::size_type _Count) {
		return _Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_many<_Ty>(_Count);
	}

	template<std::integral _Ty>
	inline address_t::size_type read_string(address_t _Address, std::basic_string<_Ty>& _Return) {
		return _Address.read_string(_Return);
	}

	template<typename _RTy, std::integral _Ty>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline address_t::size_type read_string(address_t _Address, _RTy&& _Offsets, std::basic_string<_Ty>& _Return) {
		return _Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_string(_Return);
	}

	template<std::integral _Ty>
	inline void read_string(address_t _Address, std::basic_string<_Ty>& _Return, address_t::size_type _MaxCount) {
		_Address.read_string(_Return, _MaxCount);
	}

	template<typename _RTy, std::integral _Ty>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline void read_string(address_t _Address, _RTy&& _Offsets, std::basic_string<_Ty>& _Return, address_t::size_type _MaxCount) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_string(_Return, _MaxCount);
	}

	template<std::integral _Ty>
	inline std::basic_string<_Ty> read_string(address_t _Address) {
		return _Address.read_string<_Ty>();
	}

	template<typename _RTy, std::integral _Ty>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline std::basic_string<_Ty> read_string(address_t _Address, _RTy&& _Offsets) {
		return _Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_string<_Ty>();
	}

	template<std::integral _Ty>
	inline std::basic_string<_Ty> read_string(address_t _Address, address_t::size_type _MaxCount) {
		return _Address.read_string<_Ty>(_MaxCount);
	}

	template<typename _RTy, std::integral _Ty>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline std::basic_string<_Ty> read_string(address_t _Address, _RTy&& _Offsets, address_t::size_type _MaxCount) {
		return _Address.walk_ptr(std::forward<_RTy>(_Offsets)).read_string<_Ty>(_MaxCount);
	}

	template<typename _Ty, trivially_copyable _TTy = std::remove_reference_t<_Ty>>
	inline void write_one(address_t _Address, _Ty&& _Value) {
		_Address.write_one(std::forward<_Ty>(_Value));
	}

	template<typename _RTy, typename _Ty, trivially_copyable _TTy = std::remove_reference_t<_Ty>>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline void write_one(address_t _Address, _RTy&& _Offsets, _Ty&& _Value) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).write_one(std::forward<_Ty>(_Value));
	}

	template<trivially_copyable _Ty>
	inline void write_many(address_t _Address, const _Ty* _Values, address_t::size_type _Count) {
		_Address.write_many(_Values, _Count);
	}

	template<typename _RTy, trivially_copyable _Ty>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline void write_many(address_t _Address, _RTy&& _Offsets, const _Ty* _Values, address_t::size_type _Count) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).write_many(_Values, _Count);
	}

	template<trivially_copyable _Ty, address_t::size_type _Count>
	inline void write_many(address_t _Address, const _Ty(&_Values)[_Count]) {
		_Address.write_many(_Values);
	}

	template<typename _RTy, trivially_copyable _Ty, address_t::size_type _Count>
		requires std::ranges::range<std::remove_reference_t<_RTy>>
	inline void write_many(address_t _Address, _RTy&& _Offsets, const _Ty(&_Values)[_Count]) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).write_many(_Values);
	}

	template<typename _Ty, typename _TTy = std::remove_reference_t<_Ty>, trivially_copyable _VTy = typename _TTy::value_type>
		requires std::ranges::range<_TTy>
	inline void write_many(address_t _Address, _Ty&& _Values) {
		_Address.write_many(std::forward<_Ty>(_Values));
	}

	template<typename _RTy, typename _Ty, typename _TTy = std::remove_reference_t<_Ty>, trivially_copyable _VTy = typename _TTy::value_type>
		requires std::ranges::range<std::remove_reference_t<_RTy>> && std::ranges::range<_TTy>
	inline void write_many(address_t _Address, _RTy&& _Offsets, _Ty&& _Values) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).write_many(std::forward<_Ty>(_Values));
	}

	template<std::integral _Ty, typename _STy>
		requires std::same_as<std::remove_reference_t<_STy>, std::basic_string<_Ty>>
	inline void write_string(address_t _Address, _STy&& _Value) {
		_Address.write_string(std::forward<_STy>(_Value));
	}

	template<typename _RTy, std::integral _Ty, typename _STy>
		requires std::ranges::range<std::remove_reference_t<_RTy>> && std::same_as<std::remove_reference_t<_STy>, std::basic_string<_Ty>>
	inline void write_string(address_t _Address, _RTy&& _Offsets, _STy&& _Value) {
		_Address.walk_ptr(std::forward<_RTy>(_Offsets)).write_string(std::forward<_STy>(_Value));
	}
}

#endif // !__ARTEMIS_API_MEMORY_HXX__