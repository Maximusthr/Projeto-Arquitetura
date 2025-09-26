# 	:computer::speech_balloon: Servidor de Chat Multiusuário (TCP)

- ### [:dart: Objetivo](#dart-objetivo-1)
- ### [:spiral_notepad: Comentários](#spiral_notepad-comentários-1)
- ### [:open_file_folder: Estrutura do Projeto](#open_file_folder-estrutura-do-projeto-1)
- ### [:gear: Como rodar](#gear-como-rodar-1)
- ### [:arrow_down: Baixar o projeto](https://github.com/hvslucas/chatmultiuser/archive/refs/heads/main.zip)

## Disciplina de Linguagem de Programação II (Programação Concorrente)

Esse foi um projeto desenvolvido por discentes do curso de *Engenharia da Computação da Universidade Federal da Paraíba*, curso este que pertence ao *[Centro de Informática](http://ci.ufpb.br/)*, localizado na *[Rua dos Escoteiros S/N - Mangabeira - João Pessoa - Paraíba - Brasil](https://g.co/kgs/xobLzCE)*. O projeto busca a implementação correspondente a uma versão modificada da Máquina Mic-1 na forma de uma máquina virtual em linguaguem de alto nível. O projeto foi avaliado por meio da verificação do funcionamento correto do projeto e a validação dos resultados esperados. 

### :speech_balloon: Autor:

-  :link:  *[Lucas Henrique Vieira da Silva](https://github.com/hvslucas)*

[![chip-logo](https://github.com/user-attachments/assets/63eea007-4a99-4238-b66c-cc52ebe194f4)](#computergear-implementação-de-máquina-mic-1)

## :dart: Objetivo:

O objetivo deste projeto é desenvolver um sistema concorrente em rede completo (cliente/servidor TCP ou UDP) utilizando das técnicas de programação concorrente desenvolvidas ao decorrer da disciplina e recursos de comunicação entre processos locais ou em rede. Ao final, o projeto será avaliado a partir da saída e das boas práticas do código implementado.

### Descrição da etapa 1 - Biblioteca libtslog - Logging Thread-Safe

Implementação de uma biblioteca simples de **logging concorrente** em C (`libtslog`), utilizando **pthread mutex** para garantir exclusão mútua na escrita em arquivo.  

A biblioteca permite que múltiplas threads gravem mensagens de log de forma segura, evitando *race conditions*.  
Um programa de teste (`log_test.c`) simula várias threads concorrentes gerando mensagens.


## :spiral_notepad: Comentários:

### Comentários gerais:  

### Etapa 1:

## :open_file_folder: Estrutura do Projeto


```
.
├── script.sh           # Script para compilar e executar
├── main.c              # Código-fonte principal com valores fixos
├── personalizado.c     # Código-fonte secundário, possui valores de entrada via terminal
├── semCor/              # Pasta com versão sem elementos de cor
│   └── mainO.c          # Versão simplificada, não compilada pelo script
└── README.md            # Este arquivo, documenta o projeto
```

### Arquitetura



## :gear: Como rodar

[**Atenção:** Lembre de baixar o projeto e extraí-lo devidamente do `.zip`.](#computerspeech_balloon-servidor-de-chat-multiusuário-tcp)

### :package: Dependências

Para compilar e executar o projeto em **Linux**, é necessário ter instalados:

- **GCC** (GNU Compiler Collection)
- **pthread** (biblioteca de threads POSIX)
- Bibliotecas nativas da linguagem C

No Ubuntu/Debian, instale com:
```bash
sudo apt update
sudo apt install build-essential
```

### :arrow_forward: Compilação e Execução 

O projeto contém um script `.sh` que oferece algumas opções e, a partir da escolha, compila e executa automaticamente.

1. Dê permissão de execução ao script:

```bash
chmod +x run.sh
```

2. Executar o script:

```bash
./script.sh
```

3. O script irá fornecer as opções:
```
1 - Compilar (make)
2 - Executar teste padrão (3 threads x 10 msgs)"
3 - Executar teste custom (args: <threads> <msgs>)"
4 - Limpar
0 - Sair"
```



Algumas observações: 
- O script `run.sh` possui alguns recursos para ficar mais elegante ao usuário e talvez esses recursos fiquem modificados em outros terminais
