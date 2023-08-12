#include "stack.h"
#include "../constants/types_interpreter.h"

#include <stdlib.h>
#include <stdio.h>

Stack* stack_create() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) {
        printf("Erro ao alocar memória para a pilha.\n");
        exit(1);
    }
    stack->top = NULL;
    stack->base = NULL;
    stack->size = 0;
    return stack;
}

void stack_push(Stack* stack, int type, int len, int index, int size_array) {
    StackElement* newElement = (StackElement*)malloc(sizeof(StackElement));
    if (!newElement) {
        printf("Erro ao alocar memória para um novo elemento da pilha.\n");
        exit(1);
    }
    newElement->type = type;
    newElement->len = len;
    newElement->index = index;
    newElement->size_array = size_array;
    newElement->next = stack->top;

    // printf("type %d\n", newElement->type);
    // printf("len %d\n", newElement->len);
    // printf("index %d\n", newElement->index);
    // printf("size arr %d\n", newElement->size_array);
    // printf("type %d\n", newElement->size_array);
    stack->top = newElement;
    stack->size++;

    if (stack->size == 1) {
        stack->base = newElement;
    }
}

void stack_pop(Stack* stack) {
    if (!stack->top) {
        printf("Erro: pilha vazia.\n");
        return;
    }

    StackElement* temp = stack->top;
    stack->top = stack->top->next;
    stack->size--;

    if (!stack->top) {
        stack->base = NULL;
    }

    free(temp);
}

StackElement* stack_peek(Stack* stack) {
    return stack->top;
}

int stack_isEmpty(Stack* stack) {
    return stack->top == NULL;
}

void stack_destroy(Stack* stack) {
    StackElement* temp = stack->top;

    while (temp) {
        StackElement* elementToRemove = temp;
        temp = temp->next;
        free(elementToRemove);
    }

    free(stack);
}

void stack_print_element(StackElement *element, int pos) {
  if(element->type == ID_TYPE_VAR_LOCAL_STACK) {
    // variavel local de pilha

    int index = element->index;
    int pos_stack = (pos + 1)*4;
    printf("## vi%d => -%d(%%rbp)\n\n", index, pos_stack);

  } else if (element->type == ID_TYPE_PARAMS) {
    int index = element->index;
    int pos_stack = (pos + 1)*4;
    printf("## pi%d => -%d(%%rbp)\n\n", index, pos_stack);
  }
}