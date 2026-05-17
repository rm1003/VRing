/*
===============================================================================
Autor: Ruibin Mei
Data da ultima modificacao: 17/maio/2026
Descricao: 
    Quarto programa de simulacao da disciplina de Sistemas Distribuidos usando
smpl.
    Vamos simular processos i testando processos j no anel virtual e guardando
localmente um vetor que contem informacoes sobre os processos testados.
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
    int* states;
} TipoProcesso;

TipoProcesso *processo;

int main(int argc, char* argv[]) {

    static int N, token, event, r, i, MaxTempoSimulac = 150;
    static char fa_name[5];

    int tmp, j;

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
    puts("          LOG do Trabalho Pratico 0, Tarefa 3");
    puts("  Cada processo mantem localmente o vetor State[N]. A entrada");
    puts("  do vetor State[j] indica o estado do processo j. O estado de");
    puts("  cada processo pode ser: -1 (unknown), 0 (correto) ou 1 (falho).");
    puts("  Inicialize (para todos os processos) o State[N] com -1 (indicando");
    puts("  estado “unknown”) para todos os demais processos e 0 para o ");
    puts("  proprio processo. Nesta tarefa ao executar um teste em um");
    puts("  processo j, o testador atualiza a entrada correspondente no vetor");
    puts("  State[j]. Em cada intervalo de testes, mostre o vetor State[N].");
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
        processo[i].states = malloc(sizeof(int)*N);
        for (int j = 0; j < N; ++j) {
            processo[i].states[j] = -1;
        }
        // inicializa o proprio estado em 0
        processo[i].states[i] = 0;
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
    if (N > 2) {
        schedule(FAULT, 31.0, 2);
    }
    schedule(FINISH, MaxTempoSimulac, -1);

    // agora vem o loop principal do simulador

    while(time() < MaxTempoSimulac) {
        cause(&event, &token);
        switch(event) {
            case TEST:
                if (status(processo[token].id) != 0) break; // o proprio processo falhou

                // Testando todos outros processos
                tmp = (token + 1) % N;
                while(status(processo[tmp].id) != 0 && tmp != token) {
                    printf("Sou o processo %d estou testando o processo %d suspeito no tempo %4.1f\n", token, tmp, time());
                    processo[token].states[tmp] = 1;
                    tmp = (tmp + 1) % N;
                }
                
                if (tmp == token) {
                    printf("O processo %d eh o unico processo correto no anel, teste no tempo %4.1f\n", token, time());
                } else {
                    printf("Sou o processo %d estou testando o processo %d correto no tempo %4.1f\n", token, tmp, time());
                }
                processo[token].states[tmp] = 0;

                for (j = 0; j < N; ++j) {
                    printf("State[%d]: %d\n", j, processo[token].states[j]);
                }
                printf("\n");

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
    

    // free
    for (i = 0; i < N; ++i) {
        free(processo[i].states);
    }
    free(processo);
}
