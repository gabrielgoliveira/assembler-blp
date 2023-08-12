# Assembler



Para compilar :

```
gcc src/main.c src/stack/stack.c src/execution-context/execution_context.c src/interpreter/interpreter.c -o tradutor
```

Executar :

```
export verbose=1 && ./tradutor < input.txt
```

Se verbose = 1

O codigo mostra alguns comentarios que mostra o que foi feito
Caso contrario ele mostra apenas a transpilacao