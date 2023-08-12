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

  char* env_verbose = getenv("verbose");
  
  // declara estrutura de controle de contexto
  ExecutionContext context;
  
  // verifica se esta no modo verbose e ativa o modo
  if(strcmp(env_verbose, "1") == 0) {
    is_verbose = 1;
    printf("# verbose mode: active\n");
  }

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
      // if(is_verbose) printf("# ========== Criando escopo para definicao de variaveis ============\n");
      scope_def_locals_var = 1;

      // if(is_verbose) {
      //   // mostra registradores alocados
      //   context_print_params(&context);
      // }
      
      continue;
    }

    // finaliza escopo de definicao de variaveis
    if(strcmp(line, "enddef\n") == 0 || strcmp(line, "enddef") == 0) {
      // if(is_verbose) printf("# =========== Finalizando escopo para definicao de variaveis ==========\n");
      scope_def_locals_var = 0;
      
      // aloca a pilha
      printf("subq X\n");

      // aloca as variaveis de registradores
      printf("\n# ========== REGISTRADORES E PARAMETROS ==========\n");
      context_print_params(&context);
      context_print_vlocal_regs(&context);
      printf("# ========== END  ===================================\n");

      // aloca as variaveis de pilha
      context_alloc_stack(&context); // aloca a pilha de acordo com as definicoes realizadas
      context_print_stack(&context); // printa a pilha
        
      continue;
    }
    
    // processa definicao de variaveis
    if(scope_def_locals_var) {
      context_alloc(&context, line);
      continue;
    }

    int r = -1;
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
