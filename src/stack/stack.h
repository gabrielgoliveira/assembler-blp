#ifndef STACK_H
#define STACK_H

typedef struct StackElement {
    int type;
    int len;
    int index;
    int size_array;
    int pos_stack;
    int index_array;
    struct StackElement* prev;
    struct StackElement* next;
} StackElement;

typedef struct Stack {
    StackElement* top;
    StackElement* base;
    int size;
} Stack;

Stack* stack_create();
void stack_push(Stack* stack, int type, int len, int index, int size_array, int pos_array);
void stack_pop(Stack* stack);
StackElement* stack_peek(Stack* stack);
int stack_isEmpty(Stack* stack);
void stack_destroy(Stack* stack);
void stack_print_element(StackElement *element, int pos);
void print_stack_s(Stack *s);

#endif // STACK_H

