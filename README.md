# My Garbage Collector Library (Thread-Safe)

[![Build Status](https://github. com/suatkvam/my-garbage-collector-lib/workflows/Build%20and%20Release/badge.svg)](https://github.com/suatkvam/my-garbage-collector-lib/actions)
[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202. 0-brightgreen. svg)](https://opensource.org/licenses/MPL-2.0)
[![Platform](https://img.shields.io/badge/platform-linux%20%7C%20macos-lightgrey. svg)](https://github.com/suatkvam/my-garbage-collector-lib)

A high-performance garbage collector library for C programs, providing automatic memory management through scope-based tracking and mark-and-sweep collection.

## 🎯 Features

- **Scope-Based Management**: Deterministic memory cleanup with automatic scope tracking
- **Mark-and-Sweep Collection**: Comprehensive garbage collection for unreachable objects
- **Multiple GC Modes**: Manual, Auto, and Hybrid collection modes
- **High Performance**: 23% faster than standard malloc in benchmarks
- **Rich String Utilities**: Built-in string manipulation with GC tracking
- **Easy Integration**: Simple API with minimal overhead
- **Debug Support**: Comprehensive statistics and debugging tools
- **Legacy Code Wrapper**: Seamless integration with existing codebases
- **Thread-Safe Operations**: Full concurrent support with mutex-based synchronization

## 🔒 Thread Safety

The garbage collector is **fully thread-safe** when using the same `t_gc_context` across multiple threads. 

### Features
- ✅ Concurrent allocations from multiple threads
- ✅ Thread-safe scope management
- ✅ Protected mark-and-sweep collection
- ✅ POSIX mutex-based synchronization
- ✅ Read/Write stress tested for race condition safety

### Thread-Safe Operations
```c
t_gc_context *gc = gc_create();

// Thread 1
void *ptr1 = gc_malloc(gc, 1024);
gc_scope_push(gc);
// ... 

// Thread 2 (same gc context)
void *ptr2 = gc_malloc(gc, 2048);
gc_collect(gc);  // Safe concurrent collection
```

### Performance
- **Overhead**: ~5-15% due to mutex locking
- **Scalability**: Good for 2-8 threads
- **Single-threaded benchmark**: ~23% faster than standard malloc/free
- **Comparison** (10K operations):
  - Standard malloc/free: 4.97 ms
  - GC malloc: 3. 85 ms
  - **Result: ~23% faster in single-threaded scenarios!** ⚡

### Testing

The library includes comprehensive thread-safety tests:

**1. Concurrent Allocation Test**
```bash
make
./thread_safe_test
```

Runs concurrent tests with 8 threads performing:
- 1000 allocations per thread
- 500 scope operations per thread
- 300 string operations per thread

**2. Read/Write Stress Test**
```bash
./thread_safe_read_test
```

Tests reading statistics while other threads are heavily modifying memory. This ensures no race conditions occur during list traversal.

### Implementation Details
- Uses `pthread_mutex_t` for critical sections
- Locks protect:
  - Allocation lists (global + scope)
  - Statistics updates
  - Scope stack modifications
  - Mark-and-sweep phases
- **No locks** in string utilities (they call `gc_malloc` which handles locking)

### Thread-Safe API Reference

#### Context Management

**`t_gc_context *gc_create(void)`**

Creates a new context with internal mutex lock initialization.

**`void gc_destroy(t_gc_context *ctx)`**

Acquires the lock, frees all managed memory, destroys the mutex, and frees the context itself. 

⚠️ **Warning**: Ensure no other threads are actively using the context before calling destroy.

#### Memory Allocation (Thread-Safe)

All allocation functions are thread-safe and can be called concurrently:

**`void *gc_malloc(t_gc_context *ctx, size_t size)`**

Allocates tracked memory. Locks the context during allocation to update internal lists safely. 

**`void *gc_calloc(t_gc_context *ctx, size_t nmemb, size_t size)`**

Allocates zero-initialized memory safely using gc_malloc.

**`void *gc_realloc(t_gc_context *ctx, void *ptr, size_t size)`**

Resizes an existing allocation atomically.

#### Collection Control

**`void gc_collect(t_gc_context *ctx)`**

Triggers a Mark-and-Sweep cycle. 

⚠️ **Blocking**: This operation holds the lock for the duration of the mark and sweep phases. Other threads will be blocked if they try to allocate during an ongoing collection.

**`void gc_set_mode(t_gc_context *ctx, t_gc_mode mode)`**

Sets the collection mode (Manual, Auto, Hybrid) safely using mutex protection.

#### String Utilities

All string utilities (`gc_strdup`, `gc_strjoin`, etc.) rely on `gc_malloc`. Therefore, they inherit the thread-safety of the underlying allocator.

#### Scope Management

Scope operations are protected by locks to ensure the scope stack remains consistent even under high concurrency.

**`int gc_scope_push(t_gc_context *ctx)`**

Atomic operation to push a new scope level.

**`void gc_scope_pop(t_gc_context *ctx)`**

Atomic operation to pop the current scope and free all associated resources. 

⚠️ **Concurrency Note**: If Thread A pushes a scope and Thread B pushes a scope on the same context, they are essentially sharing the same scope stack depth. For strictly isolated scopes per thread, use separate contexts.

### Best Practices for Multi-Threading

#### ✅ DO:

1. **Context per Thread (Recommended)**: For maximum performance in high-concurrency applications, create a separate `gc_create()` context for each worker thread. This eliminates lock contention entirely.

```c
// Thread function
void *worker_thread(void *arg)
{
    t_gc_context *gc = gc_create();  // Each thread has its own context
    
    // Do work... 
    void *data = gc_malloc(gc, 1024);
    
    gc_destroy(gc);
    return NULL;
}
```

2. **Use Manual Mode for Predictable Timing**: Use `GC_MODE_MANUAL` to control when collections happen to avoid unexpected pauses. 

3. **Short Critical Sections**: The library is optimized to keep locks for as short a time as possible. 

#### ❌ DON'T:

1. **Shared Context Under Heavy Load**: If you must share data between threads using a single GC context, be aware that heavy allocation in one thread might slightly delay others due to locking. 

2. **Destroy While Active**: Never call `gc_destroy()` while other threads are still using the context. 

3. **Long Collections on Large Heaps**: Be aware that `gc_collect()` on a very large heap can take time and block other threads.

### Example: Multi-Threaded Usage

```c
#include "garbage_collector.h"
#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 4

void *worker(void *arg)
{
    t_gc_context *gc = (t_gc_context *)arg;
    
    for (int i = 0; i < 1000; i++)
    {
        gc_scope_push(gc);
        
        char *str = gc_strdup(gc, "Thread-safe allocation");
        int *numbers = gc_malloc(gc, sizeof(int) * 100);
        
        // Do work with allocations... 
        
        gc_scope_pop(gc);  // Atomic cleanup
    }
    
    return NULL;
}

int main(void)
{
    t_gc_context *gc = gc_create();
    pthread_t threads[NUM_THREADS];
    
    // Launch threads sharing same context
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, worker, gc);
    
    // Wait for completion
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);
    
    gc_print_stats(gc);
    gc_destroy(gc);
    
    return 0;
}
```

## 📊 Performance

Benchmarked on WSL2 / Ubuntu 22.04:

| Operation | Scale | Time | Per Operation |
|-----------|-------|------|---------------|
| gc_malloc | 1,000 | 0.42 ms | 0.0004 ms |
| gc_malloc | 10,000 | 6.58 ms | 0.0007 ms |
| gc_malloc | 100,000 | 81.06 ms | 0.0008 ms |
| gc_collect | 1,000 objects | 0.01 ms | - |
| gc_collect | 10,000 objects | 0.39 ms | - |
| gc_collect | 100,000 objects | 10.95 ms | - |
| Scope push/pop | 1,000 cycles | 0.11 ms | 0.0001 ms |

**Comparison with Standard malloc:**
- Standard malloc/free: 4.97 ms (10K operations)
- GC malloc: 3.85 ms (10K operations)
- **Result: ~23% faster! ** ⚡

## 🚀 Quick Start

### Installation
```bash
# Clone the repository
git clone https://github.com/suatkvam/my-garbage-collector-lib. git
cd my-garbage-collector-lib

# Build the library
make

# Optional: Install system-wide
sudo make install
```

### Basic Usage
```c
#include "garbage_collector.h"
#include <stdio.h>

int main(void)
{
    // Create GC context
    t_gc_context *gc = gc_create();
    
    // Allocate memory
    int *numbers = gc_malloc(gc, sizeof(int) * 10);
    char *message = gc_strdup(gc, "Hello, GC!");
    
    // Use memory normally
    for (int i = 0; i < 10; i++)
        numbers[i] = i * 10;
    
    printf("%s\n", message);
    
    // Automatic cleanup on destroy
    gc_destroy(gc);
    
    return 0;
}
```

**Compile:**
```bash
cc your_program.c garbage_collecter.a -o your_program
```

## 📚 Core API

### Context Management

#### `t_gc_context *gc_create(void)`
Creates and initializes a new GC context with automatic root scope. 

#### `void gc_destroy(t_gc_context *context)`
Destroys context and frees all managed memory.

### Memory Allocation

#### `void *gc_malloc(t_gc_context *ctx, size_t size)`
Allocates memory tracked by the garbage collector.

#### `void *gc_calloc(t_gc_context *ctx, size_t nmemb, size_t size)`
Allocates and zero-initializes memory. 

#### `void *gc_realloc(t_gc_context *ctx, void *ptr, size_t size)`
Reallocates memory block to new size.

### String Utilities
```c
// String duplication with GC tracking
char *gc_strdup(t_gc_context *ctx, const char *s);
char *gc_strndup(t_gc_context *ctx, const char *s, size_t n);

// String manipulation
char *gc_strjoin(t_gc_context *ctx, const char *s1, const char *s2);
char *gc_substr(t_gc_context *ctx, const char *s, size_t start, size_t len);
char *gc_strtrim(t_gc_context *ctx, const char *s1, const char *set);

// Non-allocating utilities
size_t gc_strlen(const char *s);
int gc_strcmp(const char *s1, const char *s2);
int gc_strncmp(const char *s1, const char *s2, size_t n);
char *gc_strchr(const char *s, int c);
char *gc_strrchr(const char *s, int c);

// Conversion functions
char *gc_itoa(t_gc_context *ctx, int n);
char *gc_uitoa(t_gc_context *ctx, size_t n);
int gc_atoi(const char *str);
```

### Scope Management
```c
// Push new scope
gc_scope_push(gc);

// Allocations in this scope
char *temp = gc_malloc(gc, 1024);

// Pop scope - automatic cleanup! 
gc_scope_pop(gc);
```

**Example:**
```c
void process_data(t_gc_context *gc)
{
    gc_scope_push(gc);
    
    // All allocations here are temporary
    char *buffer = gc_malloc(gc, 4096);
    char *result = gc_strdup(gc, "temporary");
    
    // Automatic cleanup on scope pop
    gc_scope_pop(gc);
}
```

### Collection Modes
```c
typedef enum {
    GC_MODE_MANUAL,  // Manual gc_collect() calls only
    GC_MODE_AUTO,    // Automatic collection on thresholds
    GC_MODE_HYBRID   // Combination of both
} t_gc_mode;

gc_set_mode(gc, GC_MODE_AUTO);
gc_collect(gc);  // Manual collection
```

### Statistics & Debugging
```c
typedef struct {
    size_t total_allocated;
    size_t total_freed;
    size_t current_usage;
    size_t peak_usage;
    size_t allocation_count;
    size_t free_count;
    size_t scope_depth;
} t_gc_stats;

t_gc_stats stats;
gc_get_stats(gc, &stats);
gc_print_stats(gc);  // Pretty print to stdout

gc_set_debug(gc, GC_TRUE);  // Enable debug mode
```

## 🔧 Advanced Usage

### Legacy Code Integration (Wrapper)

Integrate with existing code without modifying malloc/free calls:

**Add to your Makefile:**
```makefile
GC_LIB = path/to/garbage_collecter.a
GC_WRAP = -Wl,--wrap=malloc -Wl,--wrap=calloc \
          -Wl,--wrap=realloc -Wl,--wrap=free

$(NAME): $(OBJS) $(GC_LIB)
	$(CC) $(OBJS) $(GC_LIB) $(GC_WRAP) -o $(NAME)
```

**In your code:**
```c
#include "garbage_collector.h"

int main(void)
{
    t_gc_context *gc = gc_create();
    gc_wrapper_push_context(gc);
    
    // Now malloc/free use GC automatically! 
    int *data = malloc(100 * sizeof(int));
    char *str = strdup("wrapped!");
    
    gc_wrapper_pop_context();
    gc_destroy(gc);
    return 0;
}
```

### Configuration
```c
// Set collection threshold (default: 10MB)
gc->collect_threshold = 20 * 1024 * 1024;

// Set collection interval (default: 1000 allocations)
gc->collect_interval = 5000;

// Get current mode
t_gc_mode mode = gc_get_mode(gc);
```

## 📖 Examples

The `examples/` directory contains detailed demonstrations:

1. **basic_example.c** - Core functionality
2. **scope_example.c** - Scope-based management
3. **modes_example.c** - Different GC modes
4. **string_utils_example.c** - String utilities
5. **debug_example.c** - Statistics and debugging
6. **benchmark_example.c** - Performance testing
7. **thread_safe_test.c** - Concurrent allocation test
8. **thread_safe_read_test.c** - Read/Write stress test

**Build and run:**
```bash
cd examples
cc basic_example.c ../garbage_collecter.a -o basic_example
./basic_example
```

## 🏗️ Architecture

### Memory Layout
```
User Allocation:
┌─────────────────────┐
│   User Data (N)     │  ← Returned to user
└─────────────────────┘

Metadata (Internal):
┌─────────────────────┐
│ t_gc_allocation     │  ← Tracking structure
│ - ptr               │
│ - size              │
│ - scope_level       │
│ - marked            │
│ - next/prev         │
│ - scope_next        │
└─────────────────────┘
```

### Dual-List Structure

Each allocation exists in TWO lists:
1. **Global List** (doubly-linked): All allocations for mark-sweep
2. **Scope List** (singly-linked): Scope-specific for deterministic cleanup

### Collection Strategy

**Scope-Based (Fast, Deterministic):**
```c
gc_scope_push(gc);     // Create scope
// ...  allocations ...
gc_scope_pop(gc);      // O(n) - free all in scope
```

**Mark-Sweep (Comprehensive):**
```c
gc_collect(gc);
// 1. Mark phase: O(n) - traverse all scopes
// 2. Sweep phase: O(n) - free unmarked
```

## 🎯 Best Practices

### ✅ DO:
- Use scopes for temporary allocations
- Call `gc_destroy()` before program exit
- Set appropriate collection thresholds
- Use `gc_strdup` instead of manual string copying
- Use separate contexts per thread for maximum performance

### ❌ DON'T:
- Mix GC and non-GC allocations
- Use pointers after scope pop
- Forget to create GC context
- Rely on collection timing in critical code
- Destroy context while other threads are using it

### Memory Management Patterns

**Pattern 1: Function-Local Memory**
```c
void process(t_gc_context *gc, const char *input)
{
    gc_scope_push(gc);
    
    char *temp = gc_strdup(gc, input);
    char *result = transform(gc, temp);
    
    gc_scope_pop(gc);  // Auto cleanup
}
```

**Pattern 2: Long-Lived Objects**
```c
typedef struct {
    char *name;
    int value;
} config_t;

config_t *load_config(t_gc_context *gc)
{
    // Allocate in root scope (created by gc_create)
    config_t *cfg = gc_malloc(gc, sizeof(config_t));
    cfg->name = gc_strdup(gc, "config");
    return cfg;  // Lives until gc_destroy()
}
```

**Pattern 3: Hybrid Approach**
```c
void bulk_process(t_gc_context *gc)
{
    gc_set_mode(gc, GC_MODE_HYBRID);
    
    for (int i = 0; i < 1000; i++) {
        gc_scope_push(gc);
        process_item(gc, i);
        gc_scope_pop(gc);
        
        // Auto-collect if threshold reached
    }
}
```

## 📜 License

This project is licensed under the **Mozilla Public License 2.0** (MPL-2.0).

### 🤔 What does this mean? 

**You CAN:**
- ✅ Use this library in commercial projects
- ✅ Use it in closed-source software
- ✅ Modify the code
- ✅ Distribute it
- ✅ Combine with other licenses

**You MUST:**
- 📄 Keep the license notice
- 📄 Share modifications to **MPL-licensed files**
- 📄 Disclose source of modified MPL files

**You DON'T have to:**
- ❌ Open source your entire project
- ❌ Share your proprietary code
- ❌ Use the same license for your code

### 💼 Commercial Use Example
```
Your Game/Software (Proprietary)
├── game_engine.c         ← Your code (closed source ✅)
├── graphics. c            ← Your code (closed source ✅)
└── lib/
    ├── gc_malloc.c       ← MPL file (if modified, share ✅)
    └── gc_collect.c      ← MPL file (if modified, share ✅)
```

**Result:** Your game stays closed-source, but if you improve `gc_malloc.c`, 
you share that improvement back to the community.  Fair deal!  🤝

### 🎯 Why MPL 2.0?

This library represents **1 week of intensive development**.  MPL 2.0 ensures:
- 🔒 My work is protected
- 🤝 Improvements come back to me
- 💼 Companies can use it commercially
- 🆓 Open source projects benefit
- 🛡️ Patent protection included

See [LICENSE](LICENSE) for full terms.

## 🐛 Troubleshooting

### Memory Not Being Freed

**Problem:** Statistics show no memory freed after collection.

**Solution:** Check if allocations are in active scopes:
```c
// Memory stays because it's in root scope
gc_malloc(gc, 1024);
gc_collect(gc);  // Won't free (still in scope)

// Solution: Use scope pop
gc_scope_pop(gc);  // Frees memory
```

### Performance Issues

**Problem:** Collection takes too long. 

**Solution:** Adjust thresholds:
```c
gc->collect_threshold = 50 * 1024 * 1024;  // 50MB
gc->collect_interval = 10000;              // Every 10K allocs
```

### Thread Contention

**Problem:** High lock contention in multi-threaded scenarios.

**Solution:** Use separate contexts per thread:
```c
// Instead of sharing one context
pthread_t threads[8];
t_gc_context *gc = gc_create();

// Use one context per thread
pthread_t threads[8];
t_gc_context *contexts[8];
for (int i = 0; i < 8; i++)
    contexts[i] = gc_create();
```

## 📈 Roadmap

- [x] Basic scope-based management
- [x] Mark-and-sweep collection
- [x] Multiple GC modes
- [x] String utilities
- [x] Statistics and debugging
- [x] Performance benchmarking
- [x] CI/CD with GitHub Actions
- [x] Thread-safe version
- [ ] Generational collection
- [ ] Compacting collector
- [ ] Finalizer support

## 🤝 Contributing

Contributions are welcome! Please:

1. Fork the repository
2.  Create a feature branch (`git checkout -b feature/amazing`)
3. Follow 42 coding standards
4. Add tests for new features
5. Submit a pull request

## 👤 Author

**suatkvam**
- GitHub: [@suatkvam](https://github.com/suatkvam)

## 🙏 Acknowledgments

Thanks to all contributors and the C community! 

## 📞 Support

- Issues: [GitHub Issues](https://github.com/suatkvam/my-garbage-collector-lib/issues)
- Discussions: [GitHub Discussions](https://github.com/suatkvam/my-garbage-collector-lib/discussions)

---

**Made with ❤️ for 42 and the C community**

*One week of intensive development, thousands of lines of code, infinite possibilities.*