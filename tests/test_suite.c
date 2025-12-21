#include <stdio.h>
#include <string.h>
#include "../src/memory.h"
#include "test_utils.h"

#define HEAP_SIZE 1024 * 1024 // 1MB for testing

void test_malloc()
{
    printf("\n--- Testing my_malloc ---\n");
    void *p1 = my_malloc(100, ALGO_FIRST_FIT);
    ASSERT_NOT_NULL(p1, "Allocation of 100 bytes should succeed");

    void *p2 = my_malloc(200, ALGO_FIRST_FIT);
    ASSERT_NOT_NULL(p2, "Allocation of 200 bytes should succeed");

    ASSERT(p1 != p2, "Pointers should be distinct");

    my_free(p1);
    my_free(p2);
}

void test_free()
{
    printf("\n--- Testing my_free ---\n");
    void *p1 = my_malloc(100, ALGO_FIRST_FIT);
    ASSERT_NOT_NULL(p1, "Allocation should succeed");

    my_free(p1);

    // Allocate again, should likely get the same address or at least succeed
    void *p2 = my_malloc(100, ALGO_FIRST_FIT);
    ASSERT_NOT_NULL(p2, "Re-allocation after free should succeed");

    // In a simple First-Fit implementation, p2 often equals p1 if it's the first hole
    // But we won't strictly assert equality as implementation details might vary

    my_free(p2);
}

void test_calloc()
{
    printf("\n--- Testing my_calloc ---\n");
    size_t num = 5;
    size_t size = sizeof(int);
    int *arr = (int *)my_calloc(num, size, ALGO_FIRST_FIT);

    ASSERT_NOT_NULL(arr, "Calloc should succeed");

    int is_zero = 1;
    for (size_t i = 0; i < num; i++)
    {
        if (arr[i] != 0)
        {
            is_zero = 0;
            break;
        }
    }
    ASSERT(is_zero, "Memory should be zero-initialized");

    my_free(arr);
}

void test_realloc()
{
    printf("\n--- Testing my_realloc ---\n");

    // 1. Test NULL ptr (should act like malloc)
    void *p1 = my_realloc(NULL, 100);
    ASSERT_NOT_NULL(p1, "realloc(NULL, size) should act like malloc");

    // Fill with data
    memset(p1, 0xAA, 100);

    // 2. Test Shrink
    void *p2 = my_realloc(p1, 50);
    ASSERT_NOT_NULL(p2, "Shrinking should succeed");
    // Verify data is preserved (first 50 bytes)
    unsigned char *bytes = (unsigned char *)p2;
    int preserved = 1;
    for (int i = 0; i < 50; i++)
    {
        if (bytes[i] != 0xAA)
            preserved = 0;
    }
    ASSERT(preserved, "Data should be preserved after shrink");

    // 3. Test Grow
    void *p3 = my_realloc(p2, 200);
    ASSERT_NOT_NULL(p3, "Growing should succeed");

    // Verify data is preserved (first 50 bytes)
    bytes = (unsigned char *)p3;
    preserved = 1;
    for (int i = 0; i < 50; i++)
    {
        if (bytes[i] != 0xAA)
            preserved = 0;
    }
    ASSERT(preserved, "Data should be preserved after grow");

    // 4. Test size 0 (should act like free)
    void *p4 = my_realloc(p3, 0);
    ASSERT_NULL(p4, "realloc(ptr, 0) should return NULL (freed)");
}

int main()
{
    printf("Initializing Test Suite...\n");

    // Initialize memory manager manually for testing
    // We use sbrk in the implementation, so we just need to ensure it's reset or init
    my_memory_reset();

    // Note: my_memory_init is usually called to set up the initial heap.
    // Since our implementation uses sbrk on demand or via my_memory_init,
    // let's try to initialize a clean state.
    // However, my_malloc calls sbrk if needed.

    test_malloc();
    test_free();
    test_calloc();
    test_realloc();

    printf("\nAll Tests Passed Successfully!\n");
    return 0;
}
