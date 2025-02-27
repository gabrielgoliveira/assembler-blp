#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./execution-context/execution_context.h"
#include "./interpreter/interpreter.h"
#include "./constants/formats.h"
#include "./stack/stack.h"
int is_verbose = 0;

int is_function_declaration(char *str) {
  // pi_N =>  parâmetro inteiro
  // pa_N =>  parâmetro de array inteiro

  char type_param1, type_param2, type_param3; 
  int index_param1, index_param2, index_param3;

  int id_function;
  char standard[] = "function f%d p%c%d p%c%d p%c%d";
  
  // function f1 pi1 pa2
  int n_match = sscanf(str, standard, &id_function, &type_param1, &index_param1, &type_param2, &index_param2, &type_param3, &index_param3);
 
  if(n_match == 0) {
    return -1;
  }
  
  return id_function;
}


int main() {
  // declaracao de variaveis
  
  int length = 256;
  int flag = 0, scope_def_locals_var = 0;
  int id_function = 0, temp = 0;
  char line[length];

  
  // declara estrutura de controle de contexto
  ExecutionContext context;

  is_verbose = 1;

  while (fgets(line, length, stdin) != NULL) {
    // if(is_verbose) printf("# line : %s\n", line);
    temp = is_function_declaration(line);
    if(temp != -1) {
      // cria o contexto
      id_function = temp;
      context_create(&context);
    
      scope_def_locals_var = 0;

      // aloca parametros nos registradores
      alloc_params(&context, line);

      printf(".globl f%d\nf%d:\n", id_function, id_function);
      printf("pushq %%rbp\n");
      printf("movq  %%rsp, %%rbp\n");

      continue;
    }

    // inicia escopo de definicao de variaveis
    if(strcmp(line, "def\n") == 0 || strcmp(line, "def") == 0) {
      scope_def_locals_var = 1;
      continue;
    }

    // finaliza escopo de definicao de variaveis
    if(strcmp(line, "enddef\n") == 0 || strcmp(line, "enddef") == 0) {
      scope_def_locals_var = 0;
      

      // aloca as variaveis de pilha
      context_alloc_stack(&context); // aloca a pilha de acordo com as definicoes realizadas
      

      int size_stack = context_get_stack_size(&context);
      printf("subq $%d, %%rbp\n", size_stack);

      // print_stack_s(context.stack);
      // printf("Size : %d\n", context.stack->size);
      // aloca a pilha
      

      // print do contexto total
      printf("\n# ========== REGISTRADORES E PARAMETROS ==========\n");
      context_print_params(&context);
      context_print_vlocal_regs(&context);
      printf("# ========== END  ===================================\n");

      context_print_stack(&context); // printa a pilha


      continue;
    }
    
    // processa definicao de variaveis
    if(scope_def_locals_var) {
      context_alloc(&context, line);
      continue;
    }

    int r = -1;
    // printf("=====> line: %s\n", line);
    r = recognize_line(&context, line);

    // if(!flag && r == -1) {
    //   printf("============================================\n");
    //   printf("Error: -1\n");
    //   printf("Diretitva nao reconhecida : %s\n", line);
    //   printf("============================================\n");
    //   //return 0;
    // }
  }

  return 0;
}
