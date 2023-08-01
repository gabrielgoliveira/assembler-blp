#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_verbose = 0;

typedef struct {
  int *vector;
} control_vars;

void print_params_reg(char type, int index) {
  char registers_params[][4] = {"rdi", "rsi", "rdx"};

  int pos = index - 1;
  char current_register[4];

  if(type == 'a') {
    // array
    strcpy(current_register, registers_params[pos]);
    printf("# param p%c%d [%d] -> register %s\n",type, index, index, current_register);
  }

  if(type == 'i') {
    // inteiro
    strcpy(current_register, registers_params[pos]);
    current_register[0] = 'e';
    printf("# param p%c%d [%d] -> register %s\n",type, index, index, current_register);

  }

  return ;
}

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


int get_size_param(char p) {
  if(!p) return 0;
  int size = 0;
  // inteiro
  if(p == 'i') size = 4;
  // array
  if(p == 'a') size = 8;

  return size;
}

int alloc_params_function(char *str, int *arr) {

  char type_param1, type_param2, type_param3; 
  int index_param1, index_param2, index_param3;

  int id_function;
  char standard[] = "function f%d p%c%d p%c%d p%c%d";
  
  // function f1 pi1 pa2
  int n_match = sscanf(str, standard, &id_function, &type_param1, &index_param1, &type_param2, &index_param2, &type_param3, &index_param3);
 
  // aloca o RA
  printf(".globl f%d\nf%d:\n", id_function, id_function);
  printf("pushq %%rbp\n");
  printf("movq  %%rsp, %%rbp\n");

  int bytes_used = 0;

  switch (n_match) {
    case 1:
      // nenhum parametro
      if(is_verbose) printf("# params : none\n");
      break;

    case 3:
      // 1 parametro informado : rsi
      bytes_used = get_size_param(type_param1);
      arr[0] = bytes_used;

      if(is_verbose) print_params_reg(type_param1, index_param1);

      break;

    case 5:
      // 2 parametros informados : rsi, rdi
      bytes_used = get_size_param(type_param1);
      arr[0] = bytes_used;

      bytes_used = get_size_param(type_param2);
      arr[1] = bytes_used;

      if(is_verbose) {
        print_params_reg(type_param1, index_param1);
        print_params_reg(type_param2, index_param2);
      }

      break;

    case 7:
      // 3 parametros informados : rsi, rdi, rdx

      bytes_used = get_size_param(type_param1);
      arr[0] = bytes_used;
      
      bytes_used = get_size_param(type_param2);
      arr[1] = bytes_used;

      bytes_used = get_size_param(type_param3);
      arr[2] = bytes_used;

      if(is_verbose) {
        print_params_reg(type_param1, index_param1);
        print_params_reg(type_param2, index_param2);
        print_params_reg(type_param3, index_param3);
      }

      break;

    default:
      printf("ERRO: Padrao de parametros nao reconhecido !!");
      return -1;
      break;
  }

}


// void recognize_local_variable(char *str) {

//     return -1;
// }

int inserir_vetor(int *arr, int len, int valor) {
  for(int i = 0; i < len; i++) {
    if(arr[i] == -1) {
      arr[i] = valor;
      return 1;
    }
  }

  return -1;
} 


