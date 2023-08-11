#include "../execution-context/execution_context.h"
#include "./interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int contador_ifs = 0;
void chamada_de_funcao(ExecutionContext *c, char *p1);

int recognize_line(ExecutionContext *c, char *line) {
  // vi1 = ci1
  int index_1, index_2;
  char atr_c0, atr_c1, atr_c2, atr_cop, atr_c3, atr_c4;
  int atr_i0, atr_i1, atr_i2;
  int r = sscanf(line, "v%c%d = %c%c%d %c %c%c%d", &atr_c0, &atr_i0, &atr_c1, &atr_c2, &atr_i1, &atr_cop, &atr_c3, &atr_c4, &atr_i2);
  
  // atribuicao simples
  if(r == 5) {
    char variavel_pilha[20];
    char variavel_pilha2[20];
    char constante[20];
    char parametro[20];
    char registrador_pilha[20] = "";
    char registrador_pilha2[20] = "";
    char constante_value[20] = "";
    char parametro_value[20] = "";

    //variavel inteira
    if(atr_c0 == 'i') {
      sprintf(variavel_pilha, "v%c%d", atr_c0, atr_i0);
      //constante
      if(atr_c1 == 'c') {
        sprintf(constante, "ci%d", atr_i1);
        printf("## %s\n", constante);
        context_get(c, variavel_pilha, registrador_pilha);
        context_get(c, constante, constante_value);
        printf("movl %s, %s\n", constante_value, registrador_pilha);
        return 1;
      }
      else if(atr_c1 == 'p') {
        //parametro
        sprintf(parametro, "pi%d", atr_i1);
        printf("## %s\n", parametro);
        context_get(c, variavel_pilha, registrador_pilha);
        context_get(c, parametro, parametro_value);
        printf("movl %s, %s\n", parametro_value, registrador_pilha);
        return 1;
      }else {
        //variavel inteira
        if(atr_c2 == 'i') {
          sprintf(variavel_pilha2, "vi%d", atr_i1);
          printf("## %s\n", variavel_pilha2);
          context_get(c, variavel_pilha, registrador_pilha);
          context_get(c, variavel_pilha2, registrador_pilha2);
          printf("movl %s, %s\n", registrador_pilha2, registrador_pilha);
          return 1;
        }else {
          //registrador
          sprintf(variavel_pilha2, "vr%d", atr_i1);
          printf("## %s\n", variavel_pilha2);
          context_get(c, variavel_pilha, registrador_pilha);
          context_get(c, variavel_pilha2, registrador_pilha2);
          printf("movl %s, %s\n", registrador_pilha2, registrador_pilha);
          return 1;
        }
      }
    }
    
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
    printf("movl %s, %%eax\n", registrador_pilha);
    printf("leave\nret\n");
    return 1;
  }
  //IF
  char if_primeiro[3], if_segundo[3], comparacao[2];
  r = sscanf(line, "if %s %s %s", if_primeiro, comparacao, if_segundo);
  if(r == 3){
    char registrador_pilha[10] = "";
    char registrador_pilha2[10] = "";
    context_get(c, if_primeiro, registrador_pilha);
    context_get(c, if_segundo, registrador_pilha2);
    
    printf("cmpl %s, %s\n", registrador_pilha, registrador_pilha2);
    if(!strcmp(comparacao, "eq")){
      printf("jne end_inf");
    }
    if(!strcmp(comparacao, "ne")){
      printf("je end_inf");
    }
    if(!strcmp(comparacao, "lt")){
      printf("jge end_inf");
    }
    if(!strcmp(comparacao, "le")){
      printf("jg end_inf");
    }
    if(!strcmp(comparacao, "gt")){
      printf("jle end_inf");
    }
    if(!strcmp(comparacao, "ge")){
      printf("jl end_inf");
    }
    contador_ifs++;
    printf("%d\n", contador_ifs);
    return 1;
  }
  
  //ENDIF
  if(!strcmp(line, "endif\n")){
    printf("end_if%d:\n", contador_ifs);
    return 1;
  }
  int num_func, num_var;
  char tipo_variavel;
  char primeiro_param_func[3];
  char segundo_param_func[3];
  char terceiro_param_func[3];
  char registrador_pilha[10] = "";
  //CHAMADA DE FUNÇÃO
  r = sscanf(line, "v%c%d = call f%d %s %s %s", &tipo_variavel, &num_var, &num_func, primeiro_param_func, segundo_param_func, terceiro_param_func);

  if(r >= 3){
    char variavel_recebe[4] = "";
    sprintf(variavel_recebe, "v%c%d", tipo_variavel, num_var);
    
    if(r >= 4){
      chamada_de_funcao(c,primeiro_param_func);
      printf("%%rdi\n");
    }
    if(r >=5){
      chamada_de_funcao(c,segundo_param_func);
      printf("%%rsi\n");
    }
    if(r == 6){
      chamada_de_funcao(c,terceiro_param_func);
      printf("%%rdx\n");
    }

    context_get(c, variavel_recebe, registrador_pilha);
    printf("movl %%eax, %s\n", registrador_pilha);
  }

  return -1;
}

void chamada_de_funcao(ExecutionContext *c, char *p1){
  char registrador_constante[10] = "";
  
  if(p1[0] == 'c'){
    context_get(c, p1, registrador_constante);
    printf("movl $%s, ", registrador_constante);
    return;
  }

  context_get(c, p1, registrador_constante);
  printf("movl %s, ", registrador_constante);
}




