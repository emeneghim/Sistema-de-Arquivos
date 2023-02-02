#include <stdio.h>
#include <stdlib.h>
#include "dispositivoDeBloco.c"
#include "sistemaDeArquivos.c"
#include <string.h>

int main(int argc, char *argv[])
{
    // inicia o basico do dispositivo
    dev.fd = -1;
    dev.tamBloco = 512;
    strcpy(dev.nome, "bla.data");

    dev.tabela = escrever_tabela();

    dev_abrir(dev.nome);

    // obtem bloco livre para ciraçao de diretorio
    int diretorio = obtem_bloco_vazio(dev.tabela);

    // int numero_de_arquivos = 10;
    // int arquivo[numero_de_arquivos];

    // // Cria um diretorio
    // if (criaDir("Diretorio_teste\0", diretorio) != -1)
    // {   
    //     // cria os arquivos de texto do diretorio
    //     for(int i=0; i<numero_de_arquivos; i++){
    //         arquivo[i]=obtem_bloco_vazio();
    //         char number[28];
    //         snprintf(number, 28, "arquivo-%d.txt", i);
    //         criaArquivoText(number, arquivo[i]);
    //     }

    //     // coloca os arquivos no diretorio (agora não é só estetico kkk)
    //     for(int i=0; i<numero_de_arquivos; i++){
    //         adicionaLinhaDir(arquivo2Linha_dir(arquivo[i]), diretorio);
    //     }

    //     // Printa os registros do diretorio
    //     printDir(diretorio);
    // }

    // //cria um buffer para receber entrada 
    // char buffer[__INT16_MAX__];
    // printf("Digite o que gostaria de escrever no arquivo:\n");
    // fgets(buffer,__INT16_MAX__, stdin);

    // // pega o tamanmho da entrada digitada
    // int tam = strlen(buffer);
    // if (tam > 0 && buffer[tam-1] == '\n')
    //     buffer[tam-1] = '\0';

    // // escreve a entrada no primeiro arquivo
    // escreveArquivoText(buffer, tam, arquivo[0]);
    // // printa o conteudo do arquivo de texto
    // printArquivoText(arquivo[0]);

    ler_tabela();

    // fecha o dispositivo bla.data
    dev_fechar();

    return 0;
}