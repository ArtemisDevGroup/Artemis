#include "pch.h"
#include "Memory.hxx"

namespace Artemis::API {
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

		return std::format("Access violation occured at {:X} (size {:X}) while {} memory region.", _Address.value(), _Size, operation);
	}

	access_violation_exception::access_violation_exception(address_t _Address, size_t _Size, memory_operation _Operation) : system_exception(format_message(_Address, _Size, _Operation)), _Address(_Address), _Size(_Size), _Operation(_Operation) {
		VirtualQuery(_Address, &this->_MBI, sizeof(this->_MBI));
	}

	address_t access_violation_exception::address() const noexcept { return this->_Address; }
	size_t access_violation_exception::size() const noexcept { return this->_Size; }
	memory_operation access_violation_exception::operation() const noexcept { return this->_Operation; }
	const MEMORY_BASIC_INFORMATION* access_violation_exception::mbi() const noexcept { return &this->_MBI; }
}