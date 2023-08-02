#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./execution-context/execution_context.h"
#include "./interpreter/interpreter.h"
#include "./constants/formats.h"
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
    return 0;
  }
  
  return 1;
}


int main() {
  // declaracao de variaveis
  
  int length = 256;
  int flag = 0, scope_def_locals_var = 0;
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

    if(is_function_declaration(line)) {
      // cria o contexto
      context_create(&context);
    
      scope_def_locals_var = 0;

      // aloca parametros nos registradores
      alloc_params(&context, line);

      if(is_verbose) {
        // mostra registradores alocados
        context_print_params(&context);
        // print_struct(&context);
      }

      continue;
    }

    // inicia escopo de definicao de variaveis
    if(strcmp(line, "def\n") == 0 || strcmp(line, "def") == 0) {
      // if(is_verbose) printf("# ========== Criando escopo para definicao de variaveis ============\n");
      scope_def_locals_var = 1;
      continue;
    }

    // finaliza escopo de definicao de variaveis
    if(strcmp(line, "enddef\n") == 0 || strcmp(line, "enddef") == 0) {
      // if(is_verbose) printf("# =========== Finalizando escopo para definicao de variaveis ==========\n");
      scope_def_locals_var = 0;
      
      // imprime o que foi alocado
      if(is_verbose) {
        // context_print_params(&context);
        context_print_vlocal_regs(&context);
        context_print_vlocal_stack(&context);
        // print_struct(&context);
      }
      
      continue;
    }
    
    // processa definicao de variaveis
    if(scope_def_locals_var) {
      context_alloc(&context, line);
      continue;
    }

    int r = -1;
    r = recognize_line(&context, line);

    if(!flag && r == -1) {
      printf("============================================\n");
      printf("Error: -1\n");
      printf("Diretitva nao reconhecida : %s\n", line);
      printf("============================================\n");
      //return 0;
    }
  }

  return 0;
}
