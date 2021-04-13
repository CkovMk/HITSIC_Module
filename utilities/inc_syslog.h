/**
 * Copyright 2018 - 2021 HITSIC
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifdef UTILITIES_INC_SYSLOG_H

#undef UTILITIES_INC_SYSLOG_H

#undef SYSLOG_A(...)
#undef SYSLOG_E(...)
#undef SYSLOG_W(...)
#undef SYSLOG_I(...)
#undef SYSLOG_D(...)
#undef SYSLOG_V(...)

#endif // ! UTILITIES_INC_SYSLOG_H

#ifndef UTILITIES_INC_SYSLOG_H
#define UTILITIES_INC_SYSLOG_H

#include <inc_syslog_port.h>
//#define SYSLOG_TAG ("DUMMY")
//#define SYSLOG_LVL (2U)

#ifndef SYSLOG_TAG
#error "SYSLOG_TAG Undefined!"
#endif // ! SYSLOG_TAG

#ifndef SYSLOG_LVL
#error "SYSLOG_LVL Undefined!"
#endif // ! SYSLOG_LVL

#define SYSLOG_LVL_A    (0U)
#define SYSLOG_LVL_E    (1U)
#define SYSLOG_LVL_W    (2U)
#define SYSLOG_LVL_I    (3U)
#define SYSLOG_LVL_D    (4U)
#define SYSLOG_LVL_V    (5U)

#if defined(SYSLOG_DEBUG)
#define SYSLOG_A(...)         (HITSIC_LOG_PRINTF("[A]") + HITSIC_LOG_PRINTF(SYSLOG_TAG) + HITSIC_LOG_PRINTF(": ") + HITSIC_LOG_PRINTF(__VA_ARGS__) + HITSIC_LOG_PRINTF(HITSIC_LOG_LINEFEED))
#endif

#if defined(SYSLOG_DEBUG) && (SYSLOG_LVL >= SYSLOG_LVL_E)
#define SYSLOG_E(...)         (HITSIC_LOG_PRINTF("[E]") + HITSIC_LOG_PRINTF(SYSLOG_TAG) + HITSIC_LOG_PRINTF(": ") + HITSIC_LOG_PRINTF(__VA_ARGS__) + HITSIC_LOG_PRINTF(HITSIC_LOG_LINEFEED))
#else
#define SYSLOG_E(...)
#endif // SYSLOG_LVL_E

#if defined(SYSLOG_DEBUG) && (SYSLOG_LVL >= SYSLOG_LVL_W)
#define SYSLOG_W(...)         (HITSIC_LOG_PRINTF("[W]") + HITSIC_LOG_PRINTF(SYSLOG_TAG) + HITSIC_LOG_PRINTF(": ") + HITSIC_LOG_PRINTF(__VA_ARGS__) + HITSIC_LOG_PRINTF(HITSIC_LOG_LINEFEED))
#else
#define SYSLOG_W(...)
#endif // SYSLOG_LVL_E

#if defined(SYSLOG_DEBUG) && (SYSLOG_LVL >= SYSLOG_LVL_I)
#define SYSLOG_I(...)         (HITSIC_LOG_PRINTF("[I]") + HITSIC_LOG_PRINTF(SYSLOG_TAG) + HITSIC_LOG_PRINTF(": ") + HITSIC_LOG_PRINTF(__VA_ARGS__) + HITSIC_LOG_PRINTF(HITSIC_LOG_LINEFEED))
#else
#define SYSLOG_I(...)
#endif // SYSLOG_LVL_E

#if defined(SYSLOG_DEBUG) && (SYSLOG_LVL >= SYSLOG_LVL_D)
#define SYSLOG_D(...)         (HITSIC_LOG_PRINTF("[D]") + HITSIC_LOG_PRINTF(SYSLOG_TAG) + HITSIC_LOG_PRINTF(": ") + HITSIC_LOG_PRINTF(__VA_ARGS__) + HITSIC_LOG_PRINTF(HITSIC_LOG_LINEFEED))
#else
#define SYSLOG_D(...)
#endif // SYSLOG_LVL_E

#if defined(SYSLOG_DEBUG) && (SYSLOG_LVL >= SYSLOG_LVL_V)
#define SYSLOG_V(...)         (HITSIC_LOG_PRINTF("[V]") + HITSIC_LOG_PRINTF(SYSLOG_TAG) + HITSIC_LOG_PRINTF(": ") + HITSIC_LOG_PRINTF(__VA_ARGS__) + HITSIC_LOG_PRINTF(HITSIC_LOG_LINEFEED))
#else
#define SYSLOG_V(...)
#endif // SYSLOG_LVL_E

#undef SYSLOG_LVL_A
#undef SYSLOG_LVL_E
#undef SYSLOG_LVL_W
#undef SYSLOG_LVL_I
#undef SYSLOG_LVL_D
#undef SYSLOG_LVL_V



#endif // ! UTILITIES_INC_SYSLOG_H
