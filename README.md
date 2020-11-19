# Trabalho 1 - Montador de Duas Passagens
## Universidade de Brasília - UnB
---
## Autor
### Nome: Heitor de Lima Belém
### Matrícula: 16/0123950
---
# Opção escolhida: Montador de duas passagens com detecção de erros
---
## Instruções para compilação
1. Verifique se, na pasta **montador_sb01/src** existe a pasta **obj**:
  - Se não existir crie essa pasta (**obj**) dentro de **montador_sb01/src/** e vá para o passo _2_
  - Se existir, vá para o passo _2_.
2. No terminal, dentro da pasta **montador_sb01/src** execute o comando:
    ```
      make
    ```
3. Agora, ainda na mesma pasta do passo anterior, execute o programa:
  - Para fazer apenas o pré processamento, use a diretiva '-p'. Fazendo isso, será obtido o arquivo texto .PRE, com o mesmo nome do arquivo de input que foi passado.
    ```
      ./montador -p <nome_do_arquivo>.asm
    ```
  - Para montar o programa, use a diretiva '-o'. Com isso, a saída será um arquivo texto .OBJ com o mesmo nome do arquivo de input que foi passado.
    ```
      ./montador -o <nome_do_arquivo>.asm
    ```
  **OBS**: o arquivo *.asm* deve estar na pasta **montador_sb01/src**
  
  ### Exemplo de Pre processamento:
  
  ```bash
    ./montador -p triangulo.asm

    Arquivo de saída: triangulo.PRE
  ```

  ### Exemplo Montador:
  ```bash
    ./montador -o triangulo.asm

    Arquivo de saída: triangulo.OBJ
  ```
# Observações Importantes sobre o Trabalho
## Trabalho feito no Linux usando MakeFile
## Os arquivos de saída são armazenados na pasta montador_sb01/src

    