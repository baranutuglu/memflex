#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(cond, msg)                                                          \
    do                                                                             \
    {                                                                              \
        if (!(cond))                                                               \
        {                                                                          \
            fprintf(stderr, "[FAIL] %s: %s (Line %d)\n", __func__, msg, __LINE__); \
            exit(1);                                                               \
        }                                                                          \
        else                                                                       \
        {                                                                          \
            printf("[PASS] %s: %s\n", __func__, msg);                              \
        }                                                                          \
    } while (0)

#define ASSERT_NOT_NULL(ptr, msg) ASSERT((ptr) != NULL, msg)
#define ASSERT_NULL(ptr, msg) ASSERT((ptr) == NULL, msg)
#define ASSERT_EQ(a, b, msg) ASSERT((a) == (b), msg)

#endif
