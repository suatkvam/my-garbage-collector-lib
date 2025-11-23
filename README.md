# My Garbage Collector Library

A simple and efficient garbage collector library for C programs, providing automatic memory management through reference counting and mark-and-sweep collection.

## Features

- **Automatic Memory Management**: Automatically tracks and frees unused memory
- **Reference Counting**: Efficient memory tracking using reference counts
- **Mark-and-Sweep Collection**: Comprehensive garbage collection algorithm
- **Easy Integration**: Simple API for C projects
- **Lightweight**: Minimal overhead and dependencies

## Installation

### Building from Source

```bash
# Clone the repository
git clone https://github.com/suatkvam/my-garbage-collector-lib.git
cd my-garbage-collector-lib

# Build the library
make

# Install (optional)
sudo make install
```

## Quick Start

```c
#include "gc.h"

int main() {
    // Initialize the garbage collector
    gc_init();
    
    // Allocate memory using the garbage collector
    int *data = (int *)gc_malloc(sizeof(int) * 10);
    
    // Use your memory as normal
    for (int i = 0; i < 10; i++) {
        data[i] = i;
    }
    
    // Run garbage collection (automatically frees unused memory)
    gc_collect();
    
    // Cleanup
    gc_cleanup();
    
    return 0;
}
```

## API Reference

### Core Functions

#### `void gc_init(void)`
Initializes the garbage collector. Must be called before any other GC functions.

#### `void *gc_malloc(size_t size)`
Allocates memory that will be tracked by the garbage collector.
- **Parameters**: `size` - Number of bytes to allocate
- **Returns**: Pointer to allocated memory, or NULL on failure

#### `void gc_collect(void)`
Runs the garbage collection algorithm to free unused memory.

#### `void gc_cleanup(void)`
Cleans up and frees all memory managed by the garbage collector. Should be called before program exit.

### Advanced Functions

#### `void gc_retain(void *ptr)`
Increments the reference count for a memory block, preventing it from being collected.
- **Parameters**: `ptr` - Pointer to memory block

#### `void gc_release(void *ptr)`
Decrements the reference count for a memory block.
- **Parameters**: `ptr` - Pointer to memory block

## Usage Examples

See the [examples](./examples) directory for more detailed examples:
- [Basic Usage](./examples/basic_example.c)
- [Reference Counting](./examples/reference_counting_example.c)
- [Data Structures](./examples/data_structure_example.c)

## Documentation

For detailed documentation, see [DOCUMENTATION.md](./DOCUMENTATION.md)

## Building and Testing

```bash
# Build the library
make

# Clean build artifacts
make clean

# Run tests (if available)
make test
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is open source and available under the MIT License.

## Author

**suatkvam**

## Support

If you have any questions or run into issues, please open an issue on GitHub.

## Roadmap

- [ ] Add multi-threading support
- [ ] Implement generational garbage collection
- [ ] Add more comprehensive test suite
- [ ] Performance optimizations
- [ ] Better memory leak detection

## Acknowledgments

Thanks to all contributors and users of this library!
```