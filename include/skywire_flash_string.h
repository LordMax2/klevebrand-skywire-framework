#pragma once

#ifndef SKYWIRE_FLASH_STRING_H
#define SKYWIRE_FLASH_STRING_H

#include "Arduino.h"
#include <avr/pgmspace.h>

inline char *skywireFindFlashString(char *haystack, const char *flash_needle)
{
    return strstr_P(haystack, flash_needle);
}

inline bool skywireContainsFlashString(char *haystack, const char *flash_needle)
{
    return skywireFindFlashString(haystack, flash_needle) != nullptr;
}

#endif
