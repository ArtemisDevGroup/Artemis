#ifndef __ARTEMIS_API_MEMORY_HXX__
#define __ARTEMIS_API_MEMORY_HXX__

#include <type_traits>	// std::is_pointer_v, std::is_integral_v
#include <vector>		// std::vector

#include "Definitions.hxx"

#include "Error.hxx"

namespace Artemis::API {
	class address_t {
	public:
		using value_type = unsigned long long;

	private:
		value_type _Value;

	public:
		constexpr address_t(value_type _Value) : _Value(_Value) {}

		template<typename T>
			requires(std::is_pointer_v<T>)
		inline address_t(T _Value) : _Value(reinterpret_cast<value_type>(_Value)) {}

		constexpr operator value_type () const noexcept { return this->_Value; }

		template<typename T>
			requires(std::is_pointer_v<T>)
		inline operator T () const noexcept { return reinterpret_cast<T>(this->_Value); }

		template<typename T>
			requires(std::is_integral_v<T>)
		constexpr address_t operator+(T _Right) const noexcept { return address_t(this->_Value + _Right); }

		template<typename T>
			requires(std::is_integral_v<T>)
		constexpr address_t& operator+=(T _Right) noexcept {
			this->_Value += _Right;
			return *this;
		}

		template<typename T>
			requires(std::is_integral_v<T>)
		constexpr address_t operator-(T _Right) const noexcept { return address_t(this->_Value - _Right); }

		template<typename T>
			requires(std::is_integral_v<T>)
		constexpr address_t& operator-=(T _Right) noexcept {
			this->_Value -= _Right;
			return *this;
		}
	};

	using ptroffset_t = unsigned short;
	using ptrchain_t = std::vector<ptroffset_t>;

	template<typename T>
	inline void read(address_t _Address, T* _Return) {
		__stack_record();

		__try {
			
		}
		__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER | EXCEPTION_CONTINUE_SEARCH) {
			
		}

		__stack_escape();
	}

	template<typename T>
	inline T read(address_t _Address);

	template<typename T>
	inline void read_ptr(address_t _Address, ptrchain_t _Offsets, T* _Return);

	template<typename T>
	inline T read_ptr(address_t _Address, ptrchain_t _Offsets);
}

#endif // !__ARTEMIS_API_MEMORY_HXX__