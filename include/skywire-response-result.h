#ifndef SKYWIRE_RESPONSE_RESULT_T
#define SKYWIRE_RESPONSE_RESULT_T

#include "Arduino.h"

struct SkywireResponseResult_t
{
    SkywireResponseResult_t(bool is_success, const char *response_content);
    bool is_success;
    const char *response_content;
};

#endif
