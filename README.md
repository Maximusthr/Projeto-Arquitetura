# 	:computer::gear: Implementação de Máquina Mic-1 

- ### [:dart: Objetivo](#dart-objetivo-1)
- ### [:spiral_notepad: Comentários](#spiral_notepad-comentários-1)
- ### [:gear: Como rodar](#gear-como-rodar-1)
- ### [:arrow_down: Baixar o projeto](https://github.com/Maximusthr/Projeto-Arquitetura/archive/refs/heads/main.zip)

## Disciplina de Arquitetura de Computadores

Esse foi um projeto desenvolvido por discentes do curso de *Engenharia da Computação da Universidade Federal da Paraíba*, curso este que pertence ao *[Centro de Informática](http://ci.ufpb.br/)*, localizado na *[Rua dos Escoteiros S/N - Mangabeira - João Pessoa - Paraíba - Brasil](https://g.co/kgs/xobLzCE)*. O projeto busca a implementação correspondente a uma versão modificada da Máquina Mic-1 na forma de uma máquina virtual em linguaguem de alto nível. O projeto foi avaliado por meio da verificação do funcionamento correto do projeto e a validação dos resultados esperados. 

### :classical_building: Autores:

-  :triangular_ruler:  *[Arthur Maximus Cavalcante Beuttenmuller](https://github.com/Maximusthr)*
-  :straight_ruler:  *[Eduardo Asfuri Carvalho](https://github.com/Asfuri)*
-  :triangular_ruler::  *[Lucas Henrique Vieira da Silva](https://github.com/hvslucas)*
-  :straight_ruler:  *[Marco Antonio de Vasconcelos Souza Filho](https://github.com/MarcoFilho1)*

###  	:building_construction: Docente:

-  :construction_worker_man::  *Augusto Holanda*
<br>

[![chip-logo](https://github.com/user-attachments/assets/63eea007-4a99-4238-b66c-cc52ebe194f4)](#computergear-implementação-de-máquina-mic-1)

## :dart: Objetivo:

O objetivo deste projeto é estudar, implementar e simular uma versão modificada da máquina Mic-1. Seguindo as especificações fornecidas pelo professor, o projeto implementa uma máquina virtual escrita em linguagem de alto nível, com o intuito de compreender a execução das instruções de uma ISA modificada, similar à da IJVM. Ao final, o grupo será avaliado ao apresentar e explicar o código implementado, demonstrando como ele processa as instruções, modifica a memória e os registradores, e gera as saídas correspondentes.

[^1]: A Mic-1 [(MicroInstruction Computer 1)](https://en.wikipedia.org/wiki/MIC-1) é uma máquina hipotética criada por Tanenbaum para ensinar os conceitos de [microarquitetura](https://pt.wikipedia.org/wiki/Microarquitetura), ou seja, a implementação de um processador em nível de microinstruções.

## :spiral_notepad: Comentários:

### Comentários gerais:  

- Este programa simula a execução de instruções específicas da IJVM que controlam registradores e memória.
- Foi projetado para interpretar microinstruções de 23 bits (representadas como strings de '0' e '1') e sequências de instruções dealto nível que são traduzidas em microinstruções.  

**Registradores simulados (nomes e propósito):**  

- MAR  : Memory Address Register - endereço lido/escrito em memória
- MDR  : Memory Data Register - dado lido/escrito em memória
- PC   : Program Counter (neste simulador existe PC_registrador separado do PC lido de IR)
- MBR  : Memory Buffer Register (8-bit) - usa-se tanto como signed quanto unsigned
- SP   : Stack Pointer
- LV   : Local variables pointer/frame pointer (conforme arquitetura do trabalho)
- CPP  : Constant Pool Pointer (uso conforme especificação do trabalho)
- TOS  : Top Of Stack (topo da pilha)
- OPC  : Operacional Code? (conforme trabalho — usado para debug/registro)
- H    : Registrador temporário (usado como operando A em ULA)    

- Memória de dados: MEM é um vetor de int32_t onde cada posição corresponde a uma "palavra" de 32 bits (lida de arquivo binário textual — cada token com 32 caracteres '0'/'1').

### Microinstrução de 23 bits (mapeamento de bits):  

Bits da ULA:
- [0]   desloca Sd << 8 (SEXT? depende da especificação do trabalho) - SLL8
- [1]   desloca Sd >> 1 - SRA1
- [2]   F0  (ULA)
- [3]   F1  (ULA) 
- [4]   ENA (controle para habilitar A) 
- [5]   ENB (controle para habilitar B)
- [6]   INVA (controle para inverter A)
- [7]   INC (carry-in para soma) - Vem-um

Bits do Barramento C:
- [8..16] bits de seleção do barramento C (escrevem Sd em registradores) - De acordo com a decodificação fornecida

Bits de controle de memória:
- [17]  WRITE (se 1 então operação de escrita na memória está habilitada)
- [18]  READ  (se 1 então operação de leitura na memória está habilitada) 
(Caso ambos sejam 1, corresponde a operação de fetch realizada por MBR)

Bits do Barramento B: 
- [19..22] código de 4 bits que seleciona qual registrador está no barramento B

### Observação sobre o 'FETCH' especial para BIPUSH:

Para a instrução BIPUSH há uma microinstrução especial que contém os 8 bits do parâmetro (byte) nos primeiros 8 bits da microinstrução. 
Quando a microinstrução tiver simultaneamente READ=1 e WRITE=1 (condição usada aqui para identificaresse caso), o simulador trata isso como um FETCH do byte para o MBR e H.

- Arquivos de entrada esperados (até etapa 3.1):

1. "memoria_inicial.txt": tokens de 32 caracteres binários (cada token -> palavra de 32 bits).
2. "registrador_inicial.txt": arquivo contendo linhas no formato:  
nome_registrador = <valor_binario>  
Exemplo:  
mar = 00000000000000000000000000000101  
mdr = 00000000000000000000000000000000  
...
**Observação:** os nomes esperados (em minúsculas) são:  
mar, mdr, pc, mbr, sp, lv, cpp, tos, opc, h  
O campo mbr será convertido para uint8_t (somente os 8 LSB do valor binário).  
 
- Arquivo de saída:  
  
- "./saida_etapa3_tarefa1.txt" - contém logs detalhados da execução de cada
microinstrução, estado dos registradores antes/depois e dump da memória.

**Projeto completo:** Permite as entradas ```ILOAD x```, ```BIPUSH``` e ```DUP```, em um arquivo ```"instrucoes.txt"```, ```"memoria_inicial.txt"``` com a memória inicial e ```"registrador_inicial.txt"```com o estado inicial dos registradores.

## :gear: Como rodar

### Requisitos e Recomendações

Para compilar e executar o arquivo `mic.cpp`, você precisa de um compilador C++ moderno. Recomendamos o uso do **GCC** (no Linux) ou **MinGW** (no Windows).

- **GCC (Linux)**: normalmente já vem instalado em distribuições Linux. Caso não tenha, instale com:
  ```bash
  sudo apt install g++
  ```

- **MinGW (Windows)**: baixe e instale pelo site oficial:
  [https://www.mingw-w64.org/](https://www.mingw-w64.org/)

---

### Compilar e Rodar

#### No Linux

```bash
g++ mic.cpp -o mic
./mic
```

#### No Windows (com MinGW)

Abra o terminal (cmd ou PowerShell) e execute:

```bash
g++ mic.cpp -o mic.exe
mic.exe
```

> Certifique-se de que o comando `g++` está disponível no terminal (variável de ambiente PATH corretamente configurada).


[**Atenção:** Lembre de baixar o projeto e extraí-lo devidamente do `.zip`.](#computergear-implementação-de-máquina-mic-1)
