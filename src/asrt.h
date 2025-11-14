/**
 * asrt.h
 * Created By Haidir
 */
#pragma once
#include <assert.h>

#define ASSERT(x) __assert_fail(x, __FILE__, __LINE__, __ASSERT_FUNCTION)