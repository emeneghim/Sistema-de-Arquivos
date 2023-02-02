#include "sistemaDeArquivos.h"
#include "dispositivoDeBloco.h"

int *criaTabela(unsigned long nEntradas)
{
    int *tabela = (int *)malloc(nEntradas * sizeof(int));
    return tabela;
}

unsigned long getNumEntradas()
{

    int tamArquivo = lseek(dev.fd, 0, SEEK_END);

    printf("tamArquivo = %d\n", tamArquivo);

    unsigned long nEntradas = tamArquivo / dev.tamBloco;
    return nEntradas;
}

// Escreve a tabela nos blocos
void escrever_blocos_tabela(int *tabela, int tamBloco)
{
    // O tamanho da tabela vai ser o numero de entradas desejado * o tamanho de um inteiro
    long tamanhoTabela = dev.countBlocos * sizeof(int);

    printf("nEntradas/nBlocosDisponiveis = %d\n", dev.countBlocos);
    printf("tamanhoTabela = %d bytes\n", tamanhoTabela);

    printf("nBlocosTabela = %d\n", dev.countBlocosTabela);
    printf("tamBloco = %d\n", tamBloco);

    int *posTabela = tabela;
    printf("posTabela Inicial= %ld\n", posTabela);
    printf("tamBloco / sizeof(int) = %ld\n", tamBloco / sizeof(int));

    for (size_t i = 0; i < dev.countBlocosTabela; i++)
    {
        escrever_bloco(i, posTabela);
        posTabela += (tamBloco / sizeof(int));
        // printf("posTabela = %d, %ld\n", i + 1, posTabela);
    }
    printf("Tabela escrita no disco\n");
}

// Verifica se o bloco está vazio
int obtem_bloco_vazio()
{
    // O tamanho da tabela vai ser o numero de entradas desejado * o tamanho de um inteiro
    long tamanhoTabela = dev.countBlocos * sizeof(int);

    for (int i = 0; i < dev.countBlocos; i++)
    {
        if (dev.tabela[i] == 0)
        {
            printf("O bloco %d está vazio\n", i);
            return i;
        }
    }
    printf("Nenhum bloco vazio\n");
    return -1;
}

// TODO: verificar se esta funcionando
void ler_blocos_tabela()
{
    // O tamanho da tabela vai ser o numero de entradas desejado * o tamanho de um inteiro
    long tamanhoTabela = dev.countBlocos * sizeof(int);

    int *posTabela = dev.tabela;

    for (size_t i = 0; i < dev.countBlocosTabela; i++)
    {
        ler_bloco(i, posTabela);
        posTabela += dev.tamBloco / sizeof(int);
    }

    printf("\nPrintando da memória:\n");
    int i = 0;
    for (i; i < dev.countBlocosTabela * (dev.tamBloco/sizeof(int)); i++)
    {
        printf("%d = %d\n",i, dev.tabela[i]);
    }
    
}

int *escrever_tabela()
{
    const char *nome = "bla.data";
    // Abre arquivo
    dev_abrir(nome);

    // nEntradas = nBlocosTotal
    dev.countBlocos = getNumEntradas();

    // Criando e armazenando tabela na memória
    int *tabela = criaTabela(dev.countBlocos);

    // Inicializando os valores na tabela

    // quantidade de blocos que a tabela ocupa
    dev.countBlocosTabela = dev.countBlocos / dev.tamBloco;
    // inicia blocos da tabela
    size_t i = 0;
    for (i; i < dev.countBlocosTabela * (dev.tamBloco / sizeof(int))- 1; i++)
    {
        tabela[i] = i + 1;
        printf("%d:%d\n", i, i + 1);
    }
    tabela[i] = -1;
    printf("%d:-1\n", i);
    i++;
    // inicia o resto dos blocos com 0
    for (i; i < dev.countBlocos; i++)
    {
        tabela[i] = 0;
        // printf("%d:0\n", i);
    }

    escrever_blocos_tabela(tabela, dev.tamBloco);

    dev_fechar();

    return tabela;
}

void ler_tabela()
{
    dev.countBlocos = getNumEntradas();

    // Abre arquivo
    dev_abrir(dev.nome);

    ler_blocos_tabela();

    dev_fechar();

    return 0;
}

int criaDir(const char *nome, unsigned int bloco_inicial)
{
    if (strlen(nome) > 28)
    {
        printf("Nome de diretorio muito grande, deve conter no máximo 28 caracteres, abordatando operacao\n");
        return -1;
    }
    // obtem o tempo (data) atual
    time_t currentTime = time(NULL);
    // instancia novo diretório
    struct Arquivo newDir = {
        .tam_bytes = 64,
        .tam_bloco = 1,
        .tam_registro = (unsigned int)sizeof(struct Linha_dir),
        .criado = currentTime,
        .modificado = currentTime,
        .diretorio = true,
        .bin = false,
        .extraflag0 = false,
        .extraflag1 = false};
    strcpy(newDir.nome, nome);
    // cria buffer de escrita e escreve no bloco inicial
    void *buffer = malloc(dev.tamBloco);
    memcpy(buffer, &newDir, sizeof(struct Arquivo));
    escrever_bloco(bloco_inicial, buffer);
    dev.tabela[bloco_inicial] = -1;
    return 1;
}

