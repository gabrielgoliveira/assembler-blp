#include "../constants/formats.h"
#include "./execution_context.h"
#include "../stack/stack.h"
#include "../constants/types_interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Como temos apenas 4 variaveis de registrador,
a alocacao se da de forma que :

  primeira var de reg : r11
  segunda var de reg  : r10
  terceira var de reg : r9
  terceira var de reg : r8

*/

void context_create(ExecutionContext* c) {
  c->var_int_stack_index = (int*)malloc(sizeof(int)*4);
  c->arr_int_index = (int*)malloc(sizeof(int)*4);
  c->arr_int_size = (int*)malloc(sizeof(int)*4);
  c->var_int_reg_index = (int*)malloc(sizeof(int)*4);
  c->reg_params = (int*)malloc(sizeof(int)*3);
  c->stack = stack_create();
  c->init = 1;

  // inicializa as variaveis de controle
  for(int i = 0; i < 4; i++) {
    c->var_int_stack_index[i] = -1;
    c->var_int_reg_index[i] = -1;
    c->arr_int_index[i] = -1;
    c->arr_int_size[i] = -1;

    if(i < 3) {
      c->reg_params[i] = -1;
    }
  }
  return ;
}

void context_free(ExecutionContext* c) {
    free(c->var_int_stack_index);
    free(c->var_int_reg_index);
    free(c->arr_int_index);
    free(c->reg_params);
}

/* INSERT VECTOR */
int alloc_vector(int *vector, int len, int value) {
  for(int i = 0; i < len; i++) {
    if(vector[i] == -1) {
      vector[i] = value;
      return i;
    }
  }

  return -1;
}

void context_alloc(ExecutionContext* c, char *str) {
  // aloca variaveis locais de funcao
  if(c->init != 1) {
    printf("Erro: Estrutura nao alocada\n");
  }
  
  int index = 0;
  int size_array = -1;

  if(sscanf(str, FMT_STACK_LOCAL_VAR, &index) == 1) {
    // eh uma variavel de pilha
    int response = alloc_vector(c->var_int_stack_index, 4, index);
    if(response == -1) {
      printf("Error: Nao foi possivel alocar a variavel de pilha %s\n", str);
    }
    return ;
  }


  if(sscanf(str, FMT_REG_LOCAL_VAR, &index) == 1) {
    // eh uma variavel de registrador
    int response = alloc_vector(c->var_int_reg_index, 4, index);
    if(response == -1) {
      printf("Error: Nao foi possivel alocar a variavel de registrador %s\n", str);
    }
    return ;
  }

  if(sscanf(str, FMT_VET_LOCAL_VAR, &index, &size_array) == 2) {
    // eh uma variavel de registrador
    int response = alloc_vector(c->arr_int_index, 4, index);
    if(response == -1) {
      printf("Error: Nao foi possivel alocar a variavel de registrador %s\n", str);
    }
    c->arr_int_size[response] = size_array;
    printf("=================================================\n");
    printf("DECLARACAO DE ARRAY\n");
    printf("TAM : %d, INDEX ARR: %d\n", c->arr_int_size[response], c->arr_int_index[response]);
    printf("=================================================\n");
    return ;
  }
  
}

int _indexof(int *arr, int len, int value) {
  for(int i = 0; i < len; i++) {
    if(arr[i] == value) {
      return i;
    }
  }
  return -1;
}

