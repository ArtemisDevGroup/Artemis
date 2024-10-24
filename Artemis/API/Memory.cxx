#include "pch.h"
#include "Memory.hxx"

namespace Artemis::API {
#pragma region Implementation of access_violation_exception class.

	std::string access_violation_exception::format_message(address_t _Address, size_t _Size, memory_operation _Operation) {
		std::string operation;
		switch (_Operation) {
		case memory_operation::read:
			operation = "reading from";
			break;
		case memory_operation::write:
			operation = "writing to";
			break;
		case memory_operation::execute:
			operation = "executing";
			break;
		default:
			operation = "performing unknown operation on";
			break;
		}

		return std::format("Access violation occured at {:X} (size {:X} while {} memory region.", _Address.value(), _Size, operation);
	}

	access_violation_exception::access_violation_exception(address_t _Address, size_t _Size, memory_operation _Operation) : system_exception(format_message(_Address, _Size, _Operation)), _Address(_Address), _Size(_Size), _Operation(_Operation) {
		VirtualQuery(_Address, &this->_MBI, sizeof(this->_MBI));
	}

	address_t access_violation_exception::address() const noexcept { return this->_Address; }
	size_t access_violation_exception::size() const noexcept { return this->_Size; }
	memory_operation access_violation_exception::operation() const noexcept { return this->_Operation; }
	const MEMORY_BASIC_INFORMATION* access_violation_exception::mbi() const noexcept { return &this->_MBI; }

#pragma endregion

#pragma region Implementations of get_address.
	
	void get_address(address_t _Address, ptrchain_t _Offsets, address_t* _Return) {
		__stack_record();

		argument_exception::throw_if_null(AE_ARGUMENT(_Address));
		argument_exception::throw_if_null(AE_ARGUMENT(_Offsets.size()));
		argument_exception::throw_if_null(AE_ARGUMENT(_Return));

		__stack_rethrow_try();
		for (ptroffset_t o : _Offsets) {
			read(_Address, _Address.buffer());
			_Address += o;
		}
		__stack_rethrow_catch();

		*_Return = _Address;

		__stack_escape();
	}

	address_t get_address(address_t _Address, ptrchain_t _Offsets) {
		__stack_record();
		__stack_rethrow(get_address(_Address, _Offsets, &_Address));
		__stack_escape();
		return _Address;
	}

#pragma endregion
}