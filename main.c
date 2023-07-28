#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getSizeParam(char *type) {
  char standard_int[] = "pi%d";
  char standard_pointer[] = "pa%d";
  int  index_param;
  char registers[][4] = {"rdi", "rsi", "rdx"};

  int n_match = sscanf(type, standard_int, &index_param);

  if(n_match) {
    int pos_reg = index_param - 1;
    char current_register[4];

    strcpy(current_register, registers[pos_reg]);
    current_register[0] = 'e';

    printf("# param [%d] -> register %s\n", index_param, current_register);
    return ;
  }

  n_match = sscanf(type, standard_pointer, &index_param);
  if(n_match) {
    // pointer
    int pos_reg = index_param - 1;
    printf("# param [%d] -> register %s\n", index_param, registers[pos_reg]);
    return ;
  }
}

int isFunctionDeclaration(char *str) {
  
  char param1[4];
  char param2[4];
  char param3[4];
  char name_function[4];
  char standard[] = "function %s %s %s %s";

  int n_match = sscanf(str, standard, name_function, param1, param2, param3);

  if(n_match == 0) {
    return 0;
  }

  printf(".globl %s\n%s:\n", name_function, name_function);
  printf("pushq %%rbp\n");
  printf("movq  %%rsp, %%rbp\n");

  if(n_match == 1) {
    printf("# params : none\n");
  }

  if(n_match == 2) {
    printf("# params (1): %s\n", param1);
    getSizeParam(param1);
  }

  if(n_match == 3) {
    printf("# params (2): %s %s\n", param1, param2);
    getSizeParam(param1);
    getSizeParam(param2);
  }

  if(n_match == 4) {
    printf("# params (3): %s %s %s\n", param1, param2, param3);
    getSizeParam(param1);
    getSizeParam(param2);
    getSizeParam(param3);
  }

  return 1;
}


int main() {
  int flag = 0;
  int length = 256;
  char line[length];

  while (fgets(line, length, stdin) != NULL) {
    flag = isFunctionDeclaration(line);

    if(flag) {
      flag = 0;
      continue;
    }

    

  }

  return 0;
}