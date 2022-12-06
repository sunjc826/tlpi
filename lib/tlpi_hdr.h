// Common header file

#if !defined(TLPI_HDR_H)
#define TLPI_HDR_H
// Type definitions
#include <cstddef>
#include <cstdint>
#include <sys/types.h>

// Standard I/O
#include <cstdio>
#include <iostream>

// Common library functions
#include <cstdlib>

// System call wrappers
#include <unistd.h>

// Errors
#include <cerrno>

// Strings
#include <cstring>
#include <string>

// For min, max, and other useful stuff
#include <algorithm>

#include "error_functions.h"
#include "get_num.h"
#include "ugid_functions.h"
#endif
