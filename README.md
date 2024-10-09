# Trabalho Prático 2 - BD

## Como Compilar:
```bash
./compile.sh <project_name>
```
Nome padrão: `my_project`

## Como Executar:
```bash
./run.sh <executable_name>
```
Por padrão, `executable_name` é o último nome de projeto compilado, localizado em `project.config`. Todos os executáveis devem estar na pasta `/bin`.

## Convenções de Nomeclatura:

- **Nome de Classes**: `PascalCase`, exemplo: 
  ```cpp
  class MinhaClasse {};
  ```

- **Nome de Métodos/Funções**: `camelCase`, exemplo:
  ```cpp
  void minhaFuncao();
  ```

- **Nome de Variáveis**: `snake_case`, exemplo:
  ```cpp
  int minha_variavel;
  ```

- **Nome de Constantes**: `MAIÚSCULAS_COM_UNDERSCORE`, exemplo:
  ```cpp
  #define MINHA_CONSTANTE 100
  ```

- **Nome de Enums**: `PascalCase`, exemplo:
  ```cpp
  enum Color { Red, Green, Blue };
  ```

## Observações
-  Mantenha a consistência nas convenções de nomeclatura em todo o projeto.
