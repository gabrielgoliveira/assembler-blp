#include "../execution-context/execution_context.h"
#include "./interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


int recognize_line(ExecutionContext *c, char *line) {
  // vi1 = ci1

  int index_1, index_2;
  int r = sscanf(line, "vi%d = ci%d", &index_1, &index_2);

  // atribuicao de contante para variavel de pilha
  if(r == 2) {
    char variavel_pilha[10];
    char constante[10];

    // monta a string vi%d, basicamente concatena vi com o seu indice
    sprintf(variavel_pilha, "vi%d", index_1);
    sprintf(constante, "ci%d", index_2);

    char registrador_pilha[10] = "";
    char constante_value[10] = "";

    printf("## %s\n", constante);

    // sei que a contante nao precisava do context_get, mas quis exemplificar 
    context_get(c, variavel_pilha, registrador_pilha);
    context_get(c, constante, constante_value);

    printf("movl %s, %s\n", constante_value, registrador_pilha);
    // printf("movl %d, %s\n", index_2, registrador_pilha);

    return 1;
  }

  return -1;
}