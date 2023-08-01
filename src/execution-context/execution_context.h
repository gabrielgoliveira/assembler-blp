#ifndef __EXECUTION_CONTEXT_H__
#define __EXECUTION_CONTEXT_H__

typedef struct {
  int *var_int_stack_index;
  int *var_int_reg_index;
  int *arr_int_index;
  int *reg_params;
  int init;
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
void context_get(ExecutionContext* c, char *str);

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

#endif //__EXECUTION_CONTEXT_H__