void context_get(ExecutionContext* c, char *str, char *dest) {
  // printf("### LINE : %s\n", str);
  if(c->init != 1) {
    printf("Erro: Estrutura nao alocada\n");
  }
  
  int index = 0;
  char type;
  
  if(sscanf(str, VAR_STACK_FORMAT, &index) == 1) {
    // eh uma variavel de pilha
    int pos = _indexof(c->var_int_stack_index, 4, index);
    if(pos == -1) {
      printf("Erro : nao foi possivel encontrar a variavel de pilha [%s]\n", str);
    }

    int pos_stack = context_get_element_stack(c, str, NULL);
 
    char stack_str[20];
    snprintf(stack_str, sizeof(stack_str), "-%d(%%rbp)", pos_stack);
    strcpy(dest, stack_str);
    return ;
  }


  if(sscanf(str, VAR_REG_FORMAT, &index) == 1) {
    // eh uma variavel de registrador
    int pos = _indexof(c->var_int_reg_index, 4, index);
    if(pos == -1) {
      printf("Erro : nao foi possivel encontrar a variavel de pilha[%s]\n", str);
    }
    
    switch (pos)
    {
    case 0:
      // r11
      break;
      // printf("%%r11\n");
      strcpy(dest, "%%r11");
    case 1:
      // r10
      // printf("%%r10\n");
      strcpy(dest, "%%r10");
      break;
      
    case 2:
       // r9
      // printf("%%r9\n");
      strcpy(dest, "%%r9");
      break;
    
    case 3:
       // r8
      // printf("%%r8\n");
      strcpy(dest, "%%r8");
      break;
    
    default:
      printf("Erro : Registrador nao encontrado !!\n");
      break;
    }
  }

  if(sscanf(str, CONSTANT_FORMAT, &index) == 1) {
    // printf("%d\n", index);
    // printf("## str %s\n", str);
    // printf("## indice %d\n", index);
    char number_str[20] = "";
    sprintf(number_str, "%d", index);
    strcpy(dest, number_str);
  }
  
  if(sscanf(str, PARAM_FORMAT, &type, &index) == 2) {
    index--;
    int *params = c->reg_params;
    char names_regs_params[][4] = {"rdi", "rsi", "rdx"};
    for(int i = 0; i < 3; i++) {
      if(params[i] == -1) break;
      if(i != index) continue;
      if(params[i] == 8) {
        strcpy(dest, names_regs_params[i]);
        return ;
      } else {

        names_regs_params[i][0] = 'e';
        strcpy(dest, names_regs_params[i]);
        return ;
      }

    }
    strcpy(dest, "##### !!!!");
  }
  // implementar array
}


int _get_size_param(char p) {
  if(!p) return 0;
  int size = 0;
  // inteiro
  if(p == 'i') size = 4;
  // array
  if(p == 'a') size = 8;

  return size;
}

int alloc_params(ExecutionContext* c, char *str) {
  int id_function;
  char type_param1, type_param2, type_param3; 
  int index_param1, index_param2, index_param3;

  int n_match = sscanf(str, FUNCTION_DEF, &id_function, &type_param1, &index_param1, &type_param2, &index_param2, &type_param3, &index_param3);

  int bytes_used = 0;
  int *arr = c->reg_params;

  switch (n_match) {
    case 1:
      // nenhum parametro
      break;

    case 3:
      // 1 parametro informado : rsi
      bytes_used = _get_size_param(type_param1);
      arr[0] = bytes_used;

      break;

    case 5:
      // 2 parametros informados : rsi, rdi
      bytes_used = _get_size_param(type_param1);
      arr[0] = bytes_used;

      bytes_used = _get_size_param(type_param2);
      arr[1] = bytes_used;

      break;

    case 7:
      // 3 parametros informados : rsi, rdi, rdx

      bytes_used = _get_size_param(type_param1);
      arr[0] = bytes_used;
      
      bytes_used = _get_size_param(type_param2);
      arr[1] = bytes_used;

      bytes_used = _get_size_param(type_param3);
      arr[2] = bytes_used;

      break;

    default:
      printf("ERRO: Padrao de parametros nao reconhecido !!");
      return -1;
      break;
  }

  return 0;
}

void context_process_def_local_var(ExecutionContext *c, char *str) {
  int index = 0, n_match = 0;

  n_match = sscanf(str, FMT_REG_LOCAL_VAR, &index);

  if(n_match == 1) {
    // variaveis de registrador
    
  }

  n_match = sscanf(str, FMT_STACK_LOCAL_VAR, &index);

  if(n_match == 1) {
    // variaveis de pilha

  }
}