struct Linha_dir *lerDiretorio(int n)
{

    void *buffer = malloc(dev.tamBloco);

    // le o cabecalho do diretorio
    ler_bloco(n, buffer);

    struct Arquivo cabecalho;

    memcpy(&cabecalho, buffer, sizeof(struct Arquivo));

    printf("Lendo diretorio %.28s...\n", cabecalho.nome);

    int tam_linha_dir = sizeof(struct Linha_dir);

    // bloco atual = bloco de dados, descartando cabecalho
    int bloco_atual = dev.tabela[n];

    // cria lista que gaurda todos os registos do diretório
    struct Linha_dir *lista = (struct Linha_dir *)malloc(cabecalho.count_registros * tam_linha_dir);
    int bloco = 0;
    struct Linha_dir *aux = lista;
    while (bloco_atual != -1)
    {
        // le os dados do diretório
        ler_bloco(bloco_atual, buffer);
        bloco_atual = dev.tabela[bloco_atual];
        if (bloco_atual != -1)
        {
            for (int i = 0; i < (dev.tamBloco / tam_linha_dir); i++)
            {
                memcpy(&aux[bloco * (dev.tamBloco / tam_linha_dir) + i], buffer + (i * tam_linha_dir), tam_linha_dir);
            }
        }
        else
        {
            for (int i = 0; i < (cabecalho.count_registros % (dev.tamBloco / tam_linha_dir)); i++)
            {
                memcpy(&aux[bloco * (dev.tamBloco / tam_linha_dir) + i], buffer + (i * tam_linha_dir), tam_linha_dir);
            }
        }
        bloco++;
    }

    return lista;
}

unsigned int getDirCount(int n)
{

    void *buffer = malloc(dev.tamBloco);

    ler_bloco(n, buffer);

    struct Arquivo cabecalho;

    memcpy(&cabecalho, buffer, sizeof(struct Arquivo));

    return cabecalho.count_registros;
}

void adicionaLinhaDir(struct Linha_dir linha, int n)
{
    int tam_linha_dir = sizeof(struct Linha_dir);

    void *buffer = malloc(dev.tamBloco);

    struct Arquivo cabecalho;

    // le cabecalho
    ler_bloco(n, buffer);

    memcpy(&cabecalho, buffer, 64);

    // obtem quantos registros tem no bloco atual
    unsigned int ultimo_registro_bloco = cabecalho.count_registros % (dev.tamBloco / sizeof(struct Linha_dir));

    int bloco_atual = dev.tabela[n];

    int ultimo_bloco = n;
    // vai até o ultimo bloco alocado pelo diretorio
    while (bloco_atual != -1)
    {
        ultimo_bloco = bloco_atual;
        bloco_atual = dev.tabela[ultimo_bloco];
    }

    // se for necessario adicionar novo bloco
    if (ultimo_registro_bloco == 0)
    {
        // obtem novo bloco vazio
        bloco_atual = obtem_bloco_vazio();
        // ultimo bloco aponta para o novo bloco criado
        dev.tabela[ultimo_bloco] = bloco_atual;
        // bloco criado se torna o ultimo
        dev.tabela[bloco_atual] = -1;
        // setta o novo bloco como ultimo
        ultimo_bloco = bloco_atual;
        cabecalho.tam_bloco += 1;
    }

    // atualiza o cabecalho
    cabecalho.count_registros++;
    cabecalho.tam_bytes += tam_linha_dir;
    cabecalho.modificado = time(NULL);

    // grava alteracoes
    memcpy(buffer, &cabecalho, sizeof(struct Arquivo));

    escrever_bloco(n, buffer);

    // adiciona registro
    ler_bloco(ultimo_bloco, buffer);

    memcpy(buffer + (tam_linha_dir * ultimo_registro_bloco), &linha, tam_linha_dir);

    escrever_bloco(ultimo_bloco, buffer);
}

void printDir(int dir)
{

    // pega a quantidade de itens do diretório
    unsigned int tam = getDirCount(dir);
    // se não for  0
    if (tam > 0)
    {
        // cria um ponteiro array para todas as linhas
        struct Linha_dir *lista = (struct Linha_dir *)malloc(tam * sizeof(struct Linha_dir));
        // recebe as linhas do diretório
        lista = lerDiretorio(dir);
        for (int i = 0; i < tam - 1; i++)
        {
            printf("\t%.28s:%d\n", lista->nome, lista->endereco);
            lista++;
        }
        printf("\t%.28s:%d\n", lista->nome, lista->endereco);
    }
    else
    {
        printf("Diretório vazio!\n");
    }
}

