// Comunicação produtor-consumidor
// Produtor escreve em um extremo (o extremo de escrita do pipe)
// Consumidor lê do outro extremo (o extremo de leitura do pipe)
// Pipes "comuns" são unidirecionais
// Necessitam de relação pai-filho entre os processos

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    (void)argc;
    // vetor_pipe[0] -> descritor de leitura
    // vetor_pipe[1] -> descritor de escrita
    int vetor_pipe[2];
    
    // +1 inclui '\0' do final da string
    // tamanho: qnt de bytes que será escrita/lida nesta operação
    int tamanho = strlen(argv[1]) + 1;

    // buffer: armazenamento temp do que é lido do pipe
    // pipe -> buffer
    char buffer[tamanho];

    pipe(vetor_pipe);
    pid_t pid = fork();

    if (pid == 0) {
        // PROCESSO FILHO

        // Fecha caminho de escrita
        close(vetor_pipe[1]);

        // Leitura
        // read retorna bytes lidos
        // read == 0 -> EOF // read == -1 -> erro
        while(read(vetor_pipe[0], buffer, sizeof(buffer)) > 0) {
            printf("Processo filho leu: %s\n", buffer);
        }
        printf("Processo filho recebeu EOF\n");
        
        // Fecha caminho de leitura
        close(vetor_pipe[0]);
        printf("Processo filho encerrado!\n");
        exit(EXIT_SUCCESS);
    } else {
        // PROCESSO PAI

        // Fecha caminho de leitura
        close(vetor_pipe[0]);

        // Escreve
        write(vetor_pipe[1], argv[1], tamanho);
        printf("Processo pai escreveu: %s\n", argv[1]);
        sleep(1);
        
        // Fecha caminho de escrita -> EOF
        close(vetor_pipe[1]);
        printf("Processo pai envia EOF\n");
        wait(NULL);

        printf("Processo pai encerrado!\n");
        exit(EXIT_SUCCESS);
    }

    return 0;
}