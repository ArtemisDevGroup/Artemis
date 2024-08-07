#ifndef __ARTEMIS_API_MEMORY_HXX__
#define __ARTEMIS_API_MEMORY_HXX__

#include <type_traits>	// std::is_pointer_v, std::is_integral_v
#include <vector>		// std::vector
#include <array>		// std::array
#include <utility>		// std::move

#include <Windows.h>	// VirtualQuery, MEMORY_BASIC_INFORMATION

#include "Definitions.hxx"

#include "Error.hxx"

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
		template<typename T>
			requires(std::is_pointer_v<T>)
		inline address_t(T _Value) : _Value(reinterpret_cast<value_type>(_Value)) {}

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
		template<typename T>
		inline T* ptr() const noexcept { return *reinterpret_cast<T*>(this->_Value); }

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
		template<typename T>
			requires(std::is_pointer_v<T>)
		inline operator T () const noexcept { return reinterpret_cast<T>(this->_Value); }

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
		template<typename T>
			requires(std::is_integral_v<T>)
		constexpr address_t operator+(T _Right) const noexcept { return address_t(this->_Value + _Right); }

		/// <summary>
		/// Overload of the += operator.
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <param name="_Right">- The value to add to the current instance.</param>
		/// <returns>A reference to the current instance.</returns>
		template<typename T>
			requires(std::is_integral_v<T>)
		constexpr address_t& operator+=(T _Right) noexcept {
			this->_Value += _Right;
			return *this;
		}

		/// <summary>
		/// Overload of the - operator.
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <param name="_Right">- The value to subtract.</param>
		/// <returns>A new object as the result of the operation.</returns>
		template<typename T>
			requires(std::is_integral_v<T>)
		constexpr address_t operator-(T _Right) const noexcept { return address_t(this->_Value - _Right); }

		/// <summary>
		/// Overload of the -= operator.
		/// </summary>
		/// <typeparam name="T">Integral type.</typeparam>
		/// <param name="_Right">- The value to subtract from the current instance.</param>
		/// <returns>A reference to the current instance.</returns>
		template<typename T>
			requires(std::is_integral_v<T>)
		constexpr address_t& operator-=(T _Right) noexcept {
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
	class access_violation_exception : public exception {
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
		inline access_violation_exception(address_t _Address, size_t _Size, memory_operation _Operation, const T& _InnerException) : exception(format_message(_Address, _Size, _Operation).c_str()), _Address(_Address), _Size(_Size), _Operation(_Operation) {
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
		ARTEMIS_API const MEMORY_BASIC_INFORMATION& mbi() const noexcept;
	};

#pragma endregion

#pragma region Overloads of read with return by pointer.

	/// <summary>
	/// Reads a value from an address.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <param name="_Address">- The address to read from.</param>
	/// <param name="_Return">- A pointer to a variable receiving the read value.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline void read(address_t _Address, T* _Return) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");
		if (!_Return)
			throw argument_exception("Pointer cannot be null", "_Return");

		__try {
			*_Return = *_Address.ptr<T>();
		}
		__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
			throw access_violation_exception(_Address, sizeof(T), memory_operation::read);
		}

		__stack_escape();
	}

	/// <summary>
	/// Reads an array of values from an address.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <param name="_Address">- The address to read from.</param>
	/// <param name="_Return">- A pointer to a buffer to receive the read values.</param>
	/// <param name="_Count">- The number of values to read.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline void read(address_t _Address, T* _Return, int _Count) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");
		if (!_Return)
			throw argument_exception("Pointer cannot be null", "_Return");
		if (_Count <= 0)
			throw argument_exception("Argument cannot be less than or equal to zero.", "_Count");
			
		__try {
			for (int i = 0; i < _Count; i++)
				_Return[i] = _Address.ptr<T>()[i];
		}
		__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
			throw access_violation_exception(_Address, sizeof(T) * _Count, memory_operation::read);
		}

		__stack_escape();
	}

	/// <summary>
	/// Reads an array of values from an address.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <typeparam name="N">The number of values to read.</typeparam>
	/// <param name="_Address">- The address to read from.</param>
	/// <param name="_Return">- A reference to a buffer to receive the read values.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T, size_t N>
		requires(N != 0)
	inline void read(address_t _Address, T(&_Return)[N]) {
		__stack_record();
		__stack_rethrow(read(_Address, _Return, N));
		__stack_escape();
	}

	/// <summary>
	/// Reads an array of values from an address and returns them in the form of an std::vector.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <param name="_Address">- The address to read from.</param>
	/// <param name="_Return">- A pointer to an std::vector object to receive the read values.</param>
	/// <param name="_Count">- The number of values to read.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline void read(address_t _Address, std::vector<T>* _Return, int _Count) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");
		if (!_Return)
			throw argument_exception("Pointer cannot be null", "_Return");
		if (_Count <= 0)
			throw argument_exception("Argument cannot be less than or equal to zero.", "_Count");

		std::vector<T> ret;
		__try {
			for (int i = 0; i < _Count; i++)
				ret.push_back(_Address.ptr<T>()[i]);
		}
		__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
			throw access_violation_exception(_Address, sizeof(T) * _Count, memory_operation::read);
		}

		_Return->swap(ret);

		__stack_escape();
	}

	/// <summary>
	/// Reads an array of values from an address and returns them in the form of an std::array.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <typeparam name="N">The number of values to read.</typeparam>
	/// <param name="_Address">- The address to read from.</param>
	/// <param name="_Return">- A pointer to an std::array object to receive the read values.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T, size_t N>
		requires(N != 0)
	inline void read(address_t _Address, std::array<T, N>* _Return) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");
		if (!_Return)
			throw argument_exception("Pointer cannot be null", "_Return");

		std::array<T, N> ret;
		__try {
			for (int i = 0; i < _Count; i++)
				ret[i] = _Address.ptr<T>()[i];
		}
		__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
			throw access_violation_exception(_Address, sizeof(T) * _Count, memory_operation::read);
		}

		_Return->swap(ret);

		__stack_escape();
	}

