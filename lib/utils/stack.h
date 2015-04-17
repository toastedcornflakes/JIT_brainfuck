#ifndef _stack_h_
#define _stack_h_

typedef struct {
	size_t size;
	size_t used_size;
	size_t element_size;
	void *el;
} stack;

stack *stack_create(size_t element_size);
void stack_push(stack *s, void *elem);

void stack_pop(stack *s, void *pop_here);
int stack_empty(stack *s);

void stack_destroy(stack *s);

#endif
