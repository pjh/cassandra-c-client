/* ccc_macros.h - Various macros used in Cassandra C Client code.
 *
 * Written in 2012 by Peter Hornyack <pjh@cs.washington.edu>
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to the public
 * domain worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

/* The "ccc" prefix is for "Cassandra C Client." */

#ifndef CCC_MACROS_H
#define CCC_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* CCC_ASSERT should always be defined for sanity-checking purposes, and
 * only un-defined when we want to disable sanity-checking code to get
 * maximum performance (it will probably make very little difference
 * though).
 * CCC_DEBUG enables verbose debug messages.
 * CCC_TESTCASE enables testcase checking.
 */
//#define CCC_ASSERT
//#define CCC_DEBUG
#define CCC_TESTCASE

/* Note that printing the result of pthread_self() in this way is not
 * portable. On my 32-bit machine, /usr/include/bits/pthreadtypes.h
 * contains: "typedef unsigned long int pthread_t;"; however, this may
 * cause formatting warnings on 64-bit machines :(
 */

/* Print normal and debug output to stdout, warning and error output to
 * stderr. Always flush after printing; this makes debugging etc. easier,
 * but possibly causes slowdown.
 */
#define ccc_print(f, a...)  do { \
	fprintf(stdout, "CCC: %lu: " f, pthread_self(), ##a); \
	fflush(stdout); \
} while(0)
#define ccc_warn(f, a...)  do { \
	fprintf(stderr, "**WARNING**: %lu: %s: " f, pthread_self(), __func__, ##a); \
	fflush(stderr); \
} while(0)
#define ccc_error(f, a...)  do { \
	fprintf(stderr, "ERROR: %lu: %s: " f, pthread_self(), __func__, ##a); \
	fflush(stderr); \
} while(0)
#define ccc_test(f, a...) fprintf(stdout, "TEST: %lu: " f, pthread_self(), ##a)

#ifdef CCC_DEBUG
#define ccc_debug(f, a...)  do { \
	fprintf(stdout, "DEBUG: %lu: %s: " f, pthread_self(), __func__, ##a); \
	fflush(stdout); \
} while(0)
#else
#define ccc_debug(f, a...)  do { ; } while(0)
#endif

/* die by abort()ing; run "ulimit -c unlimited" first if crash dumps are
 * desired.
 */
#define ccc_die(f, a...)  do { \
	fprintf(stderr, "CCC: Fatal error (%lu: %s): " f, pthread_self(), __func__, ##a); \
	fflush(stderr); \
	abort(); \
} while(0)

#ifdef CCC_TESTCASE
#define ccc_testcase_int(tid, description, expected, actual) do { \
	fprintf(stdout, "TEST %s: %lu: %s: expected=%d, actual=%d\n", \
	    (expected == actual) ? "PASS" : "FAIL", \
	    tid, description, expected, actual); \
} while(0)
#else
#define ccc_testcase_int(tid, description, expected, actual) \
	do { ; } while(0)
#endif

#ifdef CCC_TESTCASE
#define ccc_testcase_uint64(tid, description, expected, actual) do { \
	fprintf(stdout, "TEST %s: %lu: %s: expected=%llu, actual=%llu\n", \
	    (expected == actual) ? "PASS" : "FAIL", \
	    tid, description, expected, actual); \
} while(0)
#else
#define ccc_testcase_uint64(tid, description, expected, actual) \
	do { ; } while(0)
#endif

#ifdef CCC_TESTCASE
#define ccc_testcase_uint64_not(tid, description, not_expected, actual) do { \
	fprintf(stdout, "TEST %s: %lu: %s: not_expected != %llu, actual=%llu\n", \
	    (not_expected != actual) ? "PASS" : "FAIL", \
	    tid, description, not_expected, actual); \
} while(0)
#else
#define ccc_testcase_uint64_not(tid, description, not_expected, actual) \
	do { ; } while(0)
#endif

#ifdef CCC_TESTCASE
#define ccc_testcase_string(tid, description, expected, actual) do { \
	fprintf(stdout, "TEST %s: %lu: %s: expected=%s, actual=%s\n", \
	    (expected == NULL) ? "FAIL" : \
	    (actual == NULL) ? "FAIL" : \
	    (strcmp(expected, actual) == 0) ? "PASS" : "FAIL", \
	    tid, description, expected, actual); \
} while(0)
#else
#define ccc_testcase_string(tid, description, expected, actual) \
	do { ; } while(0)
#endif

#endif  //CCC_MACROS_H

