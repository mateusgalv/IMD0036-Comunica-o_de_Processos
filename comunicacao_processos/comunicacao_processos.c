#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void implentacao_filho1 (int *num) {
    *num = *num + 1;
}

void implentacao_filho2 (int *num) {
    *num = *num * 2;
}

int main () {
    int *mem;
    pid_t filho1, filho2;

    int seg_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    mem = shmat(seg_id, NULL, 0); /*anexa memória compartilhada*/

    *mem = 10; /*escreve inteiro qualquer no segmento de memória compartilhada*/

    printf("Processo PAI iniciado - PID: %d, inteiro: %d\n", getpid(), *mem);

    filho1 = fork();
    if (filho1 == 0) {
        printf("Processo filho1 iniciado - PID: %d\n", getpid());
        mem = shmat(seg_id, NULL, 0);
        implentacao_filho1(mem);
        printf("filho1 finalizou - PID: %d - inteiro: %d\n", getpid(), *mem);
        
        shmdt(mem); /*desanexa memória compartilhada*/
        exit(0);
    }
    
    waitpid(filho1, NULL, 0);
    filho2 = fork();
    
    if (filho2 == 0) {
        printf("Processo filho2 criado - PID: %d\n", getpid());
        mem = shmat(seg_id, NULL, 0);
        implentacao_filho2(mem);
        printf("filho2 finalizou - PID: %d - inteiro: %d\n", getpid(), *mem);

        shmdt(mem);
        exit(0);
    } else {
        waitpid(filho2, NULL, 0);

        printf("Processo PAI finalizado, inteiro armazenado: %d\n", *mem);
        
        shmdt(mem);
        shmctl(seg_id, IPC_RMID, NULL); /*remove o segmento do sistema*/
    }

    return 0;
}