/* Soluzione della Prova del 15 Luglio 2020 */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef int pipe_t[2];
typedef struct {
        int pid;       		/* campo c1 del testo */
        int cardue;	        /* campo c2 del testo */
        char carpen;  		/* campo c3 del testo */
	} Strut;

int main(int argc, char **argv)
{
        /* -------- Variabili locali ---------- */
        int pid;                        /* process identifier per le fork() */
        int L;                          /* numero di linee del file e numero di processi */
        int status;                     /* variabile di stato per la wait */
        pipe_t *piped;                  /* array dinamico di pipe descriptors per comunicazioni figli-padre  */
        int q, i, j;                    /* indici per i cicli */
	int fd; 			/* per la open nei figli */
	char linea[250]; 		/* per leggere una linea (il testo indica che al massimo puo' essere lunga 250 compreso il terminatore di linea */
	int nr;                      	/* variabile per valore di ritorno della read */
        int ritorno;                    /* variabile che viene ritornata da ogni figlio al padre */
        Strut S;                        /* struttura usata dai figli e dal padre */

        /* ------------------------------------ */

        /* Controllo sul numero di parametri */
        if (argc != 3 ) /* Devono essere passati esattamente due parametri */
        {
                printf("Errore nel numero dei parametri\n");
                exit(1);
        }

        /* Calcoliamo il numero passato */
        L = atoi(argv[2]);
	if ((L <= 0) || (L > 255))
	{	
        	printf("Errore: Il secondo parametro non e' un numero strettamente maggiore di 0 e minore/uguale a 255\n");
        	exit(2);
	}

        /* Allocazione dell'array di L pipe descriptors */
        piped = (pipe_t *) malloc (L*sizeof(pipe_t));
        if (piped == NULL)
        {
                printf("Errore nella allocazione della memoria\n");
                exit(3);
        }

        /* Creazione delle L pipe figli-padre */
        for (q=0; q < L; q++)
        {
                if (pipe(piped[q]) < 0)
                {
                        printf("Errore nella creazione della pipe\n");
                        exit(4);
                }
        }

        /* Ciclo di generazione dei figli */
        for (q=0; q < L; q++)
        {
                if ( (pid = fork()) < 0)
                {
                        printf("Errore nella fork\n");
                        exit(6);
                }

                if (pid == 0)
                {
                        /* codice del figlio */
                        /* in caso di errori nei figli decidiamo di tornare 0 dato che il testo indica che le linee possono essere <= 255 e che la loro numerazione parte da 1 e quindi non puo' essere un valore accettabile di ritorno */
			printf("Sono il figlio di indice %d e pid %d\n", q, getpid());

                        /* Chiusura delle pipe non usate nella comunicazione con il padre */
                        for (j=0; j < L; j++)
                        {
                                close(piped[j][0]);
                                if (q != j) close(piped[j][1]);
                        }
                                
			
			/* apriamo il file in lettura: nota bene tutti i figli aprono lo stesso file perche' devono avere l'I/O pointer separato! */
			if ((fd = open(argv[1], O_RDONLY)) < 0)
			{
				printf("Errore: FILE %s NON ESISTE\n", argv[1]);
				exit(0);
			}

			i = 1; /* inizializzo il conteggio delle linee a 1 */
			j = 0; /* valore iniziale dell'indice della linea */
			while (read (fd, &(linea[j]), 1) != 0)
			{ 	if (linea[j] == '\n') 
				{ 
					if (q+1 == i) /* trovata la linea che deve selezionare */
					{ 	
						/* il figlio comunica al padre */
						S.pid=getpid();
						S.cardue=linea[1];
						S.carpen=linea[j-1];
                                		write(piped[q][1], &S, sizeof(S));
						break; /* usciamo dal ciclo di lettura */
					}
					else
  					{       
						j = 0; 	/* azzeriamo l'indice per la prossima linea */
		  				i++; 	/* se troviamo un terminatore di linea incrementiamo il conteggio delle linee */
                			}
      				}
				else j++;
			}	

			/* il figlio ritorna il numero della linea analizzata */
			ritorno=q+1;
                        exit(ritorno); 
                }
        }

/* Codice del padre */
/* Il padre chiude i lati delle pipe che non usa */
        for (q=0; q < L; q++)
                close(piped[q][1]);

/* Il padre recupera le informazioni dai figli: in ordine di indice */
        for (q=0; q < L; q++)
        {
         /* si legge la struttura inviata  dal figlio q-esimo */
            nr = read(piped[q][0], &S, sizeof(S));
            if (nr != 0)
            {
                if (S.cardue == S.carpen) 
			printf("Il figlio di indice %d e pid %d ha trovato che il secondo carattere (%c) e il penultimo carattere (%c) della linea %d-esima del file %s sono UGUALI\n", q, S.pid, S.cardue, S.carpen, q+1, argv[1]);
		else
			/* rispetto al testo caricato questa stampa non era da effettuare, ma visto che alcuni testi avevano l'indicazione di stampare se i caratteri erano diversi, si e' introdotta anche questa stampa per verificare il funzionamento */
                	printf("Il figlio di indice %d e pid %d ha trovato che il secondo carattere (%c) e il penultimo carattere (%c) della linea %d-esima del file %s sono DIVERSI\n", q, S.pid, S.cardue, S.carpen, q+1, argv[1]);

             }
         }

        /* Il padre aspetta i figli */
        for (q=0; q < L; q++)
        {
                pid = wait(&status);
                if (pid < 0)
                {
                	printf("Errore in wait\n");
                	exit(7);
                }

                if ((status & 0xFF) != 0)
                	printf("Figlio con pid %d terminato in modo anomalo\n", pid);
                else
                { ritorno=(int)((status >> 8) & 0xFF);
                  printf("Il figlio con pid=%d ha ritornato %d (se 0 problemi nel figlio)\n", pid, ritorno);
                }
        }
        exit(0);
}