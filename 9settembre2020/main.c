/* Soluzione della Prova del 9 Settembre 2020: soluzione che usa le pipe (invece che i segnali) */
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
        /* -------- Variabili locali ---------- */
        int pid;                        /* process identifier per le fork() */
        int L;                          /* numero di linee del file */
        int Q;                          /* numero di caratteri e quindi di processi figli */
        int status;                     /* variabile di stato per la wait */
        pipe_t *pipePF;                 /* array dinamico di pipe descriptors per comunicazioni padre-figli  */
        pipe_t *pipeFP;                 /* array dinamico di pipe descriptors per comunicazioni figli-padre  */
        int q, i, j;                    /* indici per i cicli */
        int cont;                    	/* conteggio occorrenze per ogni linea */
	int fd; 			/* per la open nei figli */
	char ch; 			/* carattere per la lettura da file */
	char synch; 			/* carattere di sincroniccazione (NON importa il valore) */
        int ritorno;                    /* variabile che viene ritornata da ogni figlio al padre */

        /* ------------------------------------ */

        /* Controllo sul numero di parametri */
        if (argc < 5 ) /* Devono essere passati almeno 4 parametri */
        {
                printf("Errore nel numero dei parametri\n");
                exit(1);
        }

        /* Calcoliamo il numero passato come secondo parametro */
        L = atoi(argv[2]);
	if (L <= 0) 
	{	
        	printf("Errore: Il secondo parametro non e' un numero strettamente maggiore di 0\n");
        	exit(2);
	}

	/* controlliamo che dal terzo parametro in poi siano singoli caratteri */

        for(i=3; i<argc; i++)
        {
                if(strlen(argv[i])!=1)
                {
                        printf("%s non singolo carattere\n", argv[i]);
                        exit(3);
                }
        }
	
	/* Calcoliamo il numero di caratteri e quindi di processi figli */
	Q=argc - 3;

	/* stampa di debugging */
	printf("Padre con pid = %d e con L = %d e Q = %d\n", getpid(), L, Q);

        /* Allocazione dei 2 array di Q pipe descriptors */
        pipePF = (pipe_t *) malloc (Q*sizeof(pipe_t));
        if (pipePF == NULL)
        {
                printf("Errore nella allocazione della memoria per pipePF\n");
                exit(4);
        }
        pipeFP = (pipe_t *) malloc (Q*sizeof(pipe_t));
        if (pipeFP == NULL)
        {
                printf("Errore nella allocazione della memoria per pipeFP\n");
                exit(5);
        }

        /* Creazione delle Q pipe padre-figli e della Q pipe figli-padre */
        for (q=0; q < Q; q++)
        {
                if (pipe(pipePF[q]) < 0)
                {
                        printf("Errore nella creazione della pipe\n");
                        exit(6);
                }
                if (pipe(pipeFP[q]) < 0)
                {
                        printf("Errore nella creazione della pipe\n");
                        exit(7);
                }
        }

        /* Ciclo di generazione dei figli */
        for (q=0; q < Q; q++)
        {
                if ( (pid = fork()) < 0)
                {
                        printf("Errore nella fork\n");
                        exit(8);
                }

                if (pid == 0)
                {
                        /* codice del figlio */
                        /* in caso di errori nei figli decidiamo di tornare -1 che verra' interpretato come 255 e quindi non puo' essere un valore accettabile di ritorno */
			/* commentiamo la stampa seguente perche' non interferisca con le altre stampe 
			 * printf("Sono il figlio di indice %d e pid %d e sono associato al carattere %c\n", q, getpid(), argv[3+q][0]);
			*/

                        /* Chiusura delle pipe non usate nella comunicazione con il padre */
                        for (j=0; j < Q; j++)
                        {
                                close(pipePF[j][1]);
                                close(pipeFP[j][0]);
                                if (q != j) 
				{
					close(pipePF[j][0]);
					close(pipeFP[j][1]);
                        	}
                        }
                                
			/* apriamo il file in lettura: nota bene tutti i figli aprono lo stesso file perche' devono avere l'I/O pointer separato! */
			if ((fd = open(argv[1], O_RDONLY)) < 0)
			{
				printf("Errore: FILE %s NON ESISTE\n", argv[1]);
				exit(-1);
			}

			cont = 0; /* valore iniziale del conteggio che andra' resettato per ogni linea */
			while (read (fd, &ch, 1) != 0)
			{ 	
				if (ch == argv[3+q][0])
				{
					/* se il carattere corrente della linea e' uguale al carattere associato a questo processo, allora bisogna incrementare il conteggio */
					cont++;
				}
				else 
					if (ch == '\n') 
					{ 
						/* se siamo a fine linea, il figlio deve aspettare l'OK del padre per riportare su standard output il numero di occorrenze del carattere associato per la linea corrente */
						read(pipePF[q][0], &synch, 1);
						printf("%d occorrenze del carattere '%c'\n", cont, argv[3+q][0]);
						/* ora deve indicare al padre che ha completato la scrittura */
						write(pipeFP[q][1], &synch, 1);
						ritorno = cont;	/* salviamo l'ultimo valore prima di riazzerarlo per non perderlo, nel caso fosse l'ultima linea */
		  				cont = 0; 	/* azzeriamo il contatore per la prossima linea */
                			}
			}	

			/* il figlio ritorna l'ultimo conteggio (supposto minore di 255) */
                        exit(ritorno); 
                }
        }

/* Codice del padre */
/* Il padre chiude i lati delle pipe che non usa */
        for (q=0; q < Q; q++)
 	{
                close(pipePF[q][0]);
                close(pipeFP[q][1]);
        }

/* Il padre deve mandare per ogni linea e per ogni figlio un 'segnale' per sbloccare i figli che devono scrivere le loro informazioni, ma per prima cosa deve scrivere il numero di linea corrente */
	for (i=1; i <= L; i++)
 	{
		printf("Linea %d:\n", i);
        	for (q=0; q < Q; q++)
	        {
			write(pipePF[q][1], &synch, 1);
			/* ora e' il padre che deve aspettare il 'segnale' del figlio corrispondente per proseguire */
			read(pipeFP[q][0], &synch, 1);
        	}
        }
	printf("NON CI SONO PIU' INFORMAZIONI DA FAR STAMPARE AI FIGLI!\n");

        /* Il padre aspetta i figli */
        for (q=0; q < Q; q++)
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
                { ritorno=(int)((status >> 8) & 0xFF);
                  printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi nel figlio)\n", pid, ritorno);
                }
        }
        exit(0);
}
