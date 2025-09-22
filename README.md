# 	:computer::gear: Implementação de Máquina Mic-1 

- ### [:dart: Objetivo](#dart-objetivo-1)
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
