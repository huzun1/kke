#pragma once

#include <cstdio>
#include <utility>

namespace kke::debug {
inline void log(char const* message) {
#ifdef KKE_DEBUG
	std::fputs(message, stderr);
	std::fputc('\n', stderr);
#else
	(void)message;
#endif
}

template <typename... Args>
void log(char const* format, Args&&... args) {
#ifdef KKE_DEBUG
	std::fprintf(stderr, format, std::forward<Args>(args)...);
	std::fputc('\n', stderr);
#else
	(void)format;
	((void)args, ...);
#endif
}
}
