#include "dispositivoDeBloco.h"
#include "sistemaDeArquivos.h"

void dev_abrir(const char *nome)
{
    if(strlen(nome)>16){
        printf("Só são suportados dispositivos com nomes de no maximo 16 caracteres. Abortando operacao!\n");
        return;
    }
    if (dev.fd != 0)
    {
        close(dev.fd);
    }
    dev.fd = open(nome, O_RDWR); // Read and Write
    lseek(dev.fd, 0, SEEK_SET);   
}

void dev_fechar()
{
    close(dev.fd);
    dev.fd = -1;
}

void ler_bloco(int n, void *buffer)
{

    lseek(dev.fd, n * dev.tamBloco, SEEK_SET);
    size_t lidos = read(dev.fd, buffer, dev.tamBloco);
}

void escrever_bloco(int n, void *buffer)
{
    lseek(dev.fd, n * dev.tamBloco, SEEK_SET);
    size_t escritos = write(dev.fd, buffer, dev.tamBloco);
}

