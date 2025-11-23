# Garbage Collector Library - Complete Documentation

## Table of Contents

1. [Introduction](#introduction)
2. [Architecture](#architecture)
3. [API Reference](#api-reference)
4. [Memory Management](#memory-management)
5. [Best Practices](#best-practices)
6. [Performance Considerations](#performance-considerations)
7. [Troubleshooting](#troubleshooting)

## Introduction

The My Garbage Collector Library is a lightweight, efficient memory management solution for C programs. It combines reference counting with mark-and-sweep garbage collection to provide automatic memory management while maintaining low overhead.

### Why Use This Library?

- **Prevent Memory Leaks**: Automatically detect and free unused memory
- **Simplify Code**: Reduce manual memory management burden
- **Safer Programs**: Minimize dangling pointer and double-free errors
- **Easy Integration**: Drop-in replacement for malloc/free in many cases

## Architecture

### Core Components

#### 1. Memory Allocator
The memory allocator tracks all allocated blocks through a linked list or hash table structure. Each allocation includes metadata:
- Block size
- Reference count
- Mark bit (for mark-and-sweep)
- Next pointer (for linked list)

#### 2. Reference Counter
Maintains reference counts for each allocated block. When a reference count reaches zero, the block becomes eligible for collection.

#### 3. Mark-and-Sweep Collector
Periodically scans memory to identify and collect unreachable blocks:
1. **Mark Phase**: Traverses all reachable objects and marks them
2. **Sweep Phase**: Frees all unmarked objects

### Memory Block Structure

```c
typedef struct gc_block {
    size_t size;           // Size of the allocated memory
    int ref_count;         // Reference count
    int marked;            // Mark bit for GC
    void *data;            // Pointer to actual data
    struct gc_block *next; // Next block in chain
} gc_block_t;
```

## API Reference

### Initialization and Cleanup

#### `void gc_init(void)`

Initializes the garbage collector system.

**Description**: Must be called before any other GC functions. Sets up internal data structures and prepares the collector for use.

**Example**:
```c
int main() {
    gc_init();
    // ... your code ...
    gc_cleanup();
    return 0;
}
```

**Thread Safety**: Not thread-safe. Call once from the main thread before creating additional threads.

---

#### `void gc_cleanup(void)`

Cleans up all resources used by the garbage collector.

**Description**: Frees all tracked memory and internal structures. Should be called before program termination.

**Example**:
```c
int main() {
    gc_init();
    // ... your code ...
    gc_cleanup(); // Cleanup before exit
    return 0;
}
```

**Note**: After calling `gc_cleanup()`, you must call `gc_init()` again before using any GC functions.

---

### Memory Allocation

#### `void *gc_malloc(size_t size)`

Allocates memory tracked by the garbage collector.

**Parameters**:
- `size`: Number of bytes to allocate

**Returns**: 
- Pointer to allocated memory on success
- NULL on failure (out of memory)

**Example**:
```c
int *array = (int *)gc_malloc(sizeof(int) * 100);
if (array == NULL) {
    fprintf(stderr, "Allocation failed\n");
    return -1;
}
```

**Initial State**: Newly allocated blocks have a reference count of 1.

---

### Garbage Collection

#### `void gc_collect(void)`

Runs the garbage collection algorithm.

**Description**: Performs mark-and-sweep collection to free unreachable memory blocks. This function can be called manually or will be triggered automatically based on internal heuristics.

**Example**:
```c
// Allocate lots of temporary objects
for (int i = 0; i < 1000; i++) {
    void *temp = gc_malloc(1024);
    // Use temp...
    gc_release(temp);
}

// Force collection to free memory
gc_collect();
```

**Performance**: O(n) where n is the number of allocated blocks. May cause noticeable pauses in real-time applications.

---

### Reference Counting

#### `void gc_retain(void *ptr)`

Increments the reference count of a memory block.

**Parameters**:
- `ptr`: Pointer to memory allocated with `gc_malloc()`

**Description**: Use when creating additional references to prevent premature collection.

**Example**:
```c
typedef struct node {
    int value;
    struct node *next;
} node_t;

node_t *head = gc_malloc(sizeof(node_t));
node_t *current = head;
gc_retain(current); // Keep an extra reference

// ... use current ...

gc_release(current); // Release when done
```

---

#### `void gc_release(void *ptr)`

Decrements the reference count of a memory block.

**Parameters**:
- `ptr`: Pointer to memory allocated with `gc_malloc()`

**Description**: Call when a reference is no longer needed. If reference count reaches zero, the block becomes eligible for collection.

**Example**:
```c
void *data = gc_malloc(1024);
// Use data...
gc_release(data); // Done with this reference
```

**Warning**: Don't use the pointer after calling `gc_release()` if it was the last reference.

---

## Memory Management

### Reference Counting Strategy

#### When to Retain
- Creating new references to existing objects
- Storing pointers in data structures
- Passing ownership to other parts of the code

#### When to Release
- Removing references from data structures
- Exiting scope where reference is no longer needed
- Before reassigning a pointer variable

### Example: Proper Reference Management

```c
typedef struct {
    char *name;
    int age;
} person_t;

person_t *create_person(const char *name, int age) {
    person_t *p = gc_malloc(sizeof(person_t));
    p->name = gc_malloc(strlen(name) + 1);
    strcpy(p->name, name);
    p->age = age;
    return p; // Caller owns the reference
}

void use_person(person_t *p) {
    gc_retain(p); // Retain while using
    printf("Name: %s, Age: %d\n", p->name, p->age);
    gc_release(p); // Release when done
}

int main() {
    gc_init();
    
    person_t *person = create_person("John", 30);
    use_person(person);
    gc_release(person); // Release our reference
    
    gc_collect(); // Clean up
    gc_cleanup();
    return 0;
}
```

## Best Practices

### 1. Initialize Early
Always call `gc_init()` at the beginning of your program, before any allocations.

### 2. Pair Retain/Release
Every `gc_retain()` should have a corresponding `gc_release()`.

### 3. Periodic Collection
Call `gc_collect()` periodically in long-running programs:
```c
static int allocation_count = 0;

void *my_alloc(size_t size) {
    void *ptr = gc_malloc(size);
    if (++allocation_count > 1000) {
        gc_collect();
        allocation_count = 0;
    }
    return ptr;
}
```

### 4. Cleanup Before Exit
Always call `gc_cleanup()` before program termination.

### 5. Avoid Circular References
Be careful with circular data structures:
```c
// Problematic: circular reference
typedef struct node {
    struct node *next;
    struct node *prev;
} node_t;

// Solution: use weak references or manual cycle breaking
```

## Performance Considerations

### Memory Overhead
- Each allocation has metadata overhead (~24-32 bytes per block)
- Reference counting adds minimal per-operation cost
- Mark-and-sweep requires periodic scanning

### Optimization Tips

#### 1. Batch Allocations
```c
// Less efficient: many small allocations
for (int i = 0; i < 1000; i++) {
    int *n = gc_malloc(sizeof(int));
}

// More efficient: fewer large allocations
int *array = gc_malloc(sizeof(int) * 1000);
```

#### 2. Control Collection Frequency
```c
// Before intensive allocation
gc_collect(); // Start with clean slate

// Disable auto-collection during critical sections
// (if supported by your version)

// Re-enable and collect after critical section
gc_collect();
```

#### 3. Minimize Retain/Release Calls
```c
// Less efficient
gc_retain(obj);
process(obj);
gc_release(obj);
gc_retain(obj);
transform(obj);
gc_release(obj);

// More efficient
gc_retain(obj);
process(obj);
transform(obj);
gc_release(obj);
```

## Troubleshooting

### Memory Leaks

**Symptom**: Memory usage grows continuously

**Causes**:
1. Missing `gc_release()` calls
2. Circular references
3. Forgotten root references

**Solution**:
```c
// Add debugging to track allocations
#ifdef DEBUG_GC
#define gc_malloc(size) gc_malloc_debug(size, __FILE__, __LINE__)
#endif
```

### Premature Collection

**Symptom**: Program crashes or corrupted data

**Causes**:
1. Too many `gc_release()` calls
2. Missing `gc_retain()` calls
3. Using pointers after release

**Solution**: Audit retain/release pairs, use memory debugging tools.

### Performance Issues

**Symptom**: Unexpected pauses or slowdowns

**Causes**:
1. Too frequent garbage collection
2. Large number of tracked objects
3. Deep object graphs

**Solution**: Reduce collection frequency, optimize data structures.

### Common Error Messages

#### "GC not initialized"
- **Cause**: Calling GC functions before `gc_init()`
- **Solution**: Call `gc_init()` first

#### "Invalid pointer"
- **Cause**: Passing non-GC memory to GC functions
- **Solution**: Only use pointers from `gc_malloc()`

#### "Double release detected"
- **Cause**: Calling `gc_release()` too many times
- **Solution**: Balance retain/release calls

## Advanced Topics

### Custom Finalizers

Some versions support custom cleanup functions:
```c
void my_finalizer(void *obj) {
    // Custom cleanup code
    my_object_t *o = (my_object_t *)obj;
    // Close files, release resources, etc.
}

void *obj = gc_malloc_with_finalizer(size, my_finalizer);
```

### Statistics and Monitoring

```c
// Get GC statistics (if supported)
gc_stats_t stats = gc_get_stats();
printf("Allocated: %zu bytes\n", stats.total_allocated);
printf("Collections: %d\n", stats.collection_count);
printf("Freed: %zu bytes\n", stats.total_freed);
```

## Conclusion

This garbage collector library provides a balance between automatic memory management and performance. By following the guidelines in this documentation, you can effectively integrate it into your C projects and benefit from reduced memory management burden.

For more examples and use cases, see the examples directory in the repository.