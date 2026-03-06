#ifndef __ARTEMIS_API_MEMORY_HXX__
#define __ARTEMIS_API_MEMORY_HXX__

#include <type_traits>	// std::is_pointer_v, std::is_integral_v, std::is_same_v
#include <concepts>
#include <vector>		// std::vector
#include <array>		// std::array
#include <utility>		// std::move
#include <ranges>		// std::ranges::range, std::ranges::iterator_t

#include <Windows.h>	// VirtualQuery, MEMORY_BASIC_INFORMATION

#include "Definitions.hxx"

#include "Exception.hxx"

namespace Artemis::API {
#pragma region address_t, ptroffset_t, ptrchain_t

	/// <summary>
	/// Represents an x64 memory address.
	/// </summary>
	class address_t {
	public:
		/// <summary>
		/// The instance value type.
		/// </summary>
		using value_type = unsigned long long;

	private:
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
			requires (std::is_pointer_v<_Ty>)
		inline address_t(_Ty _Value) : _Value(reinterpret_cast<value_type>(_Value)) {}

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
			requires (std::is_pointer_v<_Ty>)
		inline operator _Ty () const noexcept { return reinterpret_cast<T>(this->_Value); }

		/// <summary>
		/// Implicit conversion from instance value to bool. Returns true if the instance value is not null, otherwise false.
		/// </summary>
		constexpr operator bool () const noexcept { return !!(this->_Value); }

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
	};

	/// <summary>
	/// Represents an offset in a pointer chain.
	/// </summary>
	using ptroffset_t = unsigned short;

	/// <summary>
	/// Represents a pointer chain.
	/// </summary>
	using ptrchain_t = const std::vector<ptroffset_t>&;

#pragma endregion

#pragma region access_violation_exception class.

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
		template<derived_exception_type T>
		inline access_violation_exception(address_t _Address, size_t _Size, memory_operation _Operation, T&& _InnerException) : system_exception(format_message(_Address, _Size, _Operation), std::forward<T>(_InnerException)), _Address(_Address), _Size(_Size), _Operation(_Operation) {
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

#pragma endregion

#pragma region Overloads of read with return by pointer.

	template<typename _Ty>
	inline void read(address_t _Address, _Ty* _Return) {
		argument_exception::throw_if_null(AE_ARGUMENT(_Address));
		argument_exception::throw_if_null(AE_ARGUMENT(_Return));

		__try {
			*_Return = *_Address.ptr<_Ty>();
		}
		__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
			throw access_violation_exception(_Address, sizeof(_Ty), memory_operation::read);
		}
	}

	template<typename _Ty>
	inline void read(address_t _Address, _Ty* _Return, int _Count) {
		argument_exception::throw_if_null(AE_ARGUMENT(_Address));
		argument_exception::throw_if_null(AE_ARGUMENT(_Return));
		argument_exception::throw_if_less_than_or_equal(AE_ARGUMENT(_Count), 0);
			
		__try {
			for (int i = 0; i < _Count; i++)
				_Return[i] = _Address.ptr<_Ty>()[i];
		}
		__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
			throw access_violation_exception(_Address, sizeof(_Ty) * _Count, memory_operation::read);
		}
	}

	template<typename _Ty, size_t _Size>
		requires(_Size != 0)
	inline void read(address_t _Address, _Ty(&_Return)[_Size]) {
		read(_Address, _Return, _Size);
	}

	template<typename _Ty>
	inline void read(address_t _Address, std::vector<_Ty>* _Return, int _Count) {
		argument_exception::throw_if_null(AE_ARGUMENT(_Address));
		argument_exception::throw_if_null(AE_ARGUMENT(_Return));
		argument_exception::throw_if_less_than_or_equal(AE_ARGUMENT(_Count), 0);

		std::vector<_Ty> ret(_Count);
		__try {
			for (int i = 0; i < _Count; i++)
				ret.push_back(_Address.ptr<_Ty>()[i]);
		}
		__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
			throw access_violation_exception(_Address, sizeof(_Ty) * _Count, memory_operation::read);
		}

		_Return->swap(ret);
	}

	template<typename _Ty, size_t _Size>
		requires(_Size != 0)
	inline void read(address_t _Address, std::array<_Ty, _Size>* _Return) {
		argument_exception::throw_if_null(AE_ARGUMENT(_Address));
		argument_exception::throw_if_null(AE_ARGUMENT(_Return));

		std::array<_Ty, _Size> ret;
		__try {
			for (int i = 0; i < _Size; i++)
				ret[i] = _Address.ptr<_Ty>()[i];
		}
		__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
			throw access_violation_exception(_Address, sizeof(_Ty) * _Size, memory_operation::read);
		}

		_Return->swap(ret);
	}

