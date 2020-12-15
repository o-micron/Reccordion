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

/// --------------------------------------------------------------------------------------------------------
/// SECTIONS OF THIS FILE
/// --------------------------------------------------------------------------------------------------------
/// - RECCORDION C EXAMPLE
/// --------------------------------------------------------------------------------------------------------

#include "../include/Reccordion.h"

#include <stdio.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if defined(RECCORDION_C_VIDEO_DIR)
#define OUTPUT_DIR RECCORDION_C_VIDEO_DIR
#else
#define OUTPUT_DIR ""
#endif

int
main(int argc, char const* argv[])
{
    char equal[60];
    memset(equal, '=', sizeof(equal));
    printf("\n\n%s\nSTARTING C VIDEO RECORDING EXAMPLE\n%s\n\n", equal, equal);
    if (ReccordionInitialize()) {
        ReccordionEnableLogRedirection();
        ReccordionStartRecording(OUTPUT_DIR, "c_video", 0, 0, ReccordionGetScreenWidth(), ReccordionGetScreenHeight());
        printf("Recording ..");
        Sleep(10000);
        printf(" done\n");
        ReccordionStopRecording();
        ReccordionDeinitialize();
    }
    printf("\n\n%s\nENDING C VIDEO RECORDING EXAMPLE.\n%s\n", equal, equal);
    return 0;
}