#pragma endregion

#pragma region Overloads of read with return by value.

	/// <summary>
	/// Reads a value from an address.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <param name="_Address">- The address to read from.</param>
	/// <returns>The read value.</returns>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline T read(address_t _Address) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");

		T ret;
		__stack_rethrow(read(_Address, &ret));

		__stack_escape();
		return ret;
	}

	/// <summary>
	/// Reads an array of values from an address and returns them in the form of an std::vector.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <param name="_Address">- The address to read from.</param>
	/// <param name="_Count">- The number of values to read.</param>
	/// <returns>An std::vector object containing the read values.</returns>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline std::vector<T> read(address_t _Address, int _Count) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");
		if (_Count <= 0)
			throw argument_exception("Argument cannot be less than or equal to zero.", "_Count");

		std::vector<T> ret;
		__stack_rethrow(read(_Address, &ret, _Count));

		__stack_escape();
		return ret;
	}

	/// <summary>
	/// Reads an array of values from an address and returns them in the form of an std::array.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <typeparam name="N">The number of values to read.</typeparam>
	/// <param name="_Address">- The address to read from.</param>
	/// <returns>An std::array object containing the read values.</returns>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T, size_t N>
		requires(N != 0)
	inline std::array<T, N> read(address_t _Address) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");

		std::array<T, N> ret;
		__stack_rethrow(read(_Address, &ret));

		__stack_escape();
		return ret;
	}

#pragma endregion

#pragma region Overloads of get_address.

	/// <summary>
	/// Gets the address at the end of a pointer chain.
	/// </summary>
	/// <param name="_Address">- The pointer base address.</param>
	/// <param name="_Offsets">- The pointer chain offsets.</param>
	/// <param name="_Return">- A pointer to a variable receiving the address at the end of the pointer chain.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	ARTEMIS_API void get_address(address_t _Address, ptrchain_t _Offsets, address_t* _Return);

	/// <summary>
	/// Gets the address at the end of a pointer chain.
	/// </summary>
	/// <param name="_Address">- The pointer base address.</param>
	/// <param name="_Offsets">- The pointer chain offsets.</param>
	/// <returns>The address at the end of the pointer chain.</returns>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	ARTEMIS_API address_t get_address(address_t _Address, ptrchain_t _Offsets);

#pragma endregion

