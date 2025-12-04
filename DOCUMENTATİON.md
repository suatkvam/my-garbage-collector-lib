# Garbage Collector Library (Thread-Safe) - Documentation

## Table of Contents

1. [Introduction](#introduction)
2. [Thread Safety Model](#thread-safety-model)
3. [Architecture](#architecture)
4. [API Reference](#api-reference)
5. [Scope Management](#scope-management)
6. [Best Practices](#best-practices)

## Introduction

The My Garbage Collector Library (Thread-Safe Edition) is a memory management solution for C programs designed to work safely in concurrent environments. It combines **Scope-Based** tracking with **Mark-and-Sweep** collection, protected by **POSIX Mutexes** to ensure data integrity across multiple threads.

### Key Features

- **Thread-Safe**: Safe to share a `t_gc_context` across multiple threads.
- **Scope-Based Management**: Deterministic cleanup of temporary variables.
- **Mark-and-Sweep**: Automatic cleanup of unreachable memory.
- **Mutex Protection**: Internal locking mechanism prevents race conditions during allocation and collection.

## Thread Safety Model

This library uses a **Coarse-Grained Locking** strategy per Context.

1.  **Context Lock**: Each `t_gc_context` has its own `pthread_mutex_t`.
2.  **Blocking Operations**: When one thread calls `gc_malloc` or `gc_collect`, it locks the context. Other threads trying to use the *same* context will wait until the operation finishes.
3.  **Concurrency**:
    * **Multiple Threads, Single Context**: Safe, but threads may block each other (contention).
    * **Multiple Threads, Multiple Contexts**: Completely independent and parallel (no contention).

## Architecture

### 1. The Context (`t_gc_context`)
The central structure containing:
- **Lock**: A mutex ensuring atomic access to internal lists.
- **Global List**: Tracks all allocations.
- **Scope Stack**: Manages variable lifespans.
- **Stats**: Thread-safe counters for memory usage.

### 2. Scopes
Allocations are tied to the current scope.
- **Push/Pop**: These operations are atomic. When a thread pops a scope, it safely removes allocations made within that scope, even if other threads are allocating elsewhere.

## API Reference

### Initialization and Cleanup

#### `t_gc_context *gc_create(void)`
Creates a new GC context, initializes the internal mutex, and sets up the root scope.

**Returns:**
- A pointer to the new context, or `NULL` if memory/mutex allocation fails.

**Example:**
```c
t_gc_context *gc = gc_create();
if (!gc) return (1);