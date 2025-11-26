# Dynamic Memory Manager (Kernel Module)

This project implements a custom Dynamic Memory Manager as a Linux Kernel Module. It provides functionality similar to standard C library functions (`malloc`, `free`, `calloc`) but operates directly within the kernel space.

The project demonstrates fundamental operating system concepts such as memory allocation strategies and fragmentation management.

## Features

- **Custom Memory Allocation:** Implements `my_kmalloc`, `my_kfree`, and `my_kcalloc`.
- **Allocation Algorithms:**
  - **First-Fit:** Allocates the first free block that fits the requested size.
  - **Best-Fit:** Allocates the smallest free block that fits the requested size (minimizes wasted space).
  - **Worst-Fit:** Allocates the largest free block (leaves large holes).
- **Fragmentation Management:** Implements **Coalescing** to merge adjacent free blocks and reduce external fragmentation.
- **Doubly Linked List:** Uses a doubly linked list structure to manage memory blocks efficiently.

## Project Structure

```
DDM/
├── Makefile            # Build script for the kernel module
├── src/
│   ├── kernel.c        # Entry point, tests, and demo logic
│   ├── memory.c        # Implementation of the memory manager
│   └── memory.h        # Header file with function prototypes and structs
└── README.md           # This file
```

## Prerequisites

- Linux Operating System (Arch Linux, Ubuntu, etc.)
- GCC Compiler
- Make tool
- Linux Kernel Headers (usually installed via `linux-headers` package)

## How to Build and Run

1.  **Open a terminal** in the project directory.

2.  **Clean previous builds (Optional):**

    ```bash
    make clean
    ```

3.  **Compile and Run:**
    This command compiles the module, inserts it into the kernel, displays the output log, and then removes the module.

    ```bash
    make run
    ```

    _Note: You may be asked for your `sudo` password because inserting kernel modules requires root privileges._

## Understanding the Output

The `make run` command will display the kernel log (dmesg). You will see output similar to this:

```text
[  262.855422] MyMemory: --- Heap Stats ---
[  262.855423] MyMemory: Block at: 000000008ae1501e Size: 104 [USED]
[  262.855423] MyMemory: Block at: 000000001ab6286b Size: 504 [FREE]
...
[  262.855429] MyMemory: Holes available: ~2000 and ~500. Allocating 400.
[  262.855430] MyMemory: Block at: 0000000092f3af78 Size: 400 [USED]
```

- **[USED]:** Indicates an allocated memory block.
- **[FREE]:** Indicates a free memory block available for allocation.
- **Size:** The size of the data area in the block (excluding the header).

## Configuration

You can modify the test scenarios in `src/kernel.c`.

To change the allocation algorithm programmatically:

```c
set_allocation_algorithm(ALGO_FIRST_FIT);
// or
set_allocation_algorithm(ALGO_BEST_FIT);
// or
set_allocation_algorithm(ALGO_WORST_FIT);
```

## Technical Details

- **Block Header:** Each memory block is preceded by a header containing its size, status (free/used), and pointers to the next/previous blocks.
- **Alignment:** All allocations are aligned to 8 bytes to ensure performance and compatibility.
- **Coalescing:** When `my_kfree` is called, the manager checks the previous and next blocks. If they are free, they are merged into a single larger block.

## License

This project is for educational purposes.
