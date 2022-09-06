#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef int pipe_t[2];

int main(int argc, char **argv)
{
        int pid;        /* process identifier per le fork() */
        int Lun;        /* numero di linee del file */
        int N;          /* numero di caratteri e quindi di processi figli */
        int status;     /* variabile di stato per la wait */
        pipe_t *pipePF; /* array dinamico di pipe descriptors per comunicazioni padre-figli  */
        pipe_t *pipeFP; /* array dinamico di pipe descriptors per comunicazioni figli-padre  */
        int n, i, j;    /* indici per i cicli */
        int cont;       /* contatore occorrenze per ogni linea */
        int fd;         /* per la open nei figli */
        char ch;        /* carattere per la lettura da file */
        char synch;     /* carattere di sincroniccazione*/
        int ritorno;    /* variabile che viene ritornata da ogni figlio al padre */

        /* Controllo sul numero di parametri */
        if (argc < 5) /* Devono essere passati almeno 4 parametri siccome in argc è presente anche il nomeprog bisogna frea +1 quindi argc < 5*/
        {
                printf("Errore nel numero dei parametri\n");
                exit(1);
        }

        // assegniamo a Lun il numero di linee del file
        Lun = atoi(argv[2]);
        if (Lun <= 0) // controlliamo che Lun sia strettamente positivo
        {
                printf("Errore: Il secondo parametro non e' un numero strettamente maggiore di 0\n");
                exit(2);
        }

        /* controlliamo che dal terzo parametro in poi siano singoli caratteri */

        for (i = 3; i < argc; i++)
        {
                if (strlen(argv[i]) != 1)
                {
                        printf("%s non singolo carattere\n", argv[i]);
                        exit(3);
                }
        }

        /* Calcoliamo il numero di caratteri e quindi di processi figli */
        N = argc - 3;

        /* stampa di debugging */
        printf("Padre con pid = %d e con L = %d e Q = %d\n", getpid(), Lun, N);

        /* Allocazione dei 2 array di N pipe descriptors */
        pipePF = (pipe_t *)malloc(N * sizeof(pipe_t));
        if (pipePF == NULL)
        {
                printf("Errore nella allocazione della memoria per pipePF\n");
                exit(4);
        }
        pipeFP = (pipe_t *)malloc(N * sizeof(pipe_t));
        if (pipeFP == NULL)
        {
                printf("Errore nella allocazione della memoria per pipeFP\n");
                exit(5);
        }

        /* Creazione delle N pipe padre-figli e della N pipe figli-padre */
        for (n = 0; n < N; n++)
        {
                if (pipe(pipePF[n]) < 0)
                {
                        printf("Errore nella creazione della pipe\n");
                        exit(6);
                }
                if (pipe(pipeFP[n]) < 0)
                {
                        printf("Errore nella creazione della pipe\n");
                        exit(7);
                }
        }

        /* Ciclo di generazione dei figli */
        for (n = 0; n < N; n++)
        {
                if ((pid = fork()) < 0) // se pid è minore di 0 c'è un errore nella fork
                {
                        printf("Errore nella fork\n");
                        exit(8);
                }

                if (pid == 0)
                {
                        // codice del figlio
                        // in caso di errori nei figli decidiamo di tornare -1 che verra' interpretato come 255 e quindi non puo' essere un valore accettabile di ritorno
                        // printf("Sono il figlio di indice %d e pid %d e sono associato al carattere %c\n", q, getpid(), argv[3+q][0]);

                        /* Chiusura delle pipe non usate nella comunicazione con il padre */
                        for (j = 0; j < N; j++)
                        {
                                close(pipePF[j][1]);
                                close(pipeFP[j][0]);
                                if (n != j)
                                {
                                        close(pipePF[j][0]);
                                        close(pipeFP[j][1]);
                                }
                        }

                        // apriamo il file in lettura
                        if ((fd = open(argv[1], O_RDONLY)) < 0)
                        {
                                printf("Errore: FILE %s NON ESISTE\n", argv[1]);
                                exit(-1);
                        }

                        cont = 0; // valore iniziale del conteggio che andra' resettato per ogni linea
                        while (read(fd, &ch, 1) != 0)
                        {
                                if (ch == argv[3 + n][0])
                                {
                                        /* se il carattere corrente della linea e' uguale al carattere associato a questo processo, allora bisogna incrementare il conteggio */
                                        cont++;
                                }
                                else if (ch == '\n')
                                {
                                        /* se siamo a fine linea, il figlio deve aspettare l'OK del padre per riportare su standard output il numero di occorrenze del carattere associato per la linea corrente */
                                        read(pipePF[n][0], &synch, 1);
                                        printf("Figlio con indice %d e pid %d ha letto %d caratteri %c nella linea corrente\n", n, getpid(), cont, argv[3 + n][0]);
                                        /* ora deve indicare al padre che ha completato la scrittura */
                                        write(pipeFP[n][1], &synch, 1);
                                        ritorno = cont; /* salviamo l'ultimo valore prima di riazzerarlo per non perderlo, nel caso fosse l'ultima linea */
                                        cont = 0;       /* azzeriamo il contatore per la prossima linea */
                                }
                        }

                        /* il figlio ritorna l'ultimo conteggio (supposto minore di 255) */
                        exit(ritorno);
                }
        }

        /* Codice del padre */
        /* Il padre chiude i lati delle pipe che non usa */
        for (n = 0; n < N; n++)
        {
                close(pipePF[n][0]);
                close(pipeFP[n][1]);
        }

        /* Il padre deve mandare per ogni linea e per ogni figlio un 'segnale' per sbloccare i figli che devono scrivere le loro informazioni, ma per prima cosa deve scrivere il numero di linea corrente */
        for (i = 1; i <= Lun; i++)
        {
                printf("Linea %d del file %s\n", i, argv[1]);
                for (n = 0; n < N; n++)
                {
                        write(pipePF[n][1], &synch, 1);
                        /* ora e' il padre che deve aspettare il 'segnale' del figlio corrispondente per proseguire */
                        read(pipeFP[n][0], &synch, 1);
                }
        }
        printf("NON CI SONO PIU' INFORMAZIONI DA FAR STAMPARE AI FIGLI!\n");

        /* Il padre aspetta i figli */
        for (n = 0; n < N; n++)
        {
                pid = wait(&status);
                if (pid < 0)
                {
                        printf("Errore in wait\n");
                        exit(9);
                }

                if ((status & 0xFF) != 0)
                        printf("Figlio con pid %d terminato in modo anomalo\n", pid);
                else
                {
                        ritorno = (int)((status >> 8) & 0xFF);
                        printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi nel figlio)\n", pid, ritorno);
                }
        }
        exit(0);
}
