#ifndef __EXECUTION_CONTEXT_H__
#define __EXECUTION_CONTEXT_H__

#include "../stack/stack.h"

typedef struct {
  int *var_int_stack_index;   // Array de indice da variavel de pilha 
  int *var_int_reg_index;     // Array de indice de variavel de registrador
  int *arr_int_size;          // Guarda o tamanho do array
  int *arr_int_index;         // Array de indices de arrays
  int *reg_params;            // Array de parametros, cada posicao guarda o tamanho 4 ou 8
  int init;                   // Se a estrutura foi iniciada
  Stack *stack;               // Ponteiro para a pilha
} ExecutionContext;

// cria estrutura de dados para guardar o contexto da funcao
void context_create(ExecutionContext* c);

// libera a estrutura
void context_free(ExecutionContext* c);

// busca uma posicao livre no vetor e insere o valor
int alloc_vector(int *vector, int len, int value);

// aloca variaveis locais de funcao
void context_alloc(ExecutionContext* c, char *str);

// busca e retorna o indice de um elemento no vetor
int _indexof(int *arr, int len, int value);

// recupera o contexto de uma variavel
void context_get(ExecutionContext* c, char *str, char *dest);

// retorna o tamanho de um parametro em bytes
int _get_size_param(char p);

// aloca registradores para os parametros de uma funcao
int alloc_params(ExecutionContext* c, char *str);

// processa e aloca memoria/registradores para declaracao de variaveis
void context_process_def_local_var(ExecutionContext *c, char *str);

// printa como os parametros foram alocados em relacao aos registradores
void context_print_params(ExecutionContext* c);

// printa como as variaveis locais foram alocadas em relacao aos registradores
void context_print_vlocal_regs(ExecutionContext* c);

// printa como as variaveis de pilha foram alocadas
void context_print_vlocal_stack(ExecutionContext* c);

// printa tudo
void print_struct(ExecutionContext* c);

void context_alloc_stack(ExecutionContext* c);
void context_print_stack(ExecutionContext* c);

void context_save(ExecutionContext* c, int *count);

int context_get_element_stack(ExecutionContext* c, char *str, char *dest_result);

int context_get_stack_size(ExecutionContext* c);

#endif //__EXECUTION_CONTEXT_H__