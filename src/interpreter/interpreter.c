#include "../execution-context/execution_context.h"
#include "../constants/types_interpreter.h"
#include "./interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int contador_ifs = 0;
void chamada_de_funcao(ExecutionContext *c, char *p1);
int if_call_function(ExecutionContext *c, char *line);

char reg_params_name[][4] = {"rdi", "rsi", "rdx"};
char nomes_regs_var[][4] = {"r11", "r10", "r9", "r8"};

int recognize_line(ExecutionContext *c, char *line) {
  int index_1, index_2;
  char atr_c0, atr_c1, atr_c2, atr_cop, atr_c3, atr_c4;
  int atr_i0, atr_i1, atr_i2;
  int r = sscanf(line, "v%c%d = %c%c%d %c %c%c%d", &atr_c0, &atr_i0, &atr_c1, &atr_c2, &atr_i1, &atr_cop, &atr_c3, &atr_c4, &atr_i2);

  int flag = 0;
  flag = if_call_function(c, line);
  if(flag == 1) return 0;

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
        printf("movl $%s, %s\n", constante_value, registrador_pilha);
        return 1;
      }
      else if(atr_c1 == 'p') {
        //parametro
        sprintf(parametro, "pi%d", atr_i1);
        // printf("## %s\n", parametro);
        context_get(c, variavel_pilha, registrador_pilha);
        context_get(c, parametro, parametro_value);
        printf("movl %%%s, %s\n", parametro_value, registrador_pilha);
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
          //registrador, verificar essa parte ainda
          sprintf(variavel_pilha2, "vr%d", atr_i1);
          printf("## %s\n", variavel_pilha2);
          context_get(c, variavel_pilha, registrador_pilha);
          context_get(c, variavel_pilha2, registrador_pilha2);
          printf("movl %s, %s\n", registrador_pilha2, registrador_pilha);
          return 1;
        }
      }
    }else{ //registrador
      sprintf(variavel_pilha, "vr%d", atr_c0, atr_i0);
      //constante
      if(atr_c1 == 'c') {
        sprintf(constante, "ci%d", atr_i1);
        printf("## %s\n", constante);
        context_get(c, variavel_pilha, registrador_pilha);
        context_get(c, constante, constante_value);
        printf("movl $%s, %s\n", constante_value, registrador_pilha);
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
        } else {
          //registrador, verificar essa parte ainda
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

  //Operações
  if(r == 9){
    char variavel_pilha[20];
    char variavel_pilha2[20];
    char registrador[20];
    char registrador2[20];
    char constante[20];
    char constante2[20];
    char parametro[20];
    char parametro2[20];
    char registrador_pilha[20] = "";
    char registrador_pilha2[20] = "";
    char registrador_value[20] = "";
    char registrador_value2[20] = "";
    char constante_value[20] = "";
    char constante_value2[20] = "";
    char parametro_value[20] = "";
    char parametro_value2[20] = "";
    
    sprintf(variavel_pilha, "v%c%d", atr_c0, atr_i0);
    context_get(c, variavel_pilha, registrador_pilha);

    if(atr_c1 == 'c'){
      sprintf(constante, "ci%d", atr_i1);
      context_get(c, constante, constante_value);
      printf("movl $%s, %s\n", constante_value, registrador_pilha);
    }
    else if(atr_c1 == 'p'){
      sprintf(parametro, "pi%d", atr_i1);
      context_get(c, parametro, parametro_value);
      printf("movl %%%s, %s\n", parametro_value, registrador_pilha);
    }else{
      sprintf(registrador, "v%c%d", atr_c2, atr_i1);
      context_get(c, registrador, registrador_value);
      if(strcmp(registrador_value, registrador_pilha))
        printf("movl %s, %s\n", registrador_value, registrador_pilha);
		}
    //Adição
    if(atr_cop == '+'){
      if(atr_c3 == 'c'){
			  printf("addl $%d, %s\n", atr_i2, registrador_pilha);
      }
      else if(atr_c3 == 'p'){
        sprintf(parametro2, "pi%d", atr_i2);
        context_get(c, parametro2, parametro_value2);
        printf("addl %s, %s\n", parametro_value2, registrador_pilha);
      }else{
        sprintf(registrador2, "v%c%d", atr_c4, atr_i2);
        context_get(c, registrador2, registrador_value2);
        printf("addl %s, %s\n", registrador_value2, registrador_pilha);
      }
		}

    //Subtração
    else if(atr_cop == '-'){
      if(atr_c3 == 'c'){
        printf("subl $%d, %s\n", atr_i2, registrador_pilha);
      }
      else if(atr_c3 == 'p'){
        sprintf(parametro2, "pi%d", atr_i2);
        context_get(c, parametro2, parametro_value2);
        printf("subl %%%s, %s\n", parametro_value2, registrador_pilha);
      }else{
        sprintf(registrador2, "v%c%d", atr_c4, atr_i2);
        context_get(c, registrador2, registrador_value2);
        printf("subl %s, %s\n", registrador_value2, registrador_pilha);
      }
    }
    //Multiplicação
    else if(atr_cop == '*'){
      if(atr_c3 == 'c'){
        printf("imull $%d, %s\n", atr_i2, registrador_pilha);
        return 1;
      }
      else if(atr_c3 == 'p'){
        sprintf(parametro2, "pi%d", atr_i2);
        context_get(c, parametro2, parametro_value2);
        printf("imull %s, %s\n", parametro_value2, registrador_pilha);
      }else{
        sprintf(registrador2, "v%c%d", atr_c4, atr_i2);
        context_get(c, registrador2, registrador_value2);
        printf("imull %s, %s\n", registrador_value2, registrador_pilha);
      }
    }else{
      if(atr_c3  == 'c'){
        printf("movl $%d, %%ecx\nmovl %s, %%eax\ncltd\nidivl %%ecx\n", atr_i2, registrador_pilha);
        printf("movl %%eax, %s\n", registrador_pilha);
        return 1;
      }
      else if(atr_c3 == 'p'){
        sprintf(parametro, "pi%d", atr_i2);
        context_get(c, parametro2, parametro_value2);
        printf("movl %%%s, %%ecx\nmovl %s, %%eax\ncltd\nidivl %%ecx\n", parametro_value2, registrador_pilha);
        printf("movl %%eax, %s\n", registrador_pilha);
      }else{
        sprintf(registrador2, "v%c%d", atr_c4, atr_i2);
        context_get(c, registrador2, registrador_value2);
        printf("movl %s, %%ecx\nmovl %s, %%eax\ncltd\nidivl %%ecx\n", registrador_value2, registrador_pilha);
        printf("movl %%eax, %s\n", registrador_pilha);
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
  char if_primeiro[4], if_segundo[4], comparacao[2];
  r = sscanf(line, "if %s %s %s", if_primeiro, comparacao, if_segundo);
  if(r == 3){
    char registrador_pilha[10] = "";
    char registrador_pilha2[10] = "";
    context_get(c, if_primeiro, registrador_pilha);
    context_get(c, if_segundo, registrador_pilha2);

    printf(if_primeiro[0] == 'c' && if_segundo[0] == 'c'?
            "cmpl $%s, $%s\n":
            if_primeiro[0] == 'c'?
            "cmpl %s, $%s\n":
            if_segundo[0] == 'c'?
            "cmpl $%s, %s\n" :
            "cmpl %s, %s\n", registrador_pilha2, registrador_pilha);
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

  //Operacoes GET TO
  char tipo_da_variavel, tipo_variavel_destino;
  int num_da_variavel, num_do_index, num_da_variavel_destino;

  r = sscanf(line, "get v%c%d index ci%d to %ci%d", &tipo_da_variavel, &num_da_variavel, &num_do_index, &tipo_variavel_destino, &num_da_variavel_destino);
  if(r == 5){

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

// imprimir atribuicao de parametros para os registradores
void print_att_params(ExecutionContext *c, char *param, int index) {
  int pos_stack;
  // char reg_params_name[][4] = {"rdi", "rsi", "rdx"};
  if(param != NULL) {
    pos_stack = context_get_element_stack(c, param, NULL); // pega a posicao na pilha
    if(pos_stack == -1) {
      printf("ERRO: VARIAVEL NAO LOCALIZADA !!!!! [%s]\n", param);
      return ;
    }
    printf("movq -%d(%%rbp), %%%s\n", pos_stack, reg_params_name[index-1]);
  }
}

void context_recover(ExecutionContext *c, int count) {
  StackElement *element;

  for (int i = 0; i < count; i++) {
    element = c->stack->top;
    
    switch (element->type) {
      case ID_TYPE_PARAMS:
        printf("mov -%d(%%rbp), %s\n", element->pos_stack, reg_params_name[element->index_array]);
        break;
      
      case ID_TYPE_VAR_LOCAL_REG:
        printf("mov -%d(%%rbp), %s\n", element->pos_stack, nomes_regs_var[element->index_array]);
        break;

      default:
        printf("[ERROR]\n");
        break;
    }

    
    stack_pop(c->stack);
  }
}

int if_call_function(ExecutionContext *c, char *line) {
  int index_function;
  char param1[7], param2[7], param3[7];
  char *str_call_function = "call f%d %s %s %s";

  int n_match = sscanf(line, str_call_function, &index_function, param1, param2, param3);
  
  if(n_match == 0) return 0;

  /*
    1. AO CHAMAR UMA FUNCAO ALOCAR PARAMETROS NOS REGISTRADORES
  */
  int count = 0;
  context_save(c, &count);    // salva tudo na pilha (variavel de registrador, parametros ....)
  context_print_stack(c);     // imprimir a pilha

  // printf("==================================\n");
  // print_stack_s(c->stack);
  // printf("==================================\n");
  
  int pos_stack;
  switch (n_match)
  {
  case 1: /* nenhum parametro */
  
    printf("call f%d\n", index_function);
    break;
  
  case 2: /* 1 parametro */
    
    // pos_stack = context_get_element_stack(c, param1, NULL);
    // if(pos_stack == -1) {
    //   printf("ERRO: VARIAVEL NAO LOCALIZADA !!!!! [%s]\n", param1);
    // }
    // printf("movq -%d(%%rbp), %%rdi\n", pos_stack);

    // resgata os parametros na pilha e move para os seus respectivos registradores
    print_att_params(c, param1, 1);
    printf("call f%d\n", index_function);
  
    break;
  
  case 3: /* 2 parametros */
    
    print_att_params(c, param1, 1);
    print_att_params(c, param2, 2);

    printf("call f%d\n", index_function);

    break;
  
  case 4:
    /* 3 parametros */
    print_att_params(c, param1, 1);
    print_att_params(c, param2, 2);
    print_att_params(c, param3, 3);

    printf("call f%d\n", index_function);

    break;
  
  default:
    break;
  }

  context_recover(c, count);

  return 1;

}

