/*
===============================================================================
Autor: Ruibin Mei
Data da ultima modificacao: 17/maio/2026
Descricao: 
    Primeiro programa de simulacao da disciplina de Sistemas Distribuidos usando
smpl.
    Vamos simular N processos.
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "smpl.h"

#define TEST 1
#define FAULT 2
#define RECOVERY 3
#define FINISH 4

typedef struct {
    int id;

} TipoProcesso;

TipoProcesso *processo;

int main(int argc, char* argv[]) {

    static int N, token, event, r, i, MaxTempoSimulac = 150;

    static char fa_name[5];

    if (argc != 2) {
        puts("Uso correto: tempo <numero de processos>");
        exit(1);
    }

    N = atoi(argv[1]);

    if (N <= 0) {
        puts("Numero de processos deve ser positivo");
        exit(1);
    }

    smpl(0, "Meu primeiro programa de simulacao de sistemas distribuidos");
    reset();
    stream(1);

    puts("===============================================================");
    puts("           Sistemas Distribuidos Prof. Elias");
    puts("          LOG do Trabalho Pratico 0, Tarefa 0");
    puts("      Digitar, compilar e executar o programa tempo.c");
    printf("   Este programa foi executado para: N=%d processos.\n", N); 
    printf("           Tempo Total de Simulacao = %d\n", MaxTempoSimulac);
    puts("===============================================================");

    // inicializar os N processos

    processo = (TipoProcesso*) malloc(sizeof(TipoProcesso)*N);
    if (processo == NULL) {
        error(0, "Alocacao invalida nos processos\n");
        exit(1);
    }

    for (i = 0; i < N; ++i) {
        memset(fa_name, '\0', 5);
        sprintf(fa_name, "%d", i);
        processo[i].id = facility(fa_name, 1);
    }

    // Vamos agora fazer o escalonamento dos eventos iniciais
    // No primeiro intervalo de testes os processos vao testar

    for (i = 0; i < N; ++i) {
        schedule(TEST, 30.0, i); // todos os processos de 0 ate N-1 vao testar na unidade de tempo 30
    }

    if (N > 1) {
        schedule(FAULT, 31.0, 1);
        schedule(RECOVERY, 61.0, 1);
    }
    schedule(FINISH, MaxTempoSimulac, -1);


    // agora vem o loop principal do simulador

    while(time() < MaxTempoSimulac) {
        cause(&event, &token);
        switch(event) {
            case TEST:
                if (status(processo[token].id) !=0) break; // Processo falho nao testa!
                printf("Sou o processo %d estou testando no tempo %4.1f\n", token, time());
                schedule(TEST, 30.0, token);
                break;
            case FAULT:
                r = request(processo[token].id, token, 0);
                printf("O processo %d falhou no tempo %4.1f\n", token, time());
                break;
            case RECOVERY:
                release(processo[token].id, token);
                printf("O processo %d recuperou no tempo %4.1f\n", token, time());
                schedule(TEST, 1.0, token);
                break;
            case FINISH:
                printf("Fim da simulacao no tempo %4.1f\n", time());
                break;
        } // switch
    } // while

    free(processo);
}
