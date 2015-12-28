/*C tackles error handling by returning error codes and setting a global errno value that you can check. THis
 * makes for complex code that exists to check if something you did has an error. You can't solve this problem
 * with exceptions because you only have one return value and trying to marshal exceptions up the stack in C
 * is difficult as no other libraries will understand it.
 *
 * The Debug Macros
 * - Whenever there is an error, the function will jump to that "error": part of the function that knows how
 *   to cleanup everything and return an error code. You use a macro called check to check return codes, print
 *   an error message, and then jump to the cleanup section.
 */

// Usual defense against accidentally including the file twice
#ifndef __dbg_h__
#define __dbg_h__

// Includes for the functions that these macros need
#include <stdio.h>
#include <errno.h>
#include <string.h>

// Lets you recompile your program so that all debug log messages are removed
#ifdef NDEBUG
#define debug(M, ...) // if NDEBUG == "No Debug" then no debug messages will remain
#else
// debug("format", arg1, arg2, ...) --> into fprintf call to stderr, ##__VAR_ARGS__ --> "put whatever they had
// for extra arguments (...) here", __FILE__ and __LINE__ get you the current file:line for the debug message
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

// Used to get a safe readable version of errno
#define clean_errno() (errno == 0 ? "None": strerror(errno))

// Macros for logging messages meant for the end user. Works like debug but can't be compiled out

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d: errno %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

// Will make sure condition A is true, and if not logs the error M (with variable arguments for log_err), then
// jumps to the function's error: for cleanup
#define check(A, M, ...) if (!(A)) { log_err(M, ##__VA_ARGS__); errno = 0; goto error; }

// Placed in any part of a function that shouldn't run, and if it does prints an error message then jumps to
// the error: label. You put this in if-statements and switch-statements to catch conditions that shouldn't
// happen
#define sentinel(M, ...) { log_err(M, ##_VA_ARGS__); errno=0; goto error; }

// Checks if a pointer is valid, and if it isn't reports an error as "Out of memory"
#define check_mem(A) check((A), "Out of memory.")

// If the error is common then you don't want to bother reporting it. It will use debug instead of log_err to
// report the message. When you define NDEBUG, the check still happens, the error jump goes off, but the
// message isn't printed
#define check_debug(A, M, ...) if (!(A)) { debug(M, ##_VA_ARGS__); errno = 0; goto error; }

#endif
