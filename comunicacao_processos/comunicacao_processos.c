#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void implementacao_filho1 (int *num) {
    *num = *num + 1;
}

void implementacao_filho2 (int *num) {
    *num = *num * 2;
}

int main () {
    int *mem;
    pid_t filho1, filho2;

    // shmget "pega" um segmento de memória compartilhada
    // shmget retorna um identificador
    int seg_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    // shmat "pega" esse segmento e o mapeia no espaço de endereçamento do processo atual
    // shmat retorna um ponteiro
    mem = shmat(seg_id, NULL, 0); /*anexa memória compartilhada*/
    /*
    void *shmat(int shmid, const void *shmaddr, int shmflg);
    shmid -> identificador -> retornado por shmget (seg_id)
    shmaddr -> NULL = Não precisa escolher manualmente o endereço
    shmflg -> 0 = flag padrão de leitura/escrita
    */

    // inteiro qualquer escrito no segmento de memória compartilhada
    *mem = 10;

    printf("Processo PAI iniciado - PID: %d\n", getpid());
    printf("Inteiro armazenado na memória compartilhada: %d\n", *mem);

    filho1 = fork();

    if (filho1 == 0) {
        printf("Processo FILHO_1 iniciado - PID: %d\n", getpid());
        printf("Inteiro armazenado na memória compartilhada: %d\n", *mem);
        implementacao_filho1(mem);
        printf("Processo FILHO_1 terminado - PID: %d\n", getpid());
        printf("Inteiro armazenado na memória compartilhada: %d\n", *mem);
        
        // desanexa memória compartilhada
        shmdt(mem);
        
        exit(0);
    }
    
    waitpid(filho1, NULL, 0);
    printf("Processo PAI update - PID: %d\n", getpid());
    printf("Inteiro armazenado na memória compartilhada: %d\n", *mem);
    filho2 = fork();
    
    if (filho2 == 0) {
        printf("Processo FILHO_2 iniciado - PID: %d\n", getpid());
        printf("Inteiro armazenado na memória compartilhada: %d\n", *mem);
        implementacao_filho2(mem);
        printf("Processo FILHO_2 terminado - PID: %d\n", getpid());
        printf("Inteiro armazenado na memória compartilhada: %d\n", *mem);

        // desanexa memória compartilhada
        shmdt(mem);

        exit(0);
    }

    waitpid(filho2, NULL, 0);
    printf("Processo PAI terminado - PID: %d\n", getpid());
    printf("Inteiro armazenado na memória compartilhada: %d\n", *mem);
    
    // desanexa memória compartilhada
    shmdt(mem);

    // remove o segmento do sistema
    shmctl(seg_id, IPC_RMID, NULL);

    return 0;
}