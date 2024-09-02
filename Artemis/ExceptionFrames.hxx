#ifndef __ARTEMIS_EXCEPTION_FRAMES_HXX__
#define __ARTEMIS_EXCEPTION_FRAMES_HXX__

#include "Definitions.hxx"
ARTEMIS_INTERNAL_HEADER

#include "API/Error.hxx"
#include "API/Logging.hxx"

#include <functional>	// std::function

namespace Artemis::_ {
	class __safe_exception_propagator {
	public:
		__safe_exception_propagator() = default;

		void exec_l1cxx(const std::function<void()>& _Fn) const;
		void exec_l1seh(const std::function<void()>& _Fn, bool _LetThroughCxxExceptions = false) const;

		void exec_l2(const std::function<void()>& _Fn) const;
	};

	class __safe_exception_net : public API::loggable {
	public:
		__safe_exception_net() = default;

		void exec_l1cxx(const std::function<void()>& _Fn) const noexcept;
		void exec_l1seh(const std::function<void()>& _Fn) const noexcept;

		void exec_l2(const std::function<void()>& _Fn) const noexcept;
	};
}

#endif // !__ARTEMIS_EXCEPTION_FRAMES_HXX__