#pragma endregion

#pragma region Overloads of read with return by value.

	template<typename _Ty>
	inline _Ty read(address_t _Address) {
		argument_exception::throw_if_null(AE_ARGUMENT(_Address));

		_Ty ret;
		read(_Address, &ret);

		return ret;
	}

	template<typename _Ty>
	inline std::vector<_Ty> read(address_t _Address, int _Count) {
		argument_exception::throw_if_null(AE_ARGUMENT(_Address));
		argument_exception::throw_if_less_than_or_equal(AE_ARGUMENT(_Count), 0);

		std::vector<_Ty> ret;
		read(_Address, &ret, _Count);

		return ret;
	}

	template<typename _Ty, size_t _Size>
		requires(_Size != 0)
	inline std::array<_Ty, _Size> read(address_t _Address) {
		argument_exception::throw_if_null(AE_ARGUMENT(_Address));

		std::array<_Ty, _Size> ret;
		read(_Address, &ret);

		return ret;
	}

#pragma endregion

#pragma region Overloads of get_address.

	ARTEMIS_API void get_address(address_t _Address, ptrchain_t _Offsets, address_t* _Return);

	ARTEMIS_API address_t get_address(address_t _Address, ptrchain_t _Offsets);

#pragma endregion

#pragma region Overloads of read_ptr with return by pointer.

	template<typename _Ty>
	inline void read_ptr(address_t _Address, ptrchain_t _Offsets, _Ty* _Return) {
		get_address(_Address, _Offsets, &_Address);
		read(_Address, _Return);
	}

	template<typename _Ty>
	inline void read_ptr(address_t _Address, ptrchain_t _Offsets, _Ty* _Return, int _Count) {
		get_address(_Address, _Offsets, &_Address);
		read(_Address, _Return, _Count);
	}

	template<typename _Ty, size_t _Size>
		requires(_Size != 0)
	inline void read_ptr(address_t _Address, ptrchain_t _Offsets, _Ty(&_Return)[_Size]) {
		read_ptr(_Address, _Offsets, _Return, _Size);
	}

	template<typename _Ty>
	inline void read_ptr(address_t _Address, ptrchain_t _Offsets, std::vector<_Ty>* _Return, int _Count) {
		get_address(_Address, _Offsets, &_Address);
		read(_Address, _Return, _Count);
	}

	template<typename _Ty, size_t _Size>
		requires(_Size != 0)
	inline void read_ptr(address_t _Address, ptrchain_t _Offsets, std::array<_Ty, _Size>* _Return) {
		get_address(_Address, _Offsets, &_Address);
		read(_Address, _Return);
	}

#pragma endregion

