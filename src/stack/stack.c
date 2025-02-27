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

void stack_push(Stack* stack, int type, int len, int index, int size_array, int pos_array) {
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
    newElement->index_array = pos_array;

    StackElement* oldElement = stack->top;

    if (stack->size == 0) {
      stack->base = newElement;
      newElement->next = NULL;
      newElement->prev = NULL;
      
      // define a posicao na pilha
      if(newElement->type < 10) {
        newElement->pos_stack = 4;
      } else if (newElement->type == ID_TYPE_ARR_LOCAL){
        newElement->pos_stack = size_array;
      } else {
        newElement->pos_stack = 8;
      }

    } else if (stack->size >= 1) {
      newElement->prev = stack->top;
      oldElement->next = newElement;

      // define a posicao na pilha
      int pos_stack_old = oldElement->pos_stack;

      if(newElement->type < 10) {
        newElement->pos_stack = 4 + pos_stack_old;
      } else if (newElement->type == ID_TYPE_ARR_LOCAL){
        newElement->pos_stack = pos_stack_old + size_array;
      } else {
        newElement->pos_stack = 8 + pos_stack_old;
      }


    }

    stack->top = newElement;
    stack->size++;
}

void stack_pop(Stack* stack) {
    if (!stack->top) {
        printf("Erro: pilha vazia.\n");
        return;
    }

    StackElement* temp = stack->top;
    stack->top = stack->top->prev;
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

}

void print_stack_s(Stack *s) {
  StackElement *element = s->base;
  for (int i = 0; i < s->size; i++) {
    if(element == NULL) return;
    printf("\n\n");
    printf("type %d\n", element->type);
    printf("len %d\n", element->len);
    printf("index %d\n", element->index);
    printf("size_array %d\n", element->size_array);
    printf("pos_stack %d\n", element->pos_stack);
    element = element->next;
  }
}