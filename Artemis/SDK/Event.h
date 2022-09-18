#ifndef __EVENT_H__
#define __EVENT_H__

#ifdef __cplusplus

template<typename FN>
class DelegateClass {};

template<typename R, typename... A>
class DelegateClass<R(*)(A...)> {};

#define delegate(x) typedef DelegateClass<x>

template<typename FN>
class event {};

template<typename R, typename... A>
class event<DelegateClass<R(*)(A...)>> {
	using FunctionType = R(*)(A...);
	FunctionType szFunctions[64];
public:
	event() { memset(szFunctions, 0x00, sizeof(szFunctions)); }

	void Invoke(A... args) {
		for (int i = 0; i < 64; i++) {
			if (szFunctions[i]) szFunctions[i](args...);
		}
	}

	void operator+=(FunctionType fn) {
		for (int i = 0; i < 64; i++) {
			if (!szFunctions[i]) {
				szFunctions[i] = fn;
				break;
			}
		}
	}

	void operator-=(FunctionType fn) {
		for (int i = 0; i < 64; i++) {
			if (szFunctions[i] == fn) szFunctions[i] = nullptr;
		}
	}
};

#endif // __cplusplus

#endif // !__EVENT_H__