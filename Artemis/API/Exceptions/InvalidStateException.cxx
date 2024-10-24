#include "pch.h"
#include "ExceptionBase.hxx"
#include "InvalidStateException.hxx"

namespace Artemis::API {
	invalid_state_exception::invalid_state_exception() noexcept : exception("Object has an invalid state to preform this operation.") {}

	invalid_state_exception::invalid_state_exception(std::string_view&& _Message) noexcept : exception(std::move(_Message)) {}

	void invalid_state_exception::throw_on_condition_not_met(bool _Condition, std::string_view&& _ConditionString) {
		if (_Condition)
			throw invalid_state_exception(std::format("The state of the object is invalid. Condition: {}", _ConditionString));
	}
}