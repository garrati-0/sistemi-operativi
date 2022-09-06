/* Soluzione della parte C del compito del 10 Giugno 2015 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

typedef int pipe_t[2];
typedef struct{
        int id; 	/* indice figlio (campo c1 del testo) */
	int occ; 	/* numero occorrenze (campo c2 del testo) */
		} s_occ;

int main (int argc, char **argv)
{
int N; 		/* numero di file */
int H; 		/* numero di linee */
int *pid;	/* array di pid per fork */
pipe_t *pipes;	/* array di pipe usate a pipeline da primo figlio, a secondo figlio .... ultimo figlio e poi a padre: ogni processo legge dalla pipe i-1 e scrive sulla pipe i */
int i,j; 	/* contatori */
int fd; 	/* file descriptor */
int pidFiglio, status, ritorno;	/* per valore di ritorno figli */
char Cx, ch;  	/* carattere da cercare e carattere letto da linea */
s_occ cur; 		/* struttura usata dal figlio corrente */
s_occ pip; 		/* struttura usata dal figlio e per la pipe */
int nr,nw;		/* variabili per salvare valori di ritorno di read e write da/su pipe */

if (argc < 4)
{
	printf("Errore numero di parametri\n");
	exit(1);
}

H = atoi(argv[argc-1]);
printf("Numero di linee %d\n", H);
if (H < 0)
{
	printf("Errore numero linee: non strettamente maggiore di zero\n");
	exit(2); 
}

N = argc-2;
printf("Numero di processi da creare %d\n", N);

/* allocazione pid */
if ((pid=(int *)malloc(N*sizeof(int))) == NULL)
{
	printf("Errore allocazione pid\n");
	exit(3); 
}

/* allocazione pipe */
if ((pipes=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
{
	printf("Errore allocazione pipe\n");
	exit(4); 
}

/* creazione pipe */
for (i=0;i<N;i++)
	if(pipe(pipes[i])<0)
	{
		printf("Errore creazione pipe\n");
		exit(5);
	}

/* chiediamo all'utente per prima cosa un carattere Cx */
printf("Inserire un carattere Cx che verra' cercato dai figli in ogni linea dei file associati\n");
read(0, &Cx, 1);
printf("Carattere letto %c\n", Cx);

/* creazione figli */
for (i=0;i<N;i++)
{
	if ((pid[i]=fork())<0)
	{
		printf("Errore creazione figli\n");
		exit(6);
	}
	if (pid[i]==0)
	{ /* codice figlio */
	printf("Sono il figlio %d\n", getpid());
	/* chiusura pipes inutilizzate */
	for (j=0;j<N;j++)
	{
		if (j!=i)
			close (pipes[j][1]);
		if ((i == 0) || (j != i-1))
			close (pipes[j][0]);
	}
 
	/* inizializzazione struttura */
	cur.id = i;
	cur.occ= 0;
	/* apertura file */
	if ((fd=open(argv[i+1],O_RDONLY))<0)
	{
		printf("Impossibile aprire il file %s\n", argv[i+1]);
		/* torniamo un valore maggiore di 0 dato che non siamo stati in grado di aprire il file */
		exit(1);
	}
	while(read(fd,&ch,1)>0)
	{
		/* controllo carattere */
		if (ch == Cx)
			cur.occ++;
		else
			if (ch=='\n')
			{		
				if (i!=0)
				{	
					/* lettura da pipe della struttura per tutti i figli a parte il primo */
					nr=read(pipes[i-1][0],&pip,sizeof(s_occ));
					if (nr!=sizeof(pip)) 
						 { 
							printf("Errore in lettura da pipe[%d]\n",i);
							/* torniamo un valore maggiore di 0 dato che c'e' stato un errore in lettura da pipe */
							exit(2);
						 }
					else	
					if (cur.occ >= pip.occ)
					/* se il numero di occorrenze non e' minore dobbiamo passare avanti la struttura ricevuta 
					   altrimenti rimane valida la struttura corrente */
					{	
					cur.id = pip.id;
					cur.occ = pip.occ;
					}
				}
				/* comunicazione struttura al figlio seguente  */
				nw=write(pipes[i][1],&cur,sizeof(s_occ));
				if (nw!=sizeof(pip)) 
				{ 
					printf("Errore in scrittura da pipe[%d]\n",i);
					/* torniamo un valore maggiore di 0 dato che c'e' stato un errore in scrittura su pipe */
					exit(3);
		 		}
				else	
		 		{
					/* dopo l'invio ripristino dei dati */
					cur.occ=0;
					cur.id=i;
				}
			}	
		}	
	exit(0);
	}
} /* fine for */

/* codice del padre */
/* chiusura pipe: tutte meno l'ultima in lettura */
for(i=0;i<N;i++)
	{
	close (pipes[i][1]);
	if (i != N-1) close (pipes[i][0]);
	}

/* il padre deve leggere H strutture, tante quante sono le linee lette dai figli */
for(j=0;j<H;j++)
	{
	read(pipes[N-1][0],&pip,sizeof(s_occ));
	printf("Ricevuto per la linea %d che il figlio di indice %d e pid %d ha trovato %d occorrenze del carattere %c\n", j, pip.id, pid[pip.id], pip.occ, Cx);
	} 

/* Il padre aspetta i figli */
for (i=0; i < N; i++)
        {
        pidFiglio = wait(&status);
        if (pidFiglio < 0)
                {
                	printf("Errore in wait\n");
                	exit(7);
                }

        if ((status & 0xFF) != 0)
                printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else
                { ritorno=(int)((status >> 8) & 0xFF);
                  printf("Il figlio con pid=%d ha ritornato %d\n", pidFiglio, ritorno);
                }
        }
exit(0);
}
