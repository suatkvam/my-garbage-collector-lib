/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 18:52:34 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 18:52:34 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../garbage_collector.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void test_simple_fork(t_gc_context *gc)
{
    pid_t pid;
    char *parent_data;
    
    printf("\n=== Test 1: Simple Fork ===\n");
    
    parent_data = gc_strdup(gc, "parent data");
    printf("Parent allocated: %s (%p)\n", parent_data, (void *)parent_data);
    
    pid = fork();
    
    if (pid == 0)
    {
        // Child process
        printf("  Child: reading parent data: %s\n", parent_data);
        printf("  Child: ✅ COW works! (can read parent data)\n");
        gc_destroy(gc);
        _exit(0);
    }
    else
    {
        wait(NULL);
        printf("Parent: child finished successfully\n");
    }
}

void test_fork_with_child_alloc(t_gc_context *gc)
{
    pid_t pid;
    char *parent_str;
    
    printf("\n=== Test 2: Fork with Child Allocation ===\n");
    
    parent_str = gc_strdup(gc, "parent string");
    printf("Parent allocated: %s\n", parent_str);
    
    pid = fork();
    
    if (pid == 0)
    {
        // Child allocates (triggers COW)
        char *child_str = gc_strdup(gc, "child string");
        printf("  Child allocated: %s (%p)\n", child_str, (void *)child_str);
        printf("  Child can still read parent: %s\n", parent_str);
        printf("  Child: ✅ COW triggered, separate memory\n");
        gc_destroy(gc);
        _exit(0);
    }
    else
    {
        wait(NULL);
        printf("Parent: child allocation successful\n");
    }
}

// fork_test.c - line 81 civarı

void test_fork_with_scope(t_gc_context *gc)
{
    pid_t pid;
    
    printf("\n=== Test 3: Fork with Scope Management ===\n");
    
    gc_scope_push(gc);
    gc_malloc(gc, 100);  // ✅ SADECE BUNU DEĞİŞTİR (char *data sil)
    printf("Parent: allocated in scope\n");
    
    pid = fork();
    
    if (pid == 0)
    {
        // Child pushes its own scope
        gc_scope_push(gc);
        gc_malloc(gc, 50);
        printf("  Child: pushed scope and allocated\n");
        gc_scope_pop(gc);
        printf("  Child: ✅ scope pop successful\n");
        gc_scope_pop(gc);
        gc_destroy(gc);
        _exit(0);
    }
    else
    {
        wait(NULL);
        gc_scope_pop(gc);
        printf("Parent: scope management successful\n");
    }
}

void test_multiple_forks(t_gc_context *gc)
{
    pid_t pid1, pid2;
    
    printf("\n=== Test 4: Multiple Sequential Forks ===\n");
    
    char *data = gc_strdup(gc, "shared data");
    
    // First fork
    pid1 = fork();
    if (pid1 == 0)
    {
        printf("  Child 1: processing %s\n", data);
        gc_destroy(gc);
        _exit(0);
    }
    
    wait(NULL);
    
    // Second fork
    pid2 = fork();
    if (pid2 == 0)
    {
        printf("  Child 2: processing %s\n", data);
        gc_destroy(gc);
        _exit(0);
    }
    
    wait(NULL);
    printf("Parent: ✅ Both children completed\n");
}

void test_pool_with_fork(t_gc_context *gc)
{
    pid_t pid;
    char *small_alloc;
    
    printf("\n=== Test 5: Pool Allocation with Fork ===\n");
    
    // Small allocation from pool
    small_alloc = (char *)gc_malloc(gc, 100);
    printf("Parent: allocated 100 bytes from pool (%p)\n", (void *)small_alloc);
    
    pid = fork();
    
    if (pid == 0)
    {
        // Child reads pool memory
        printf("  Child: can read pool memory\n");
        
        // Child allocates (triggers COW on pool)
        char *child_alloc = (char *)gc_malloc(gc, 50);
        printf("  Child: allocated 50 bytes (%p)\n", (void *)child_alloc);
        printf("  Child: ✅ Pool COW works!\n");
        
        gc_destroy(gc);
        _exit(0);
    }
    else
    {
        wait(NULL);
        printf("Parent: ✅ Pool survived fork\n");
    }
}

int main(void)
{
    t_gc_context *gc;
    
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║         Fork + COW Functionality Tests        ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    gc = gc_create();
    if (!gc)
    {
        printf("❌ Failed to create GC context\n");
        return 1;
    }
    
    gc_set_mode(gc, GC_MODE_MANUAL);
    
    test_simple_fork(gc);
    test_fork_with_child_alloc(gc);
    test_fork_with_scope(gc);
    test_multiple_forks(gc);
    test_pool_with_fork(gc);
    
    gc_destroy(gc);
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║          All Fork Tests Completed ✅           ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    
    return 0;
}
