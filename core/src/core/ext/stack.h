#pragma once

#include "sugar.h"

typedef struct Stack Stack;

Stack* stack_make(size_t capacity);
void   stack_destroy(Stack *s);
bool   stack_empty(Stack *s);
bool   stack_full(Stack *s);
size_t stack_capacity(Stack *s);
size_t stack_top(Stack *s);
u32    stack_peek(Stack *s);
void   stack_push(Stack *s, u32 val);
u32    stack_pop(Stack *s);