int criaArquivoText(const char *nome, unsigned int bloco_inicial)
{
    if (strlen(nome) > 28)
    {
        printf("Nome de arquivo muito grande, deve conter no máximo 28 caracteres, abortando operacao\n");
        return -1;
    }
    // obtem o tempo (data) atual
    time_t currentTime = time(NULL);
    // instancia novo diretório
    struct Arquivo newArquivo = {
        .tam_bytes = 64,
        .tam_bloco = 1,
        .tam_registro = dev.tamBloco,
        .criado = currentTime,
        .modificado = currentTime,
        .diretorio = false,
        .bin = false,
        .extraflag0 = false,
        .extraflag1 = false};
    strcpy(newArquivo.nome, nome);
    // cria buffer de escrita e escreve no bloco inicial
    void *buffer = malloc(dev.tamBloco);
    memcpy(buffer, &newArquivo, sizeof(struct Arquivo));
    escrever_bloco(bloco_inicial, buffer);
    dev.tabela[bloco_inicial] = -1;
    return 1;
}

int escreveArquivoText(const char *conteudo, unsigned int tam_conteudo, unsigned int arquivo)
{
    unsigned int blocos = tam_conteudo / dev.tamBloco;
    if (tam_conteudo % dev.tamBloco > 0)
    {
        blocos++;
    }

    // atualiza cabecalho
    void *buffer = malloc(dev.tamBloco);

    ler_bloco(arquivo, buffer);

    struct Arquivo cabecalho;

    memcpy(&cabecalho, buffer, sizeof(struct Arquivo));

    cabecalho.tam_bloco += blocos;
    cabecalho.tam_bytes += tam_conteudo;

    // grava alteracoes
    memcpy(buffer, &cabecalho, sizeof(struct Arquivo));
    escrever_bloco(arquivo, buffer);

    // faz a escrita
    int bloco_atual = arquivo;
    int bloco_anterior = arquivo;

    for (int i = 0; i < blocos; i++)
    {
        printf("alocando um bloco para a escrita...\n");
        // atualiza tabela
        bloco_atual = obtem_bloco_vazio();
        // escreve o conteudo no bloco atual
        dev.tabela[bloco_anterior] = bloco_atual;
        dev.tabela[bloco_atual] = -1;
        memcpy(buffer, conteudo, dev.tamBloco);
        escrever_bloco(bloco_atual, buffer);
        // prepara para a proxima escrita
        conteudo += dev.tamBloco;
        bloco_anterior = bloco_atual;
    }
    return 1;
}

const char *leArquivoText(unsigned int arquivo)
{

    // atualiza cabecalho
    void *buffer = malloc(dev.tamBloco);

    ler_bloco(arquivo, buffer);

    struct Arquivo cabecalho;

    memcpy(&cabecalho, buffer, sizeof(struct Arquivo));

    unsigned int blocos = cabecalho.tam_bloco - 1;
    char *conteudo = malloc(cabecalho.tam_bytes - 64);

    printf("Lendo Arquivo: %s...\n", cabecalho.nome);

    // vai apra o primeiro bloco de dados
    int bloco_atual = dev.tabela[arquivo];
    int bloco_anterior = arquivo;

    if (bloco_atual == -1)
    {
        printf("Arquivo vazio!");
        return NULL;
    }

    char *aux = conteudo;

    // le enquanto não chega no ultimo bloco
    while (bloco_atual != -1)
    {
        printf("lendo bloco de texto...\n");
        // le o conteudo do bloco
        ler_bloco(bloco_atual, buffer);
        memcpy(aux, buffer, dev.tamBloco);
        // setta o proximo bloco
        aux += dev.tamBloco;
        bloco_anterior = bloco_atual;
        bloco_atual = dev.tabela[bloco_anterior];
    }

    return conteudo;
}

unsigned int getTamConteudo(unsigned int arquivo)
{

    void *buffer = malloc(dev.tamBloco);

    ler_bloco(arquivo, buffer);

    struct Arquivo cabecalho;

    memcpy(&cabecalho, buffer, sizeof(struct Arquivo));

    return cabecalho.tam_bytes - 64;
}

void printArquivoText(unsigned int arquivo)
{
    unsigned int tam = getTamConteudo(arquivo);
    printf("tam: %d\n", tam);
    char conteudo[tam];
    strcpy(conteudo, leArquivoText(arquivo));
    printf("\n%*s\n\n", tam, conteudo);
}

struct Linha_dir arquivo2Linha_dir(unsigned int arquivo)
{
    void *buffer = malloc(dev.tamBloco);

    ler_bloco(arquivo, buffer);

    struct Arquivo cabecalho;

    memcpy(&cabecalho, buffer, sizeof(struct Arquivo));

    struct Linha_dir linha_dir = {.endereco = arquivo};

    strcpy(linha_dir.nome, cabecalho.nome);

    return linha_dir;
}