#pragma region Overloads of read_ptr with return by pointer.

	/// <summary>
	/// Reads a value from the end of a pointer chain.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <param name="_Address">- The pointer base address.</param>
	/// <param name="_Offsets">- The pointer chain offsets.</param>
	/// <param name="_Return">A pointer to a variable receiving the read value.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline void read_ptr(address_t _Address, ptrchain_t _Offsets, T* _Return) {
		__stack_record();

		__stack_rethrow(get_address(_Address, _Offsets, &_Address));
		__stack_rethrow(read(_Address, _Return));

		__stack_escape();
	}

	/// <summary>
	/// Reads an array of values from the end of a pointer chain.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <param name="_Address">- The pointer base address.</param>
	/// <param name="_Offsets">- The pointer chain offsets.</param>
	/// <param name="_Return">- A pointer to a buffer to receive the read values.</param>
	/// <param name="_Count">- The number of values to read.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline void read_ptr(address_t _Address, ptrchain_t _Offsets, T* _Return, int _Count) {
		__stack_record();

		__stack_rethrow(get_address(_Address, _Offsets, &_Address));
		__stack_rethrow(read(_Address, _Return, _Count));

		__stack_escape();
	}

	/// <summary>
	/// Reads an array of values from the end of a pointer chain.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <typeparam name="N">The number of values to read.</typeparam>
	/// <param name="_Address">- The pointer base address.</param>
	/// <param name="_Offsets">- The pointer chain offsets.</param>
	/// <param name="_Return">- A reference to a buffer to receive the read values.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T, size_t N>
		requires(N != 0)
	inline void read_ptr(address_t _Address, ptrchain_t _Offsets, T(&_Return)[N]) {
		__stack_record();
		__stack_rethrow(read_ptr(_Address, _Offsets, _Return, N));
		__stack_escape();
	}

	/// <summary>
	/// Reads an array of values from the end of a pointer chain and returns them in the form of an std::vector.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <param name="_Address">- The pointer base address.</param>
	/// <param name="_Offsets">- The pointer chain offsets.</param>
	/// <param name="_Return">- A pointer to an std::vector object to receive the read values.</param>
	/// <param name="_Count">- The number of values to read.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline void read_ptr(address_t _Address, ptrchain_t _Offsets, std::vector<T>* _Return, int _Count) {
		__stack_record();

		__stack_rethrow(get_address(_Address, _Offsets, &_Address));
		__stack_rethrow(read(_Address, _Return, _Count));

		__stack_escape();
	}

	/// <summary>
	/// Reads an array of values from the end of a pointer chain and returns them in the form of an std::vector.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <typeparam name="N">The number of values to read.</typeparam>
	/// <param name="_Address">- The pointer base address.</param>
	/// <param name="_Offsets">- The pointer chain offsets.</param>
	/// <param name="_Return">- A pointer to an std::array object to receive the read values.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T, size_t N>
		requires(N != 0)
	inline void read_ptr(address_t _Address, ptrchain_t _Offset, std::array<T, N>* _Return) {
		__stack_record();

		__stack_rethrow(get_address(_Address, _Offsets, &_Address));
		__stack_rethrow(read(_Address, _Return));

		__stack_escape();
	}

#pragma endregion

#pragma region Overloads of read_ptr with return by value.

	/// <summary>
	/// Reads a value from the end of a pointer chain.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <param name="_Address">- The pointer base address.</param>
	/// <param name="_Offsets">- The pointer chain offsets.</param>
	/// <returns>The read value.</returns>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline T read_ptr(address_t _Address, ptrchain_t _Offsets) {
		__stack_record();

		__stack_rethrow(get_address(_Address, _Offsets, &_Address));

		T ret;
		__stack_rethrow(read(_Address, &ret));

		__stack_escape();
		return ret;
	}

	/// <summary>
	/// Reads an array of values from the end of a pointer chain and returns them in the form of an std::vector.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <param name="_Address">- The pointer base address.</param>
	/// <param name="_Offsets">- The pointer chain offsets.</param>
	/// <param name="_Count">- The number of values to read.</param>
	/// <returns>An std::vector object containing the read values.</returns>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline std::vector<T> read_ptr(address_t _Address, ptrchain_t _Offsets, int _Count) {
		__stack_record();

		__stack_rethrow(get_address(_Address, _Offsets, &_Address));

		std::vector<T> ret;
		__stack_rethrow(read(_Address, &ret, _Count));

		__stack_escape();
		return ret;
	}

	/// <summary>
	/// Reads an array of values from the end of a pointer chain and returns them in the form of an std::vector.
	/// </summary>
	/// <typeparam name="T">The datatype to read.</typeparam>
	/// <typeparam name="N">The number of values to read.</typeparam>
	/// <param name="_Address">- The pointer base address.</param>
	/// <param name="_Offsets">- The pointer chain offsets.</param>
	/// <returns>An std::array object containing the read values.</returns>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T, size_t N>
		requires(N != 0)
	inline std::array<T, N> read_ptr(address_t _Address, ptrchain_t _Offsets) {
		__stack_record();

		__stack_rethrow(get_address(_Address, _Offsets, &_Address));

		std::array<T, N> ret;
		__stack_rethrow(read(_Address, &ret));

		__stack_escape();
		return ret;
	}

#pragma endregion

