# Garbage Collector Examples

This directory contains example programs demonstrating how to use the garbage collector library.

## Building

First, build the library:
```bash
cd ..
make
```

Then compile any example:
```bash
cc -Wall -Wextra -Werror examples/basic_example.c garbage_collecter.a -o basic_example
```

## Examples

### 1. basic_example.c
Basic usage: `gc_malloc`, `gc_calloc`, `gc_strdup` functions.
```bash
cc examples/basic_example.c garbage_collecter.a -o basic_example
./basic_example
```

### 2. scope_example.c
Scope management: automatic memory cleanup with `gc_scope_push` and `gc_scope_pop`.
```bash
cc examples/scope_example.c garbage_collecter.a -o scope_example
./scope_example
```

### 3. modes_example.c
GC modes: usage of MANUAL, AUTO and HYBRID modes.
```bash
cc examples/modes_example.c garbage_collecter.a -o modes_example
./modes_example
```

### 4. string_utils_example.c
String functions: `gc_strdup`, `gc_strndup`, `gc_strjoin`.
```bash
cc examples/string_utils_example.c garbage_collecter.a -o string_utils_example
./string_utils_example
```

### 5. debug_example.c
Debug mode: statistics and memory tracking.
```bash
cc examples/debug_example.c garbage_collecter.a -o debug_example
./debug_example
```

### 6. pool_test.c ✨ NEW
Memory pool functionality: small vs large allocations, stress test.
```bash
cc examples/pool_test.c garbage_collecter.a -o pool_test
./pool_test
```

### 7. fork_test.c ✨ NEW
Fork safety: COW mechanism, parent-child data sharing.
```bash
cc examples/fork_test.c garbage_collecter.a -o fork_test
./fork_test
```

### 8. benchmark_example.c
Performance benchmarks: compare GC vs standard malloc, measure overhead.
```bash
cc examples/benchmark_example.c garbage_collecter.a -o benchmark_example
./benchmark_example
```

## Quick Test

To run all examples in sequence:
```bash
for example in basic scope modes string_utils debug pool_test fork_test benchmark; do
    cc examples/${example}_example.c garbage_collecter.a -o ${example}_example 2>/dev/null || \
    cc examples/${example}.c garbage_collecter.a -o ${example} 2>/dev/null
    ./${example}_example 2>/dev/null || ./${example}
    echo ""
done
```

## Cleanup

To remove compiled files:
```bash
rm -f *_example pool_test fork_test benchmark
```
