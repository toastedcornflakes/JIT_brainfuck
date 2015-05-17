#include <stdlib.h>
#include <assert.h>
#include <dbg.h>
#include <utils/stack.h>

static size_t DEFAULT_STACK_ELEMENTS = 8;

stack *stack_create(size_t element_size) {
	stack *s = malloc(sizeof(stack));
	check_mem(s);
	s->size = DEFAULT_STACK_ELEMENTS;
	s->used_size = 0;
	s->element_size = element_size;
	s->el = calloc(element_size, s->size);
	return s;

error:
	if(s) {
		stack_destroy(s);
	}
	return NULL;
}

static void stack_grow(stack *s) {
	s->size = 2 * s->size + 1;
	s->el = realloc(s->el, s->size * s->element_size);
}

static void stack_shrink(stack *s) {
	s->size = s->size / 2;
	assert(s->size >= s->used_size);
	s->el = realloc(s->el, s->size * s->element_size);
}

void stack_push(stack *s, void *elem) {
	if(s->size <= s->used_size) {
		stack_grow(s);
	}
	void *above_top = s->el  + s->used_size * s->element_size;
	memcpy(above_top, elem, s->element_size);
	s->used_size++;
}

void stack_pop(stack *s, void *pop_here) {
	if(stack_empty(s)) {
		return;
	}
	void *top = s->el + (s->used_size - 1) * s->element_size;
	memcpy(pop_here, top, s->element_size);
	s->used_size--;

	if (s->size > 2 * s->used_size) {
		stack_shrink(s);
	}
}

int stack_empty(stack *s) {
	return s->used_size == 0;
}

void stack_destroy(stack *s) {
	if (s->el) free(s->el);
	free(s);
}


static void stack_test_all() {
	stack *s = stack_create(sizeof(int));
	int a = 0x69;
	int b = 0xabad1dea;
	stack_push(s, &a);
	stack_push(s, &b);

	int c;
	stack_pop(s, &c);
	printf("popped c is %d and should be same as b, %d\n", c, b);
	stack_pop(s, &c);
	printf("popped c is %d and should be same as a, %d\n", c, a);
	stack_destroy(s);
}
