#ifndef BLA_DISPOSITIVO_BLOCO_H
#define BLA_DISPOSITIVO_BLOCO_H

void dev_abrir(const char *nome);
void dev_fechar();
void ler_bloco(int n, void *buffer);
void escrever_bloco(int n, void *buffer);

#endif