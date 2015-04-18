#pragma once

// C++ Standard Library:
#include <cstdint>

namespace ed {
	template<typename R, typename... A>
	void* toObjectPointer(R (*functionPointer)(A...)) {
		static_assert(sizeof(void*) == sizeof(void (*)()),
			"sizeof(void*) != sizeof(void (*)())");
		return reinterpret_cast<void*>(
			reinterpret_cast<std::uintptr_t>(functionPointer));
	}
	
	template<typename R, typename... A>
	R (*toFunctionPointer(void* objectPointer))(A...) {
		static_assert(sizeof(void*) == sizeof(void (*)()),
			"sizeof(void*) != sizeof(void (*)())");
		return reinterpret_cast<R (*)(A...)>(
			reinterpret_cast<std::uintptr_t>(objectPointer));
	}
}