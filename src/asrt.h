/**
 * asrt.h
 * Created By Haidir
 */
#pragma once
#include <assert.h>
#include <stdio.h>

#ifdef __WIN32__
#define ASSERT(x) { \
                    printf(x); \
                    assert(0); \
                    }
#else
#define ASSERT(x) __assert_fail(x, __FILE__,  __LINE__, __ASSERT_FUNCTION);
#endif

#ifdef __WIN32__
#define ASSERT_INSTRUCTION(x, instruction, line)    { \
printf("%s:%i %s\n", instruction, line, x); \
assert(0); \
}
#else
#define ASSERT_INSTRUCTION(x, instruction, line ) __assert_fail(x, instruction, line, __ASSERT_FUNCTION)
#endif