#pragma region Overloads of read_ptr with return by value.

	template<typename _Ty>
	inline _Ty read_ptr(address_t _Address, ptrchain_t _Offsets) {
		get_address(_Address, _Offsets, &_Address);

		_Ty ret;
		read(_Address, &ret);

		return ret;
	}

	template<typename _Ty>
	inline std::vector<_Ty> read_ptr(address_t _Address, ptrchain_t _Offsets, int _Count) {
		get_address(_Address, _Offsets, &_Address);

		std::vector<_Ty> ret;
		read(_Address, &ret, _Count);

		return ret;
	}

	template<typename _Ty, size_t _Size>
		requires(_Size != 0)
	inline std::array<_Ty, _Size> read_ptr(address_t _Address, ptrchain_t _Offsets) {
		get_address(_Address, _Offsets, &_Address);

		std::array<_Ty, _Size> ret;
		read(_Address, &ret);

		return ret;
	}

#pragma endregion

#pragma region Overloads of write.

	template<typename _Ty>
	inline void write(address_t _Address, _Ty&& _Value) {
		argument_exception::throw_if_null(AE_ARGUMENT(_Address));

		__try {
			*_Address.ptr<_Ty>() = std::forward<_Ty>(_Value);
		}
		__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
			throw access_violation_exception(_Address, sizeof(_Ty), memory_operation::write);
		}
	}

	template<typename _Ty>
	inline void write(address_t _Address, const _Ty* const _Values, int _Count) {
		argument_exception::throw_if_null(AE_ARGUMENT(_Address));
		argument_exception::throw_if_null(AE_ARGUMENT(_Values));
		argument_exception::throw_if_less_than_or_equal(AE_ARGUMENT(_Count), 0);

		__try {
			for (int i = 0; i < _Count; i++)
				_Address.ptr<_Ty>()[i] = _Values[0];
		}
		__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
			throw access_violation_exception(_Address, sizeof(_Ty) * _Count, memory_operation::write);
		}
	}

	template<typename _Ty, size_t _Size>
		requires(_Size != 0)
	inline void write(address_t _Address, const _Ty(&_Values)[_Size]) {
		write(_Address, _Values, _Size);
	}


	namespace _ {
		template<typename _Ty>
		constexpr bool is_sized = requires (_Ty _X) {
			{ _X.size() } -> std::integral;
		};
	}

	template<std::ranges::range _Ty>
	inline void write(address_t _Address, _Ty&& _Range) {
		argument_exception::throw_if_null(AE_ARGUMENT(_Address));

		address_t original = _Address;

		__try {
			for (auto&& value : _Range) {
				*_Address.ptr<std::remove_reference_t<decltype(value)>>() = std::forward<decltype(value)>(value);
				_Address += sizeof(value);
			}
		}
		__except (seh_filter(GetExceptionInformation()).handle_on(EXCEPTION_ACCESS_VIOLATION)) {
			if constexpr (_::is_sized<_Ty>)
				throw access_violation_exception(original, sizeof(_Ty) * _Range.size(), memory_operation::write);
			else
				throw access_violation_exception(_Address, sizeof(_Ty), memory_operation::write);
		}
	}

#pragma endregion

#pragma region Overloads of write_ptr.

	template<typename _Ty>
	inline void write_ptr(address_t _Address, ptrchain_t _Offsets, _Ty&& _Value) {
		get_address(_Address, _Offsets, &_Address);
		write(_Address, std::forward<_Ty>(_Value));
	}

	template<typename _Ty>
	inline void write_ptr(address_t _Address, ptrchain_t _Offsets, const _Ty* const _Values, int _Count) {
		get_address(_Address, _Offsets, &_Address);
		write(_Address, _Values, _Count);
	}

	template<typename _Ty, size_t _Size>
		requires(_Size != 0)
	inline void write_ptr(address_t _Address, ptrchain_t _Offsets, const _Ty(&_Values)[_Size]) {
		get_address(_Address, _Offsets, &_Address);
		write(_Address, _Values, _Size);
	}

	template<std::ranges::range _Ty>
	inline void write_ptr(address_t _Address, ptrchain_t _Offsets, _Ty&& _Range) {
		get_address(_Address, _Offsets, &_Address);
		write(_Address, std::forward<_Ty>(_Range));
	}

#pragma endregion
}

#endif // !__ARTEMIS_API_MEMORY_HXX__