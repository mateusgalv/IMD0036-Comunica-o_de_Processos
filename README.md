# Comunicação entre Processos em C

Exemplos simples de comunicação entre processos por memória compartilhada e pipes em C.

## Objetivo

Demonstrar a comunicação por **Memória Compartilhada** e **Pipes**:

* **Memória compartilhada** permite que processos compartilhem e alterem dados através de uma região de memória comum.
* **Pipe anônimo** permite a comunicação unidirecional entre processos através de um canal de leitura e escrita.
* O **processo pai** atua como produtor e escreve uma mensagem no pipe.
* O **processo filho** atua como consumidor e lê a mensagem.
* O `fork()` cria o processo filho, que herda os descritores do pipe.

## Compilação e Execução

### Comunicação de Processos por Memória compartilhada
```bash
gcc -Wall -Wextra comunicacao_processos/comunicacao_processos.c -o comunicacao_processos/comunicacao_processos && ./comunicacao_processos/comunicacao_processos
```
### Comunicação de Processos por Pipes
```bash
gcc -Wall -Wextra pipes/pipes.c -o pipes/pipes && ./pipes/pipes 'Olá, mundo!'
```

## Exemplos de saída:

```text
// Memória compartilhada:
Processo PAI iniciado - PID: 12345
Inteiro armazenado na memória compartilhada: 10

Processo FILHO_1 iniciado - PID: 12346
Inteiro armazenado na memória compartilhada: 10
Processo FILHO_1 terminado - PID: 12346
Inteiro armazenado na memória compartilhada: 11

Processo PAI update - PID: 12345
Inteiro armazenado na memória compartilhada: 11

Processo FILHO_2 iniciado - PID: 12347
Inteiro armazenado na memória compartilhada: 11
Processo FILHO_2 terminado - PID: 12347
Inteiro armazenado na memória compartilhada: 22

Processo PAI terminado - PID: 12345
Inteiro armazenado na memória compartilhada: 22
```

```text
// Pipes:
Processo pai escreveu: Olá, mundo!
Processo filho leu: Olá, mundo!
Processo pai manda EOF
Processo filho recebeu EOF
Processo filho encerrado!
Processo pai encerrado!
```

## Conceitos utilizados

* `fork()` — criação do processo filho.
* `wait()` — espera pelo término do processo filho.
* `sleep()` — processo "hiberna" por tempo determinado em segundos.
* `shmget()` — cria ou obtém um segmento de memória compartilhada, retornando seu identificador.
* `shmat()` — anexa o segmento de memória compartilhada ao espaço de endereçamento do processo. // permite acesso
* `shmdt()` — desanexa o segmento de memória compartilhada do espaço de endereçamento do processo. // revoga acesso
* `shmctl()` — remove o segmento de memória compartilhada após seu uso.
* `pipe()` — criação do canal de comunicação.
* `write()` — escrita de dados no pipe.
* `read()` — leitura de dados do pipe.
* `close()` — fechamento dos descritores.
* **EOF** — ocorre quando o extremo de escrita do pipe é fechado e não existem mais escritores.