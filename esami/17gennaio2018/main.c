/* Soluzione della parte C del compito del 17 Gennaio 2018 */
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
int pid;	/* pid per fork */
pipe_t *pipes;	/* array di pipe usate a pipeline da primo figlio, a secondo figlio .... ultimo figlio e poi a padre: ogni processo legge dalla pipe i-1 e scrive sulla pipe i */
pipe_t *pipeST;	/* array di pipe usate dal padre per dire ai figli se scrivere o meno */
int i,j,k; 	/* contatori */
int fd; 	/* file descriptor */
int pidFiglio, status, ritorno;	/* per valore di ritorno figli */
char Cx, Cstampa;  	/* carattere da cercare e carattere mandato da ultimo figlio */
char linea[250];	/* supponiamo che bastino 250 caratteri per contenere ogni riga insieme con il terminatore di linea e con il terminatore di stringa */
int nlinee=0; 		/* variabile per contare il numero di linea */
s_occ cur; 		/* struttura usata dal figlio corrente */
s_occ pip; 		/* struttura usata dal figlio e per la pipe */
int nr,nw;		/* variabili per salvare valori di ritorno di read e write da/su pipe */
int stampate=0;		/* variabile per contare il numero di linee stampate che vanno tornate al padre */

if (argc < 4)
{
	printf("Errore numero di parametri\n");
	exit(1);
}

H = atoi(argv[argc-1]);
printf("Numero di linee %d\n", H);
if (H <= 0)
{
	printf("Errore numero linee: non strettamente maggiore di zero\n");
	exit(2); 
}

if (strlen(argv[argc-2]) != 1)
{
	printf("Errore penultimo parametro NON carattere\n");
	exit(3); 
}

Cx = argv[argc-2][0];

N = argc-3;
printf("Numero di processi da creare %d\n", N);

/* allocazione pipe usate in pipeline */
if ((pipes=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
{
	printf("Errore allocazione pipe usate in pipeline\n");
	exit(4); 
}

/* allocazione pipe usate dall'ultimo figlio */
if ((pipeST=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
{
	printf("Errore allocazione pipe usate dall'ultimo figlio\n");
	exit(5);
}
/* creazione pipe */
for (i=0;i<N;i++)
{
	if(pipe(pipes[i])<0)
	{
		printf("Errore creazione pipe usate in pipeline\n");
		exit(6);
	}
	if(pipe(pipeST[i])<0)
	{
		printf("Errore creazione pipe usate dal padre per comunicare con i figli\n");
		exit(6);
	}
}

/* creazione figli */
for (i=0;i<N;i++)
{
	if ((pid=fork())<0)
	{
		printf("Errore creazione figli\n");
		exit(7);
	}
	if (pid==0)
	{ /* codice figlio */
	printf("Sono il figlio %d\n", getpid());
	/* chiusura pipes inutilizzate */
	for (j=0;j<N;j++)
	{
		if (j!=i)
			close (pipes[j][1]);
		if ((i == 0) || (j != i-1))
			close (pipes[j][0]);
        	close(pipeST[j][1]); /* si devono chiudere i lati di scrittura */
		if (i != j) close(pipeST[j][0]); /* si deve lasciare aperta solo il lato di lettura della pipe con il proprio indice */
	}
	 
	/* inizializzazione struttura */
	cur.id = i;
	cur.occ= 0;
	/* apertura file */
	if ((fd=open(argv[i+1],O_RDONLY))<0)
	{
		printf("Impossibile aprire il file %s\n", argv[i+1]);
		/* torniamo un valore 0 dato che non siamo stati in grado di aprire il file e quindi non sono state scritte linee da questo figlio */
		exit(0);
	}
	k = 0; /* indice per linea */
	while(read(fd,&linea[k],1)>0)
	{
		/* controllo carattere */
		if (linea[k] == Cx)
		 	{ cur.occ++; k++; }
		else
			if (linea[k] =='\n')
			{		
				nlinee++;
				k++;
				linea[k]='\0'; /*inseriamo (dopo il terminatore di linea) il terminatore di stringa per stampare con %s */

				if (i!=0)
				{	
					/* lettura da pipe della struttura per tutti i figli a parte il primo */
					nr=read(pipes[i-1][0],&pip,sizeof(s_occ));
					if (nr!=sizeof(pip)) 
						 { 
							printf("Errore in lettura da pipe[%d]\n",i);
							/* torniamo un valore 0 dato che c'e' stato un errore in lettura da pipe */
							exit(0);
						 }
					else	
					if (cur.occ <= pip.occ)
					/* se il numero di occorrenze non e' maggiore dobbiamo passare avanti la struttura ricevuta 
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
						/* torniamo un valore 0 dato che c'e' stato un errore in scrittura su pipe */
						exit(0);
		 			}
					else	
		 			{
						/* si deve aspettare l'indicazione dal padre */
						read(pipeST[i][0], &Cstampa, 1);
						if (Cstampa == 'S')
						{
							printf("Sono il figlio di indice %d, pid %d e questa e' la mia linea corrente (numero %d) %s\n", i, getpid(), nlinee, linea);
							stampate++;
						}
					}
                                /* alla fine ripristino dei dati */
				cur.occ=0;
                                cur.id=i;
                                k=0;
			}
			else k++;	
		}	
	exit(stampate);
	}
} /* fine for */

/* codice del padre */

/* chiusura pipe non utilizzate */
/* chiusura pipes: tutte meno l'ultima in lettura */
for(i=0;i<N;i++)
	{
	close (pipes[i][1]);
	if (i != N-1) close (pipes[i][0]);
	}
/* chiusura pipeST */
for (i=0;i<N;i++)
	close(pipeST[i][0]); /* si devono chiudere solo i lati di lettura */

/* il padre deve leggere H strutture, tante quante sono le linee lette dai figli */
for(j=0;j<H;j++)
{
	read(pipes[N-1][0],&pip,sizeof(s_occ));
	/* printf("Ricevuto per la linea %d che il figlio di indice %d e pid %d ha trovato %d occorrenze del carattere %c\n", j, pip.id, pid[pip.id], pip.occ, Cx); */
	for (i=0; i < N; i++)
        {
        	if (i == pip.id)
                {
                	Cstampa = 'S';
                        write(pipeST[i][1], &Cstampa, 1);
                }
                else
                {
                	Cstampa = 'N';
                        write(pipeST[i][1], &Cstampa, 1);
                }
       }

} 

/* printf("PADRE ASPETTA I FIGLI\n"); */
/* Il padre aspetta i figli */
for (i=0; i < N; i++)
        {
        pidFiglio = wait(&status);
        if (pidFiglio < 0)
                {
                printf("Errore in wait\n");
                exit(8);
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