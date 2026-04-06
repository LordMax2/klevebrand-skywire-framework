#include "skywire-response-result.h"

SkywireResponseResult_t::SkywireResponseResult_t(bool is_success, const char *response_content)
    : is_success(is_success), response_content(response_content)
{
}