int main() {
  
  int length = 256;
  char line[length];

  // variaveis de controle
  int flag = 0, scope_def_locals_var = 0;

  // %rdi %rsi e %rdx.
  int reg_params[3];

  // guardar o indice da variavel
  int var_int_stack_index[4];

  // guardar o indice da variavel
  int var_int_reg_index[4];

  // guardar o indice da variavel
  int arr_int_index[4];

  char* env_verbose = getenv("verbose");
  
  if(strcmp(env_verbose, "1") == 0) {
    is_verbose = 1;
    printf("# verbose mode active\n");
  }

  while (fgets(line, length, stdin) != NULL) {
    if(is_verbose) printf("# line : %s\n", line);
    flag = is_function_declaration(line);
    
    if(flag) {

      // inicializa variaveis de controle

      for (int i = 0; i < 4; i++) {
        var_int_stack_index[i] = -1;
        var_int_reg_index[i] = -1;
        arr_int_index[i] = -1;
      }

      for(int i = 0; i < 3; i++) {
        reg_params[i] = -1;
      }

      scope_def_locals_var = 0;

      // aloca parametros nos registradores
      alloc_params_function(line, reg_params);

      if(is_verbose) {
        // mostra registradores alocados
        printf("# ");
        for(int i = 0; i < 3; i++) {
          printf("%d ", reg_params[i]);
        }
        printf("\n");
      }


      continue;
    }

    // definicao de variaveis
    if(strcmp(line, "def\n") == 0 || strcmp(line, "def") == 0) {
      if(is_verbose) printf("# ========== Criando escopo para definicao de variaveis ============\n");
      scope_def_locals_var = 1;
      continue;
    }

    // fim da definicao
    if(strcmp(line, "enddef\n") == 0 || strcmp(line, "enddef") == 0) {
      if(is_verbose) printf("# ========= Finalizando escopo para definicao de variaveis ==========\n");
      scope_def_locals_var = 0;
      
      for(int i = 0; i < 4; i++) {
        if(var_int_reg_index[i] != -1) {
          if(is_verbose) printf("# alocacao ri%d\n", var_int_reg_index[i]);
        }

        if(var_int_stack_index[i] != -1) {
          if(is_verbose) printf("# alocacao var%d\n", var_int_stack_index[i]);
        }
      }

      continue;
    }

    // quando estou no escopo de definicao de variaveis
    if(scope_def_locals_var) {

      // identifica e aloca variaveis
      // recognize_local_variable(line)
      char *fmt_var_pilha = "var vi%d";
      char *fmt_arr = "vet va%d size ci%d";
      char *fmt_reg = "reg ri%d";

      int index = 0;

      int n_match = 0;
      char *str = line;

      n_match = sscanf(str, fmt_reg, &index);

      if(n_match == 1) {
        // variaveis de registrador
        if(is_verbose) printf("# %s# eh variavel de registrador\n", line);
        inserir_vetor(var_int_reg_index, 4, index);
      }

      n_match = sscanf(str, fmt_var_pilha, &index);

      if(n_match == 1) {
        // variaveis de pilha
        printf("# %s# eh variavel de pilha\n", line);
        inserir_vetor(var_int_stack_index, 4, index);
      }

      // n_match = sscanf(str, fmt_reg, &index);

      continue;
    }

    
    // fora do escopo, aqui vale tudo !!

    /*
      (i) atribuição de variável inteira, 
          * uma atribuição simples (Uma atribuição simples pode ser uma variável inteira recebendo o valor de outra variável, um parâmetro inteiro ou uma constante), 
          * uma expressão ( Uma expressão pode ser as operações de soma, subtração, multiplicação ou divisão, sendo que os operandos podem ser variáveis inteiras, parâmetros inteiros ou constante)
          * retorno de uma chamada de função
          * 
          * uma variável inteira pode receber o retorno de uma chamada de função.
          
      (ii) alteração de uma posição do array, 
      (iii) recuperação de valor de uma posição de um array, 
      (iv) condicional if 
      (v) returno de um valor.
    
    */


    // 1. variável inteira recebendo o valor de outra variável
    //  posibilidades

    int origem, destino, r;

    // =========== Variaveis de pilha ====================

    r = sscanf(line, "vi%d = vi%d", &destino, &origem);
    if(r == 2) {
      if(is_verbose) printf("# atribuicao de variavel de pilha para variavel de pilha\n");
      continue;
    }
    // variavel de pilha para variavel de registrador
    r = sscanf(line, "vi%d = ri%d", &destino, &origem);
    if(r == 2) {
      /*
        1. buscar posicao da variavel na pilha
        2. movl variavel de pilha para registrador
        3. movl registrador para outra variavel de pilha

        Como a pilha esta em memoria nao podemos fazer pilha -> pilha ou podemos ???
      */
      if(is_verbose) printf("# atribuicao de registrador para variavel de pilha\n");
      continue;
    }

    r = sscanf(line, "vi%d = ci%d", &destino, &origem);
    if(r == 2) {
      if(is_verbose) printf("# atribuicao de constante para variavel de pilha\n");
      continue;
    }

    // =========== Variaveis de registrador ====================
    // variavel de registrador para variavel de registrador
    
    r = sscanf(line, "ri%d = ri%d", &destino, &origem);
    if(r == 2) {
      /*
        1. Buscar o registrador da variavel
        2. movl registrador para registrador
      */
      printf("movl reg1, reg2");
      continue;
    }
    // variavel de pilha para variavel de registrador
    r = sscanf(line, "ri%d = vi%d", &destino, &origem);
    if(r == 2) {
      if(is_verbose) printf("# atribuicao de variavel de pilha para variavel de registrador\n");
      continue;
    }

    if(strcmp(line, "end\n") == 0 || strcmp(line, "end") == 0) {
      printf("leave");
      printf("ret");
      printf("# ---------------------- FIM FUNCAO --------------------------------\n");
      continue;
    }

    if(!flag) {
      printf("============================================\n");
      printf("Error: -1\n");
      printf("Diretitva nao reconhecida : %s\n", line);
      printf("============================================\n");
      //return 0;
    }
  }

  return 0;
}
