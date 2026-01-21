# Garbage Collector Library - Documentation

## Table of Contents

1. [Introduction](#introduction)
2. [Architecture](#architecture)
3. [API Reference](#api-reference)
4. [Scope Management](#scope-management)
5. [String Utilities](#string-utilities)
6. [Best Practices](#best-practices)

## Introduction

The My Garbage Collector Library is a lightweight memory management solution for C programs. It uses a **Scope-Based** approach combined with a **Mark-and-Sweep** collector to provide automatic memory management.

### Key Features

- **Scope-Based Management**: Deterministic cleanup of temporary variables using scope stacks.
- **Auto-Collection**: Automatically triggers garbage collection based on memory usage thresholds.
- **Leak Prevention**: Keeps track of all allocations to ensure everything is freed at the end.
- **Standard-like API**: Provides `gc_malloc`, `gc_calloc`, and `gc_realloc` similar to standard libc.
- **Memory Pool Optimization**: Pre-allocated pools for small allocations (<256 bytes) reduce fragmentation.
- **Hash Table Integration**: O(1) allocation lookup instead of O(n) linear search for better performance.
- **Fork-Safe Design**: Copy-on-Write (COW) mechanism for safe process forking.

## Architecture

### 1. The Context (`t_gc_context`)
The heart of the library. It holds:
- A global doubly-linked list of all allocations (for tracking and sweeping).
- A stack of "Scopes" (for managing variable lifespan).
- A hash table (1024 buckets) for O(1) allocation lookup.
- Memory pool list for small allocation optimization.
- Statistics (memory usage, allocation counts, peak usage).
- Configuration (modes, thresholds, debug settings).

### 2. Scopes
Allocations are tied to the "current scope".
- When you enter a function or a logical block, you can **push** a new scope.
- When you leave, you **pop** the scope, which automatically frees all memory allocated within that duration.

### 3. Mark-and-Sweep
Used for general cleanup or when memory limits are reached:
1. **Mark**: Iterates through all active scopes and marks reachable memory.
2. **Sweep**: Iterates through the global list and frees anything that wasn't marked.

### 4. Memory Pool Optimization
Small allocations (<256 bytes) use pre-allocated memory pools:
- **Bump Allocator**: Fast sequential allocation from pool blocks.
- **Reduced Fragmentation**: Fewer system malloc calls for small objects.
- **1MB Pool Size**: Default pool size (`GC_POOL_SIZE`).
- **Automatic Management**: Pools are freed when the GC context is destroyed.

### 5. Hash Table for Fast Lookup
FNV-1a hash-based allocation registry:
- **O(1) Lookup**: Constant-time allocation search instead of O(n).
- **1024 Buckets**: Default hash table size (`GC_HASH_SIZE`).
- **Efficient Operations**: Faster `gc_realloc`, `gc_find_allocation`, and `gc_untrack`.

### 6. Dual-List Structure
Each allocation exists in two lists:
1. **Global List** (doubly-linked): All allocations for mark-sweep and tracking.
2. **Scope List** (singly-linked): Scope-specific allocations for deterministic cleanup.

This dual structure enables both fast scope-based cleanup and comprehensive garbage collection.

## API Reference

### Initialization and Cleanup

#### `t_gc_context *gc_create(void)`
Creates a new garbage collector context and initializes the root scope.

**Returns:**
- A pointer to the new context, or `NULL` on failure.

**Example:**
```c
t_gc_context *gc = gc_create();
if (!gc) return (1);
```

#### `void gc_destroy(t_gc_context *context)`
Destroys the GC context and frees all tracked allocations.

**Parameters:**
- `context`: The GC context to destroy.

**Example:**
```c
gc_destroy(gc);
```

### Memory Allocation Functions

#### `void *gc_malloc(t_gc_context *contex, size_t size)`
Allocates memory with garbage collector tracking.

**Parameters:**
- `contex`: The GC context.
- `size`: Size in bytes to allocate.

**Returns:**
- Pointer to allocated memory, or `NULL` on failure.

**Notes:**
- Small allocations (<256 bytes) use memory pools for better performance.
- Large allocations use system malloc directly.

#### `void *gc_calloc(t_gc_context *contex, size_t nmemb, size_t size)`
Allocates and zero-initializes memory.

#### `void *gc_realloc(t_gc_context *contex, void *ptr, size_t size)`
Reallocates memory block to new size.

### External Memory Tracking

#### `void *gc_track(t_gc_context *contex, void *ptr)`
Tracks externally allocated memory (e.g., from `getcwd`, `readline`).

**Example:**
```c
char *path = getcwd(NULL, 0);
gc_track(gc, path);  // Now managed by GC
```

#### `void *gc_track_sized(t_gc_context *contex, void *ptr, size_t size)`
Tracks external allocation with known size.

#### `void gc_untrack(t_gc_context *contex, void *ptr)`
Removes pointer from GC tracking. Use before manual `free()`.

**Important:** As of the latest version, `gc_untrack` properly removes allocations from both global and scope lists, preventing memory leaks.

**Example:**
```c
void *ptr = malloc(1024);
gc_track(gc, ptr);
gc_untrack(gc, ptr);  // Safe to free manually now
free(ptr);
```

## Recent Improvements

### Bug Fixes (Latest Version)

1. **gc_untrack Scope Leak Fix**
   - Previously, `gc_untrack` only removed allocations from the global list.
   - Now properly removes from both global and scope lists.
   - Prevents dangling pointers and potential segmentation faults.

2. **Code Organization**
   - `gc_add_to_global` moved to `gc_track_utils.c` for better modularity.
   - Improved code structure and maintainability.

3. **Norminette Compliance**
   - All comment styles converted from `//` to `/* */` for 42 coding standards.
   - Fixed typos and improved documentation clarity.

## Known Limitations

- **Thread Safety**: Current version (optimization branch) is not thread-safe.
  - Thread-safe version is in development on a separate branch.
  - Use mutex locks if sharing GC context between threads.

- **Cyclic References**: Mark-and-sweep collector handles most cases, but deeply nested cyclic structures may require manual intervention.

## Performance Tips

1. **Use Scopes Aggressively**: Push/pop scopes frequently for deterministic cleanup.
2. **Tune Thresholds**: Adjust `collect_threshold` and `collect_interval` based on workload.
3. **Small Allocations**: Keep allocations under 256 bytes when possible to benefit from memory pools.
4. **Fork Safely**: Use COW mechanism - each process should manage its own GC context after fork.