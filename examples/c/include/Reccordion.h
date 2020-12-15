#pragma once

#include <stdint.h>

extern int
ReccordionInitialize();

extern void
ReccordionDeinitialize();

extern void
ReccordionEnableLogRedirection();

extern void
ReccordionDisableLogRedirection();

extern int32_t
ReccordionGetScreenWidth();

extern int32_t
ReccordionGetScreenHeight();

extern void
ReccordionStartRecording(const char* directory, const char* name, int32_t x, int32_t y, int32_t width, int32_t height);

extern void
ReccordionStopRecording();