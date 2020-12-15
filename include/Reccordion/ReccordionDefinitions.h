/// --------------------------------------------------------------------------------------------------------
/// MIT License
/// --------------------------------------------------------------------------------------------------------
/// Copyright (c) 2020 Omar Sherif Fathy
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
/// --------------------------------------------------------------------------------------------------------

#pragma once

#ifdef NDEBUG
#define RECCORDION_TARGET_NAME "RELEASE"
#define RECCORDION_TARGET_RELEASE
#else
#define RECCORDION_TARGET_NAME "DEBUG"
#define RECCORDION_TARGET_DEBUG
#endif

#if defined(__EMSCRIPTEN__)
#define RECCORDION_PLATFORM_NAME "WEBKIT"
#define RECCORDION_PLATFORM_WEBKIT
#define RECCORDION_FORCE_INLINE inline
#define RECCORDION_FORCE_NO_INLINE
#define RECCORDION_DYNAMIC_FN_CALL
#elif defined(_WIN32) || defined(_WIN64)
#define RECCORDION_PLATFORM_NAME "WINDOWS"
#define RECCORDION_PLATFORM_WINDOWS
#define RECCORDION_FORCE_INLINE    __forceinline
#define RECCORDION_FORCE_NO_INLINE _declspec(noinline)
#define RECCORDION_DYNAMIC_FN_CALL __stdcall
#elif defined(__linux__)
#define RECCORDION_PLATFORM_NAME "LINUX"
#define RECCORDION_PLATFORM_LINUX
#define RECCORDION_FORCE_INLINE inline
#define RECCORDION_FORCE_NO_INLINE
#define RECCORDION_DYNAMIC_FN_CALL
#elif defined(__APPLE__)
#define RECCORDION_PLATFORM_NAME "DARWIN"
#define RECCORDION_PLATFORM_DARWIN
#define RECCORDION_FORCE_INLINE inline
#define RECCORDION_FORCE_NO_INLINE
#define RECCORDION_DYNAMIC_FN_CALL
#else
#error "Platform not supported."
#endif

#if defined(RECCORDION_PLATFORM_WEBKIT) && defined(RECCORDION_EXPORT_DYNAMIC)
#define RECCORDION_API __attribute__((visibility("default")))
#elif defined(RECCORDION_PLATFORM_WINDOWS) && defined(RECCORDION_EXPORT_DYNAMIC)
#define RECCORDION_API __declspec(dllexport)
#elif defined(RECCORDION_PLATFORM_WINDOWS) && defined(RECCORDION_IMPORT_DYNAMIC)
#define RECCORDION_API __declspec(dllimport)
#elif defined(RECCORDION_PLATFORM_DARWIN) && defined(RECCORDION_EXPORT_DYNAMIC)
#define RECCORDION_API __attribute__((visibility("default")))
#elif defined(RECCORDION_PLATFORM_LINUX) && defined(RECCORDION_EXPORT_DYNAMIC)
#define RECCORDION_API __attribute__((visibility("default")))
#else
#define RECCORDION_API
#endif

#if defined(__cplusplus)
#define RECCORDION_EXTERN extern "C"
#else
#define RECCORDION_EXTERN extern
#endif