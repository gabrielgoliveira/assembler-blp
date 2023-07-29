#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// includes locais

#include "./map/map.h"

char registers[][4] = {
  "rax", "rbx", "rcx",
  "rdx", "rsi", "rdi",
  "r8",  "r9",  "r10",
  "r11", "r12", "r13",
  "r14", "r15"
};

// chamada => rbx, r12, r13, r14, r15 (verdes)

int registers_status[14] = {0};

void print_params_reg(char type, int index);
int get_size_param(char p);
int is_function_declaration(char *str);
int is_return(char *str);
int recognize_line(char *, int);

int main() {
  
  int length = 256;
  char line[length];

  // flags
  int scoped_def_flag = 0;
  int flag = 0;

  while (fgets(line, length, stdin) != NULL) {
    flag = is_function_declaration(line);

    if(flag) {
      // preparando para entrar no escopo da funcao
      for(int i = 0; i < 14; i++) {
        registers_status[i] = 0;
      }



    } else {
      if(strcmp(line, "def\n") == 0) {
        // entra no scope para definir as variaveis
        scoped_def_flag = 1;
        continue;

      } else if (strcmp(line, "enddef\n") == 0) {
        // sai do escopo de definicao de variaveis
        scoped_def_flag = 0;
        continue;
      }
      flag = recognize_line(line, scoped_def_flag);
    }

    if(strcmp(line, "end\n") == 0 || strcmp(line, "end") == 0) {
      printf("leave");
      printf("ret");
      printf("# ---------------------- FIM FUNCAO --------------------------------\n");
    }

    if(!flag) {
      printf("============================================\n");
      printf("Error: -1\n");
      printf("Diretitva nao reconhecida : %s\n", line);
      printf("============================================\n");
    }
  }

  return 0;
}

/*
  máximo quatro variáveis inteiras de pilha (int), quatro variáveis inteiras de registrador (int) ou 
  quatro arrays de inteiros (int[]) - totalizando, no máximo, doze variáveis locais.
*/

/*
  * A definição das variáveis inteiras de pilha iniciam com a palavra-chave var  o nome segue o padrão viN
  * A definição dos arrays inteiros iniciam com a palavra-chave vet, seguida do nome do array,
  seguido da palavra-chave size e então uma constante inteira informando o tamanho do array. O
  nome do array segue o padrão vaN (por exemplo, va3 ou va5).
  * Uma constante inteira tem o formato civ, onde v é o valor da constante, por exemplo,
  ci5 (5), ci-15 (-15), ci1024 (1024), ci-8273 (-8273), etc. No caso do tamanho dos arrays,
  sempre deve ser uma constante positiva não nula (> 0).

  * A definição de variável de registrador iniciam com a palavra-chave reg, seguida do nome da
  variável. 

*/


int recognize_line(char *line, int scoped_def_flag) {

  if(scoped_def_flag == 1) {
    
    // formaters
    char *fmt_var_pilha = "var vi%d";
    char *fmt_arr = "vet va%d size ci%d";
    char *fmt_reg = "reg ri%d";

    int index;
    // variaveis locais
    if(sscanf(line, fmt_reg, &index)) {

    }

  }

  return -1;
}


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

int get_size_param(char p) {
  if(!p) return 0;
  int size = 0;
  // inteiro
  if(p == 'i') size = 4;
  // array
  if(p == 'a') size = 8;

  return size;
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


int alloc_params_function(char *str, int *regs_status) {

  char type_param1, type_param2, type_param3; 
  int index_param1, index_param2, index_param3;

  int id_function;
  char standard[] = "function f%d p%c%d p%c%d p%c%d";
  
  // function f1 pi1 pa2
  int n_match = sscanf(str, standard, &id_function, &type_param1, &index_param1, &type_param2, &index_param2, &type_param3, &index_param3);
 
  printf(".globl f%d\nf%d:\n", id_function, id_function);
  printf("pushq %%rbp\n");
  printf("movq  %%rsp, %%rbp\n");

  int bytes_used = 0;
  int len = 0;
    
  switch (n_match) {
    case 1:
      // nenhum parametro
      printf("# params : none\n");
      break;

    case 3:
      // 1 parametro informado : rsi
      bytes_used = get_size_param(type_param1);
      print_params_reg(type_param1, index_param1);

      break;

    case 5:
      // 2 parametros informados : rsi, rdi
      bytes_used = 0;
      bytes_used += get_size_param(type_param1);
      bytes_used += get_size_param(type_param2);

      print_params_reg(type_param1, index_param1);
      print_params_reg(type_param2, index_param2);
      break;

    case 7:
      // 3 parametros informados : rsi, rdi, rdx
      bytes_used = 0;
      len = 0;
      len = get_size_param(type_param1);
      if(len == 4) {

      }
      bytes_used += len;
      len = get_size_param(type_param2);
      bytes_used += len;
      len = get_size_param(type_param3);
      bytes_used += len;

      print_params_reg(type_param1, index_param1);
      print_params_reg(type_param2, index_param2);
      print_params_reg(type_param3, index_param3);
      break;

    default:
      printf("ERRO: Padrao de parametros nao reconhecido !!");
      return -1;
      break;
  }

}

int is_return(char *str) {
  printf("leave");
  printf("ret");
}