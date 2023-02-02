#ifndef __SISTEMADEARQUIVOS_H__
#define __SISTEMADEARQUIVOS_H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

struct Linha_dir
{
    char nome[28];
    unsigned int endereco;
};

struct Arquivo
{
    char nome[28];                // nome do arquivo
    unsigned int tam_bytes;       // tamanho atual em bytes 
    unsigned int tam_bloco;       // tamanho atual em bloco 
    unsigned int count_registros; // quantidade de registros caso seja um diretório
    unsigned int tam_registro;    // tamanho de cada registro caso seja um diretorio
    time_t criado;                // data de criacao
    time_t modificado;            // data da ultima modificacao
    bool diretorio;               // é diretorio ou arquivo
    bool bin;                     // é um arquivo binario ou de texto
    bool extraflag0;
    bool extraflag1;
    // 64 bytes
};

struct Dispositivo
{
    char nome[16];                      // nome do dispositivo
    int fd;                             // id do dispositivo
    unsigned int tamBloco;              // tamanho de cada bloco do dispositivo
    unsigned int countBlocos;           // Quantidade de blocos que formam o dispositivo
    unsigned int countBlocosTabela;     // Quantidade de blocos ocupados pela tabela
    // 32 bytes

    int *tabela;
};

static struct Dispositivo dev;

//static int fd = -1; (fd movido para dentro da estrutura Dispositivo)

// Retorna a table criada, como argumento recebe a quantidade de blocos 
int *criaTabela(unsigned long nEntradas);

// Retorna a qunatidade de blocos que formam o dispositivo
unsigned long getNumEntradas();

void escrever_blocos_tabela(int *tabela, int tamBloco);

void ler_blocos_tabela();

int obtem_bloco_vazio();

// Inicia tabela e a escreve no disco
int *escrever_tabela();

void ler_tabela();

// Cria um diretorio dando como paramentros um string de nome (max 28 carateres), o tamanho maximo(nao funcional) e o bloco inicial
int criaDir(const char * nome, unsigned int bloco_inicial);

// Retorna uma lista com todas as linhas do diretório
struct Linha_dir *lerDiretorio(int n);

// Retorna a quantidade de linhas no diretório
unsigned int getDirCount(int n);

// Adiciona uma linha no formato struct Linha_Dir a o diretório especificado pelo bloco inicial
void adicionaLinhaDir(struct Linha_dir linha, int n);

// Printa todas as linhas do diretório espicificado por seu bloco inicial
void printDir(int dir);

// Cria um arquivo de texto, recebe nome e bloco inicial
int criaArquivoText(const char *nome, unsigned int bloco_inicial);

// escreve um arquivo de texto, recebe um ponteiro com o cotneudo, seu tamanho e o arquivo alvo
int escreveArquivoText(const char* conteudo, unsigned int tam_conteudo, unsigned int arquivo);

// le o arquivo e retorna um pointeiro com seu conteudo
const char* leArquivoText(unsigned int arquivo);

// retorna o tamanho em bytes do conteudo do arquivo
unsigned int getTamConteudo(unsigned int arquivo);

// printa o conteuido de um arquivo de texto
void printArquivoText(unsigned int arquivo);

// recebe o bloco inicial de um arquivo e retorna uma formatacao no formato struct Linha_dir dele
struct Linha_dir arquivo2Linha_dir(unsigned int arquivo);

#endif // __SISTEMADEARQUIVOS_H__