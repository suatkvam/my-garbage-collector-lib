#include "garbage_collector.h"
#include <string.h>

/**
 * Real function declarations
 * These will be linked to the actual libc implementations
 */
void	*__real_malloc(size_t size);
void	*__real_calloc(size_t nmemb, size_t size);
void	*__real_realloc(void *ptr, size_t size);
void	__real_free(void *ptr);

/**
 * __wrap_malloc - Wrapper for malloc that uses GC
 * @size: Size to allocate
 * 
 * This function is called instead of malloc when using --wrap=malloc
 * Return: Allocated memory pointer
 */
void	*__wrap_malloc(size_t size)
{
	return (__real_malloc(size));
}

/**
 * __wrap_calloc - Wrapper for calloc that uses GC
 * @nmemb: Number of elements
 * @size: Size of each element
 * 
 * Return: Allocated and zeroed memory pointer
 */
void	*__wrap_calloc(size_t nmemb, size_t size)
{
	return (__real_calloc(nmemb, size));
}

/**
 * __wrap_realloc - Wrapper for realloc that uses GC
 * @ptr: Pointer to reallocate
 * @size: New size
 * 
 * Return: Reallocated memory pointer
 */
void	*__wrap_realloc(void *ptr, size_t size)
{
	return (__real_realloc(ptr, size));
}

/**
 * __wrap_free - Wrapper for free that uses GC
 * @ptr: Pointer to free
 * 
 * In GC mode, this is often a no-op since GC handles cleanup
 */
void	__wrap_free(void *ptr)
{
	__real_free(ptr);
}