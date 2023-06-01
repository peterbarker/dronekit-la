#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#if defined(__GNUC__) && __GNUC__ >= 9
#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
#endif
#include "mavlink_c_library/ardupilotmega/mavlink.h"
#include "mavlink_c_library/common/mavlink.h"
#pragma GCC diagnostic pop