void context_print_params(ExecutionContext* c) {
  int *arr = c->reg_params;
  // printf("# ========== params list ===============\n");
  for(int i = 0; i < 3; i++) {
    if(arr[i] != -1 && i == 0) {
      // rdi
      if(arr[i] == 4) {
        printf("# pi1 => %%edi\n");
      } else {
        printf("# pi1 => %%rdi\n");
      }
    } else if (arr[i] != -1 && i == 1) {
      // rsi
      if(arr[i] == 4) {
        printf("# pi2 => %%esi\n");
      } else {
        printf("# pi2 => %%rsi\n");
      }
    } else if (arr[i] != -1 && i == 2) {
      // rdx
      if(arr[i] == 4) {
        printf("# pi3 => %%edx\n");
      } else {
        printf("# pi3 => %%rdx\n");
      }
    }
  }
  // printf("# ========== end params list ===============\n");
  return ;
}

void context_print_vlocal_regs(ExecutionContext* c) {
  char regs_names[][4] = {
    "r11", "r10", "r9", "r8"
  };
  int *arr = c->var_int_reg_index;
  for (int i = 0 ; i < 4; i++) {
    if(arr[i] == -1) continue;
    printf("# vr%d => %s\n", arr[i], regs_names[i]);
  }
}

void context_print_vlocal_stack(ExecutionContext* c) {
  int *arr = c->var_int_stack_index;
  int pos = 4;
  for (int i = 0 ; i < 4; i++) {
    if(arr[i] == -1) continue;
    printf("# vi%d => -%d(%%rbp)\n", arr[i], pos);
    pos *= 2;
  }
}

void print_struct(ExecutionContext* c) {
  printf("Vetor de Variaveis de Pilha : \n");
  for (int i = 0; i < 4; i++) {
    printf("%d ", c->var_int_stack_index[i]);
  }
  printf("============\n\n");
  
  printf("Vetor de Variaveis de Registradores : \n");
  for (int i = 0; i < 4; i++) {
    printf("%d ", c->var_int_reg_index[i]);
  }
  printf("============\n\n");
  // printf("Vetor de Registradores : \n");
  // for (int i = 0; i < 4; i++) {
  //   printf("%d ", c->reg_params[i]);
  // }

  printf("============\n");
  printf("Vetor de parametros : \n");
  for (int i = 0; i < 3; i++) {
    printf("%d ", c->reg_params[i]);
  }
  printf("============\n\n");
}


/*
  Essa funcao deve ser chamada apos a definicao de variaveis
ela analisa o que precisa ser alocado na pilha
*/

void context_alloc_stack(ExecutionContext* c) {
  int flag = c->var_int_stack_index[0];
  Stack *s = c->stack;

  if(flag != -1) {
    // existem variaveis de pilha para alocar na pilha
    for(int i = 0; i < 4; i++) {
      if(c->var_int_stack_index[i] == -1) break;

      // aloca a variavel de pilha na pilha
      stack_push(s, ID_TYPE_VAR_LOCAL_STACK, 4, c->var_int_stack_index[i], -1, -1);
    }
  }
}


/*
  Printa a pilha
*/

/*
  if(element->index_array != NULL && element->index_array != -1) {
    nome = nomes_regs_var[element->index_array];
  }
*/

void context_print_stack(ExecutionContext* c) {
  char nomes_regs_var[][4] = {"r11", "r10", "r9", "r8"};
  char *nome;

  printf("\n\n## ----------- STACK ---------------\n");
  int flag = c->var_int_stack_index[0];

  Stack *s = c->stack;
  StackElement *element = s->base;
  
  for(int i = 0; i < s->size; i++) {
    if(element == NULL) break;

    if(element->type == ID_TYPE_VAR_LOCAL_STACK) {
      // variavel local de pilha
      int index = element->index;
      int pos_stack = element->pos_stack;
      printf("## vi%d => -%d(%%rbp)\n", index, pos_stack);

    } else if (element->type == ID_TYPE_PARAMS) {
      int index = element->index;
      int pos_stack = element->pos_stack;
      printf("## pi%d => -%d(%%rbp)\n", index, pos_stack);
    } else if (element->type == ID_TYPE_VAR_LOCAL_REG) {
      int index = element->index;
      int pos_stack = element->pos_stack;

      printf("## vr%d => -%d(%%rbp)\n", index, pos_stack);
    }
    
    element = element->next;
  }

  printf("## ----------- END STACK -----------\n\n");
}

