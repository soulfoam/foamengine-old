#include <stdlib.h>
#include "stack.h"
#include "logger.h"

struct
Stack
{
    u32 *data;
    size_t capacity;
    size_t top;
    bool empty;
};

Stack*
stack_make(size_t capacity)
{
    Stack *s    = malloc(sizeof(*s));
    s->data     = malloc(sizeof(*s->data) * capacity);
    s->capacity = capacity;
    s->top      = 0;
    s->empty    = true;

    return s;
}

void
stack_destroy(Stack *s)
{
    free(s->data);
    free(s);
}

bool
stack_empty(Stack *s)
{
    return s->empty;
}

bool
stack_full(Stack *s)
{
    return s->top == s->capacity;
}

size_t
stack_capacity(Stack *s)
{
    return s->capacity;
}

size_t
stack_top(Stack *s)
{
    return s->top-1;
}

u32
stack_peek(Stack *s)
{
    return s->data[s->top-1];
}

void
stack_push(Stack *s, u32 val)
{
    if (stack_full(s)) 
    {
        log_debug("Failed to push full stack.");
        return;
    }
    
    s->empty = false;
    s->data[s->top++] = val;
}

u32
stack_pop(Stack *s)
{
    if (s->empty) 
    {
        log_debug("Failed to pop empty stack.");
        return 0;
    }

    size_t top = --s->top;
    if (s->top == 0) s->empty = true;

    return s->data[top];
}
