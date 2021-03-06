/*
 * Copyright 2017 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <stdint.h>
#include <time.h>

#include <folly/portability/Config.h>
#include <folly/Portability.h>

// OSX is a pain. The XCode 8 SDK always declares clock_gettime
// even if the target OS version doesn't support it, so you get
// an error at runtime because it can't resolve the symbol. We
// solve that by pretending we have it here in the header and
// then enable our implementation on the source side so that
// gets linked in instead.
#if __MACH__ && FOLLY_HAVE_CLOCK_GETTIME &&                    \
    (MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_12 || \
     __IPHONE_OS_VERSION_MIN_REQUIRED < __IPHONE_10_0)

#define FOLLY_FORCE_CLOCK_GETTIME_DEFINITION 1

#endif

// These aren't generic implementations, so we can only declare them on
// platforms we support.
#if !FOLLY_HAVE_CLOCK_GETTIME && (defined(__MACH__) || defined(_WIN32))
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1
#define CLOCK_PROCESS_CPUTIME_ID 2
#define CLOCK_THREAD_CPUTIME_ID 3
#endif

#if (!FOLLY_HAVE_CLOCK_GETTIME || FOLLY_FORCE_CLOCK_GETTIME_DEFINITION) && (defined(__MACH__) || defined(_WIN32))
namespace folly {
namespace portability {
namespace time {
typedef uint8_t clockid_t;
int clock_gettime(clockid_t clk_id, struct timespec* ts);
int clock_getres(clockid_t clk_id, struct timespec* ts);
}
}
}

FOLLY_PUSH_WARNING
#if __CLANG_PREREQ(3, 0)
FOLLY_GCC_DISABLE_WARNING("-Wheader-hygiene")
#endif
/* using override */ using namespace folly::portability::time;
FOLLY_POP_WARNING
#endif

#ifdef _WIN32
#define TM_YEAR_BASE (1900)

extern "C" {
char* asctime_r(const tm* tm, char* buf);
char* ctime_r(const time_t* t, char* buf);
tm* gmtime_r(const time_t* t, tm* res);
tm* localtime_r(const time_t* t, tm* o);
int nanosleep(const struct timespec* request, struct timespec* remain);
char* strptime(
    const char* __restrict buf,
    const char* __restrict fmt,
    struct tm* __restrict tm);
}
#endif
