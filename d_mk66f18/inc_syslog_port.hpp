#ifndef D_MK66F18_INC_SYSLOG_PORT_HPP
#define D_MK66F18_INC_SYSLOG_PORT_HPP

#include "hitsic_common.h"
#include "cstdio"

#ifdef DEBUG
#define SYSLOG_DEBUG
#endif

#define HITSIC_LOG_PRINTF(...)  (printf(__VA_ARGS__))

#endif // ! D_MK66F18_INC_SYSLOG_PORT_HPP
