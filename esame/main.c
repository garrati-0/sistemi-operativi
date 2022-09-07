#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#define PERM 0644

typedef int pipe_t[2];

int main(int argc, char **argv)
{
    int V;                          /* numero di file/processi */
    int pid;                        /* pid per fork */
    pipe_t *pipes;                  /* array di pipe usate a pipeline da primo figlio, a secondo
                     figlio .... ultimo figlio e poi a padre: ogni processo (a parte il primo) legge dal
                    la pipe i-1 e scrive sulla pipe i */
    int v, j, i;                 /* indici */
    int fd, createdfile;                    /* file descriptor */
    int pidFiglio, status, ritorno; /* per valore di ritorno figli */

    char *curr; /* array dinamico di linee */
    /* ATTENZIONE NOME tutteLinee imposto dal testo! */
    char CHR;     // carattere letto dai figli
    int nr, nw;   /* variabili per salvare valori di ritorno di read/write da/su pipe */
    int cont = 1; /* variabile per contare i caratteri letti */

    /* controllo sul numero di parametri: il quale deve essere strettamente maggiore di 1 */
    if (argc < 3)
    {
        printf("Errore numero di parametri\n");
        exit(1);
    }

    V = argc-1;
    printf("Numero di processi da creare %d\n", V);

    /* allocazione pipe */
    if ((pipes = (pipe_t *)malloc(V * sizeof(pipe_t))) == NULL)
    {
        printf("Errore allocazione pipe\n");
        exit(3);
    }

    /* creazione pipe */
    for (v = 0; v < V; v++)
        if (pipe(pipes[v]) < 0)
        {
            printf("Errore creazione pipe\n");
            exit(4);
        }

    /* allocazione array di caratteri */
    if ((curr = malloc(V * sizeof(char))) == NULL)
    {
        printf("Errore allocazione array di linee\n");
        exit(5);
    }

    /* creiamo il file nella directory corrente avente come nome il mio cognome (TUTTO IN MAIUSCOLO, come specificato nel testo) */
    if ((createdfile = creat("GARRAPA", PERM)) < 0)
    {
        printf("Errore nella creat del file %s\n", "Leonardi");
        exit(6);
    }

    /* creazione figli */
    for (v = 0; v < V; v++)
    {
        if ((pid = fork()) < 0)
        {
            printf("Errore creazione figlio\n");
            exit(7);
        }
        else if (pid == 0)
        { /* codice figlio */
            printf("Sono il figlio %d e sono associato al file %s\n", getpid(), argv[v + 1]);
            /* nel caso di errore in un figlio decidiamo di ritornare il valore -1 che sara' interpretato dal padre come 255 (valore NON ammissibile) */

            /* chiusura pipes inutilizzate */
            for (j = 0; j < V; j++)
            { /* si veda commento nella definizione dell'array pipes per comprendere le chiusure */
                if (j != v)
                    close(pipes[j][1]);
                if ((v == 0) || (j != v - 1))
                    close(pipes[j][0]);
            }

            /* apertura file */
            if ((fd = open(argv[v + 1], O_RDONLY)) < 0)
            {
                printf("Impossibile aprire il file %s\n", argv[v + 1]);
                exit(-1);
            }

            /* con un ciclo leggiamo tutti i caretteri*/
            while (read(fd, &CHR, 1) != 0)
            {

                if (cont % 2 == 0) /* siamo in posizione pari*/
                {
                    if (v != 0)
                    {
                        /* se non siamo il primo figlio, dobbiamo aspettare l'array dal figlio precedente per mandare avanti */
                        nr = read(pipes[v - 1][0], curr, V * sizeof(char));
                        /* per sicurezza controlliamo il risultato della lettura da pipe */
                        if (nr != V * sizeof(char))
                        {
                            printf("Figlio %d ha letto un numero di byte sbagliati %d\n", v, nr);
                            exit(-1);
                        }
                    }
                    /* a questo punto si deve inserire il carattere letto nel posto giusto */

                    curr[v] = CHR;

                    /* ora si deve mandare l'array in avanti */
                    nw = write(pipes[v][1], curr, V * sizeof(char));
                    /* anche in questo caso controlliamo il risultato della scrittura */
                    if (nw != V * sizeof(char))
                    {
                        printf("Figlio %d ha scritto un numero di byte sbagliati %d\n", v, nw);
                        exit(-1);
                    }
                    // ritorniamo l'ultimo carattere
                    ritorno = CHR;
                    cont++;
                }
                else
                {
                    cont++;
                }
            }
            /* ogni figlio deve tornare l'ultimo carattere letto */
            exit(ritorno);
        }
    } /* fine for */

    /* codice del padre */
    /* chiusura di tutte le pipe che non usa */
    for (v = 0; v < V; v++)
    {
        close(pipes[v][1]);
        if (v != V - 1)
            close(pipes[v][0]);
    }

    /* il padre deve leggere tutti gli array di caratteri inviati dall'ultimo figlio */
    for (j = 1; j <= (cont)*2; j++)
    {
        nr = read(pipes[v - 1][0], curr, V * sizeof(char));
        /* per sicurezza controlliamo il risultato della lettura da pipe */
        if (nr != V * sizeof(char))
        {
            printf("Padre ha letto un numero di byte sbagliati %d\n", nr);
            exit(8);
        }

        /* il padre deve scrivere i caratteri sul file creato */
        for (i = 0; i < V; i++)
        {
            if (write(createdfile, &curr[i], 1) != 1)
            {
                printf("Errore nella scrittura del file %s\n", "GARRAPA");
                exit(9);
            }
        }
    }

    /* Il padre aspetta i figli */
    for (v = 0; v < V; v++)
    {
        pidFiglio = wait(&status);
        if (pidFiglio < 0)
        {
            printf("Errore in wait\n");
            exit(9);
        }
        if ((status & 0xFF) != 0)
            printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else
        {
            ritorno = (int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi)\n", pidFiglio, ritorno);
        }
    }
    exit(0);
}