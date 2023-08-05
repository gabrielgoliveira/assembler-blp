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

    /*
      vai pegar o contexto da variavel e salvar em registrador_pilha
      Exemplo :

        variavel_pilha = "vi3"
        retorno em registrador_pilha => -4(%rbp)

      com esse retorno a gente so precisa montar o comando assembly
    */
   
    context_get(c, variavel_pilha, registrador_pilha);

    // sei que a contante nao precisava do context_get, mas quis exemplificar 
    context_get(c, constante, constante_value);

    printf("movl %s, %s\n", constante_value, registrador_pilha);
    // printf("movl %d, %s\n", index_2, registrador_pilha);

    return 1;
  }

  //retorno de constante
  r = sscanf(line, "return ci%d", &index_1);

  if(r == 1){
    printf("movl $%d, %%eax\n", index_1);
    printf("leave\nret\n");
    return 1;
  }
  //retorno variável
  char variavel_retorno[3];

  r = sscanf(line, "return %c%c%c", &variavel_retorno[0],&variavel_retorno[1],&variavel_retorno[2]);
  if(r == 3){
    char registrador_pilha[10] = "";
    context_get(c, variavel_retorno, registrador_pilha);
    printf("movl -%s, %%eax\n", registrador_pilha);
    printf("leave\nret\n");
    return 1;
  }



  return -1;
}