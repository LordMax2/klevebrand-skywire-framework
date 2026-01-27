#ifndef SKYWIRE_RESPONSE_RESULT_T
#define SKYWIRE_RESPONSE_RESULT_T

#include "Arduino.h"

struct SkywireResponseResult_t
{
    SkywireResponseResult_t(bool is_success, String response_content)
    {
        this->is_success = is_success;
        this->response_content = response_content;
    }
    bool is_success;
    String response_content;
};

#endif // SKYWIRE_RESPONSE_RESULT