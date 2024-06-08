//
//	CommonDefines.h
//	DLSS-Raytracing
//
//	Created by Diego Revilla on 8/06/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _COMMONDEFINES__H_
#define _COMMONDEFINES__H_

#include <memory>
#include <exception>

#define DONTDISCARD	[[nodiscard]]
#define CLASS_EXCEPTION(name) struct name ## Exception : public std::exception { name ## Exception(const char* what) : std::exception(what) {}};

constexpr float PI = 3.14159265358979323846f;

namespace std {
	template<typename T>
	using deleted_pointer = std::unique_ptr<T, void(*)(T*)>;
}

#endif