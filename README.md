# Assembler



Para compilar :

```
export verbose=1 && gcc src/main.c src/execution-context/execution_context.c src/interpreter/interpreter.c -o tradutor
```

Executar :

```
./tradutor < input.txt
```