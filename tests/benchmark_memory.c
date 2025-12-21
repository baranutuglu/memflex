#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#define UNIT_TESTING

// Mock kernel macros
#define KERN_INFO ""
#define KERN_CONT ""
#define KERN_ERR ""
#define printk(...)

// Include source directly
#include "../src/memory.c"

#define HEAP_SIZE (1024 * 1024 * 10) // 10 MB Heap
#define NUM_OPS 10000                // 10.000 İşlem

typedef struct
{
    void *ptr;
    size_t size;
} alloc_record_t;

alloc_record_t allocations[NUM_OPS];

typedef struct
{
    char name[20];
    double time;
    int free_blocks;
} benchmark_result_t;

benchmark_result_t results[3];
int result_idx = 0;

void run_benchmark(alloc_algo_t algo, const char *algo_name)
{
    // Reset heap
    void *heap_mem = malloc(HEAP_SIZE);
    heap_init(heap_mem, HEAP_SIZE);
    set_allocation_algorithm(algo);

    memset(allocations, 0, sizeof(allocations));

    clock_t start = clock();

    int alloc_count = 0;

    // Rastgele işlemler döngüsü
    for (int i = 0; i < NUM_OPS; i++)
    {
        // %60 ihtimalle alloc, %40 ihtimalle free (eğer alloc edilmiş varsa)
        int action = rand() % 100;

        if (action < 60)
        {
            // ALLOC
            size_t size = (rand() % 1024) + 1; // 1 byte - 1KB arası
            void *p = my_kmalloc(size);
            if (p)
            {
                allocations[alloc_count].ptr = p;
                allocations[alloc_count].size = size;
                alloc_count++;
            }
        }
        else
        {
            // FREE
            if (alloc_count > 0)
            {
                int idx = rand() % alloc_count;
                if (allocations[idx].ptr)
                {
                    my_kfree(allocations[idx].ptr);
                    // Listeden silmek yerine son elemanla yer değiştirip sayıyı azaltalım
                    allocations[idx] = allocations[alloc_count - 1];
                    alloc_count--;
                }
            }
        }
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    // Fragmentasyon ölçümü (Basitçe kaç tane free blok kalmış ona bakalım)
    int free_blocks = 0;
    block_header_t *curr = head;
    while (curr)
    {
        if (curr->is_free)
            free_blocks++;
        curr = curr->next;
    }

    printf("%-15s | Süre: %f sn | Kalan Free Blok Sayısı: %d\n", algo_name, time_spent, free_blocks);

    // Save results
    if (result_idx < 3)
    {
        strncpy(results[result_idx].name, algo_name, 19);
        results[result_idx].time = time_spent;
        results[result_idx].free_blocks = free_blocks;
        result_idx++;
    }

    free(heap_mem);
}

void save_results_to_json()
{
    FILE *f = fopen("results.json", "w");
    if (!f)
        return;
    fprintf(f, "[\n");
    for (int i = 0; i < result_idx; i++)
    {
        fprintf(f, "  {\"name\": \"%s\", \"time\": %f, \"free_blocks\": %d}%s\n",
                results[i].name, results[i].time, results[i].free_blocks,
                (i < result_idx - 1) ? "," : "");
    }
    fprintf(f, "]\n");
    fclose(f);
    printf("Sonuçlar results.json dosyasına kaydedildi.\n");
}

int main()
{
    srand(time(NULL));

    printf("=== Performans Karşılaştırması (10MB Heap, %d Rastgele İşlem) ===\n", NUM_OPS);
    printf("------------------------------------------------------------------\n");

    run_benchmark(ALGO_FIRST_FIT, "First Fit");
    run_benchmark(ALGO_BEST_FIT, "Best Fit");
    run_benchmark(ALGO_WORST_FIT, "Worst Fit");

    save_results_to_json();

    return 0;
}
