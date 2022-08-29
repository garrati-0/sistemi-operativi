/* Soluzione della Prova d'esame del 14 Febbraio 2017 - Parte C */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

typedef int pipe_t[2];

int mia_random(int n)
{
int casuale;
casuale = rand() % n;
return casuale;
}

int main(int argc, char **argv) 
{
	/* -------- Variabili locali ---------- */
	int pid;			/* process identifier per le fork() */
	int N; 				/* numero di file passati sulla riga di comando (uguale al numero di file) */
	int H; 				/* numero passato come ultimo parametro e che rappresenta la lunghezza in linee dei file passati sulla riga di comando */
	int fd; 			/* file descriptor per apertura file */
	int status;			/* variabile di stato per la wait */
	pipe_t *pipedFP;		/* array dinamico di pipe descriptors per comunicazioni figli-padre */
	pipe_t *pipedPF;		/* array dinamico di pipe descriptors per comunicazioni padre-figli */
	int i, j;			/* indici per i cicli */
	char linea[512];		/* array di caratteri per memorizzare la linea, supponendo una lunghezza massima di ogni linea di 512 caratteri compreso il terminatore di linea */
	int nlinee=0;           	/* variabile per contare il numero di linea */
	int valore; 			/* variabile che viene usata dal padre per recuperare il valore comunicato da ogni figlio e che contiene la lunghezza della linea corrente */
	int giusto; 			/* variabile che viene usata dal padre per trovare il valore minimo fra quelli inviati dai figli */
	int r; 				/* variabile usata dal padre per calcolare i valori random */
	int ritorno=0; 			/* variabile che viene ritornata da ogni figlio al padre e che contiene il numero di caratteri scritti sul file (supposta minore di 255) */
	/* ------------------------------------ */
	
	/* Controllo sul numero di parametri */
	if (argc < 4) /* Meno di cinque parametri */  
	{
		printf("Errore nel numero dei parametri\n");
		exit(1);
	}
	printf("Sono il padre con pid %d\n", getpid());
	srand(time(NULL)); /* inizializziamo il seme per la generazione random di numeri  */

	/* Calcoliamo il numero di file passati */
	N = argc - 2;
	/* convertiamo l'ultima stringa in un numero */
	H = atoi(argv[argc-1]);
	/* controlliamo il numero H: deve essere strettamente positivo e minore (o uguale) a 255 */
	if ((H <= 0) || (H >= 255))
	{
		printf("Errore nel numero passato %d\n", H);
		exit(2);
	}

	/* Allocazione dei due array di N pipe descriptors*/
	pipedFP = (pipe_t *) malloc (N*sizeof(pipe_t));
	pipedPF = (pipe_t *) malloc (N*sizeof(pipe_t));
	if ((pipedFP == NULL) || (pipedPF == NULL))
	{
		printf("Errore nella allocazione della memoria\n");
		exit(3);
	}
	
	/* Creazione delle N pipe figli-padre e delle N pipe padre-figli */
	for (i=0; i < N; i++)
	{
		if(pipe(pipedFP[i]) < 0)
		{
			printf("Errore nella creazione della pipe figli-padre\n");
			exit(4);
		}
		if(pipe(pipedPF[i]) < 0)
		{
			printf("Errore nella creazione della pipe padre-figli\n");
			exit(5);
		}

	}
	
/* Ciclo di generazione dei figli */
	for (i=0; i < N; i++)
	{
		if ( (pid = fork()) < 0)
		{
			printf("Errore nella fork\n");
			exit(6);
		}
		
		if (pid == 0) 
		{
			/* codice del figlio */
			printf("Sono il processo figlio di indice %d e pid %d e sono associato al file %s\n", i, getpid(), argv[i+1]);
			/* Chiusura delle pipe non usate nella comunicazione con il padre */
			for (j=0; j < N; j++)
			{
				close(pipedFP[j][0]);
				close(pipedPF[j][1]);
				if (i != j)  {
						close(pipedFP[j][1]);
						close(pipedPF[j][0]);
						}
			}
			/* apertura del file associato in sola lettura */
			if ((fd=open(argv[i+1], O_RDONLY)) < 0)
			{
                                printf("Errore nella open del file %s\n", argv[i+1]);
                                exit(-1); /* decidiamo di tornare -1 che verra' interpretato dal padre come 255 e quindi un valore non ammissibile! */
                       	}
			/* adesso il figlio legge dal file una linea alla volta */
			j=0;
		        while (read(fd, &(linea[j]), 1))
			{
				if (linea[j] == '\n') 
			 	{ /* dobbiamo mandare al padre la lunghezza della linea corrente compreso il terminatore di linea (come int) e quindi incrementiamo j */
				   	j++;
					nlinee++; /* incrementiamo il numero di linee */
				   	write(pipedFP[i][1], &j, sizeof(j));
			/* il figlio Pi deve leggere il valore inviato dal padre */
				   	read(pipedPF[i][0], &r, sizeof(r));
			/* il figlio deve scrivere sullo standard output il carattere corrispondente all'indice inviato dal padre (calcolato in modo random), oltre al suo indice d'ordine come figlio e al suo pid */
					printf("Il figlio %d-esimo con pid %d ha letto nella posizione %d della linea %d del file %s il carattere %c\n", i, getpid(), r,nlinee,  argv[i+1],  linea[r]);
			/* il figlio incrementa il valore di ritorno */
					ritorno++;
			   	    	j=0; /* azzeriamo l'indice per le prossime linee */
		                }
				else j++; /* continuiamo a leggere */
			}
			/* il figlio Pi deve ritornare al padre il valore corrispondente al numero di caratteri scritti sul file */
			exit(ritorno);
  		}	
	}
	
/* Codice del padre */
/* Il padre chiude i lati delle pipe che non usa */
	for (i=0; i < N; i++)
 	{
		close(pipedFP[i][1]);
		close(pipedPF[i][0]);
        }

/* Il padre recupera le informazioni dai figli: prima in ordine di linee e quindi in ordine di indice */
	for (j=1; j <= H; j++)
	{	giusto = 513; /* per ogni linea, il padre inizializza il valore del minimo al valore massimo che, per ipotesi, non puÃ² essere raggiunto dall'indice di linea */ 
		for (i=0; i < N; i++)
		{ 
			/* il padre recupera tutti i valori interi dai figli */
			read(pipedFP[i][0], &valore, sizeof(valore));
			/* il padre deve calcolare qual e' il valore minimo inviato dai figli, per poi, su quello usare la funzione random */
			if (valore < giusto) 
				giusto=valore;
		}	
/* il padre calcola in modo random l'indice della linea che inviera' a tutti i figli */
		r=mia_random(giusto);
                /* stampa di controllo */
		/* printf("Il numero generato in modo random compreso fra 0 e %d per selezionare l'indice della linea e' %d\n", giusto-1, r); */
/* il padre deve inviare a tutti i figli l'indice */
		for (i=0; i < N; i++)
		{
			write(pipedPF[i][1], &r, sizeof(r));
		}	
	}	

	/* Il padre aspetta i figli */
	for (i=0; i < N; i++)
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
		{ ritorno=(int)((status >> 8) &	0xFF); 
		  printf("Il figlio con pid=%d ha ritornato %d\n", pid, ritorno);
		}
	}
exit(0);
}