/*
  Salva o contexto atual da execucao
*/
void context_save(ExecutionContext* c, int *count) {
  Stack *s = c->stack;
  char nomes_registradores[][4] = {"rdi", "rsi", "rdx"};
  char nomes_regs_var[][4] = {"r11", "r10", "r9", "r8"};

  printf("# => salvando na pilha\n");

  // Salva os parametros da funcao
  if(c->reg_params[0] != -1) {
    for (int i = 0; i < 3; i++) {
      if(c->reg_params[i] == -1) break;
      stack_push(s, ID_TYPE_PARAMS, c->reg_params[i], i+1, -1, i);
      (*count)++;
      StackElement *element;
      element = s->top;
      if(c->reg_params[i] < 8) {
        // imprime a operacao de salvar o registrador na pilha
        nomes_registradores[i][0] = 'e';
        printf("movq %%%s, -%d(%%rbp)\n", nomes_registradores[i], element->pos_stack);

      } else {
        printf("movq %%%s, -%d(%%rbp)\n", nomes_registradores[i], element->pos_stack);
      }
      
      // printf("movq pi%d -%d(%%rsp)\n", i+1, element->pos_stack);
    }
  }

  // Salva as variaveis de registradores
  if(c->var_int_reg_index[0] != -1) {
    for (int i = 0; i < 4; i++) {
      if(c->var_int_reg_index[i] == -1) break;
      // stack_push(Stack* stack, int type, int len, int index, int size_array)
      stack_push(s, ID_TYPE_VAR_LOCAL_REG, 4, c->var_int_reg_index[i], -1, i);
      (*count)++;
      StackElement *element;
      element = s->top;
      
      printf("movq %%%s, -%d(%%rbp)\n", nomes_regs_var[i], element->pos_stack);
    }
  }

}

/*
  Remove apenas as variaveis que nao sao de pilha mas 
que estao na pilha e depois printa 
*/

void context_destroy_print(ExecutionContext* c) {

  // Salva os parametros da funcao

  // Salva os registradores
}


int context_get_element_stack(ExecutionContext* c, char *str, char *dest_result) {
  Stack *s = c->stack;
  StackElement *element = s->base;

  int type = -1, index;
  int template = -1;

  /*
    p%c%d (parametro na pilha)
    v%c%d (variavel de pilha) onde %c eh i ou a
    v%c%d (variavel de registrador) onde %c == r

    OBJETIVO : Recuperar a posicao da pilha de uma variavel 
      movq -4(%rbp), %rsi
  */

  char *fmt_1 = "p%c%d";
  char *fmt_2 = "v%c%d";
  char id;

  int n_match = sscanf(str, fmt_1, &id, &index);
  
  if(n_match == 2) {
    if(id == 'i') type = ID_TYPE_PARAMS;      // parametro inteiro
    if(id == 'a') type = ID_TYPE_PARAMS_ARR;  // parametro array
    template = 1;
  } else if (sscanf(str, fmt_2, &id, &index) == 2)  {
    if(id == 'i') type = ID_TYPE_VAR_LOCAL_STACK;
    if(id == 'a') type = ID_TYPE_ARR_LOCAL;
    if(id == 'r') type = ID_TYPE_VAR_LOCAL_REG;
    template = 2;
  }

  if(type == -1) return -1;

  for (int i = 0; i < s->size; i++) {
    if(element == NULL) break;
    if(element->type == type && element->index == index) {
      return element->pos_stack;    
    } else {
      element = element->next;
    }

  }

  return -1;
}
