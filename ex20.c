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

#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None": strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d: errno %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define check(A, M, ...) if (!(A)) { log_err(M, ##__VA_ARGS__); errno = 0; goto error; }

#define sentinel(M, ...) { log_err(M, ##_VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if (!(A)) { debug(M, ##_VA_ARGS__); errno = 0; goto error; }

#endif
