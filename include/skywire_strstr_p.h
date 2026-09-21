#pragma once

#ifndef SKYWIRE_STRSTR_P_H
#define SKYWIRE_STRSTR_P_H

#include "Arduino.h"

inline char *skywireStrstrP(char *haystack, const char *needle_progmem)
{
    return strstr_P(haystack, needle_progmem);
}

inline bool skywireContainsP(char *haystack, const char *needle_progmem)
{
    return skywireStrstrP(haystack, needle_progmem) != nullptr;
}

#endif