#pragma region Overloads of write.

	/// <summary>
	/// Writes a value to an address.
	/// </summary>
	/// <typeparam name="T">The datatype to write.</typeparam>
	/// <param name="_Address">- The address to write to.</param>
	/// <param name="_Value">- The value to write.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline void write(address_t _Address, const T& _Value) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");

		__try {
			*_Address.ptr<T>() = _Value;
		}
		__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
			throw access_violation_exception(_Address, sizeof(T), memory_operation::write);
		}

		__stack_escape();
	}

	/// <summary>
	/// Writes a value to an address.
	/// </summary>
	/// <typeparam name="T">The datatype to write.</typeparam>
	/// <param name="_Address">- The address to write to.</param>
	/// <param name="_Value">- The value to write.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline void write(address_t _Address, T&& _Value) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");

		__try {
			*_Address.ptr<T>() = std::move(_Value);
		}
		__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
			throw access_violation_exception(_Address, sizeof(T), memory_operation::write);
		}

		__stack_escape();
	}

	/// <summary>
	/// Writes an array of values to an address.
	/// </summary>
	/// <typeparam name="T">The datatype to write.</typeparam>
	/// <param name="_Address">- The address to write to.</param>
	/// <param name="_Values">- A pointer to a buffer containing the values to write.</param>
	/// <param name="_Count">- The number of values to write.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline void write(address_t _Address, const T* const _Values, int _Count) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");
		if (!_Values)
			throw argument_exception("Pointer cannot be null", "_Return");
		if (_Count <= 0)
			throw argument_exception("Argument cannot be less than or equal to zero.", "_Count");

		__try {
			for (int i = 0; i < _Count; i++)
				_Address.ptr<T>()[i] = _Values[0];
		}
		__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
			throw access_violation_exception(_Address, sizeof(T) * _Count, memory_operation::write);
		}

		__stack_escape();
	}

	/// <summary>
	/// Writes an array of values to an address.
	/// </summary>
	/// <typeparam name="T">The datatype to write.</typeparam>
	/// <typeparam name="N">The number of values to write.</typeparam>
	/// <param name="_Address">- The address to write to.</param>
	/// <param name="_Values">- A reference to an array containing the values to write.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T, size_t N>
		requires(N != 0)
	inline void write(address_t _Address, const T(&_Values)[N]) {
		__stack_record();
		__stack_rethrow(write(_Address, _Values, N));
		__stack_escape();
	}

	/// <summary>
	/// Writes an array of values to an address.
	/// </summary>
	/// <typeparam name="T">The datatype to write.</typeparam>
	/// <param name="_Address">- The address to write to.</param>
	/// <param name="_Values">- An std::vector containing the values to write.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline void write(address_t _Address, const std::vector<T>& _Values) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");
		if (_Values.size() == 0)
			throw argument_exception("Collection cannot be empty.", "_Values");

		__try {
			for (int i = 0; i < _Values.size(); i++)
				_Address.ptr<T>()[i] = _Values[i];
		}
		__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
			throw access_violation_exception(_Address, sizeof(T) * _Values.size(), memory_operation::write);
		}

		__stack_escape();
	}

	/// <summary>
	/// Writes an array of values to an address.
	/// </summary>
	/// <typeparam name="T">The datatype to write.</typeparam>
	/// <param name="_Address">- The address to write to.</param>
	/// <param name="_Values">- An std::vector containing the values to write.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T>
	inline void write(address_t _Address, std::vector<T>&& _Values) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");
		if (_Values.size() == 0)
			throw argument_exception("Collection cannot be empty.", "_Values");

		__try {
			for (int i = 0; i < _Values.size(); i++)
				_Address.ptr<T>()[i] = std::move(_Values[i]);
		}
		__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
			throw access_violation_exception(_Address, sizeof(T) * _Values.size(), memory_operation::write);
		}

		__stack_escape();
	}

	/// <summary>
	/// Writes an array of values to an address.
	/// </summary>
	/// <typeparam name="T">The datatype to write.</typeparam>
	/// <typeparam name="N">The number of values to write.</typeparam>
	/// <param name="_Address">- The address to write to.</param>
	/// <param name="_Values">- An std::array contaning the values to write.</param>
	/// <exception cref="argument_exception"/>
	/// <exception cref="access_violation_exception"/>
	template<typename T, size_t N>
		requires(N != 0)
	inline void write(address_t _Address, const std::array<T, N>& _Values) {
		__stack_record();

		if (!_Address)
			throw argument_exception("Argument cannot be null.", "_Address");

		__try {
			for (int i = 0; i < _Values.size(); i++)
				_Address.ptr<T>()[i] = _Values[i];
		}
		__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
			throw access_violation_exception(_Address, sizeof(T) * N, memory_operation::write);
		}

		__stack_escape();
	}

#pragma endregion
}

#endif // !__ARTEMIS_API_MEMORY_HXX__