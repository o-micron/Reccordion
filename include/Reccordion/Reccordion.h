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

#include "ReccordionDefinitions.h"

#include <stdint.h>

/// --------------------------------------------------------------------------------------------------------
/// EXPORTS
/// --------------------------------------------------------------------------------------------------------

/// Initializes some globals and allocates memory ahead of time
RECCORDION_EXTERN RECCORDION_API bool
ReccordionInitialize();

/// DeInitializes globals and deallocates any allocated memory
RECCORDION_EXTERN RECCORDION_API void
ReccordionDeinitialize();

/// Redirects Cin, Cout, Cerr
RECCORDION_EXTERN RECCORDION_API void
ReccordionEnableLogRedirection();

/// Return Cin, Cout, Cerr to default
RECCORDION_EXTERN RECCORDION_API void
ReccordionDisableLogRedirection();

/// Fetch screen width
RECCORDION_EXTERN RECCORDION_API int32_t
ReccordionGetScreenWidth();

/// Fetch screen height
RECCORDION_EXTERN RECCORDION_API int32_t
ReccordionGetScreenHeight();

/// Starts recording a video of the desktop asynchronously
RECCORDION_EXTERN RECCORDION_API void
ReccordionStartRecording(const char* directory,
                         const char* name,
                         int32_t     x      = 0,
                         int32_t     y      = 0,
                         int32_t     width  = 0,
                         int32_t     height = 0);

/// Stops recording the running video
RECCORDION_EXTERN RECCORDION_API void
ReccordionStopRecording();

/// --------------------------------------------------------------------------------------------------------