/* Soluzione della Prova d'esame del 13 Settembre 2017 - Parte C */
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
	int pid;			/* process identifier per le fork() */
	int N; 				/* numero di file passati sulla riga di comando */
	int status;			/* variabile di stato per la wait */
	int fdout;			/* file decriptor per il file creato dal padre */
	int fd;       			/* file descriptor per il file aperto da ogni figlio */
	pipe_t *piped;			/* array dinamico di pipe descriptor per comunicazioni figli-padre  */
	int i, j;			/* indici per i cicli */
	char linea[250]; 		/* variabile che serve per leggere le linee sia da parte del figlio che del padre */
					/* bastano 250 caratteri per contenere ogni riga insieme con il terminatore di linea (lo dice il testo) */
	int lung; 			/* variabile che mantiene la lunghezza della linea mandata dal figlio al padre */
	int ritorno; 			/* variabile che mantiene il valore ritornato dal figlio al padre */
	/* ------------------------------------ */
	
	/* Controllo sul numero di parametri */
	if (argc < 3 ) /* almeno due parametri */  
	{
		printf("Errore nel numero dei parametri\n");
		exit(1);
	}

	/* Calcoliamo il numero di file passati */
	N = argc - 1;
	
	/* Allocazione dell'array di N pipe descriptor */
	piped = (pipe_t *) malloc (N*sizeof(pipe_t));
	if (piped == NULL)
	{
		printf("Errore nella allocazione della memoria\n");
		exit(2);
	}
	
	/* Creazione delle N pipe figli-padre */
	for (i=0; i < N; i++)
	{
		if (pipe(piped[i]) < 0)
		{
			printf("Errore nella creazione della pipe\n");
			exit(3);
		}
	}

	/* Creazione file Ultime_Linee */
	if ((fdout=creat("Ultime_Linee",0644)) < 0)
	{
        	printf("Impossibile creare il file Ultime_Linee\n");
		exit(4);
	}

	/* Ciclo di generazione dei figli */
	for (i=0; i < N; i++)
	{
		if ( (pid = fork()) < 0)
		{
			printf("Errore nella fork\n");
			exit(4);
		}
		
		if (pid == 0) 
		{
			/* codice del figlio */
			/* stampa di controllo non richiesta */
			printf("Sono il processo figlio di indice %d e pid %d: sono associato al file %s\n", i, getpid(), argv[i+1]);
			/* Chiusura delle pipe non usate nella comunicazione con il padre  */
			for (j=0; j < N; j++)
			{
				close(piped[j][0]);
				if (i != j) close(piped[j][1]);
			}
  			/* apertura file */
        		if ((fd=open(argv[i+1],O_RDONLY)) < 0)
        		{	
        			printf("Impossibile aprire il file %s\n", argv[i+1]);
         			exit(-1); /* in caso di errori nei figli decidiamo di tornare -1 che corrispondera' per il padre al valore 255 che non puo' essere un valore accettabile di ritorno */
        		}

			/* adesso il figlio legge dal file fino a che ci sono caratteri e cioe' linee */
			j=0; /* inizializziamo l'indice della linea */
		       	while (read(fd, &(linea[j]), 1))
			{	
				/* se siamo arrivati alla fine di una linea */
				if (linea[j] == '\n')  
				/* si deve salvare il valore dell'indice prima di azzerarlo perche' non sappiamo se e' l'ultima linea */
					{ lung=j; j=0; }
				else
					j++;
			}	
			/* quando si esce dal while di lettura, nell'array linea abbiamo l'ultima linea ricevuta */
			/* il figlio comunica al padre la linea con una write che passa sempre 250 caratteri anche se la linea (come dice il testo sara' piu' corta: questo serve cosi' il padre potra' leggere con una unica read (come indicato dal testo) */
			write(piped[i][1], linea, 250);
			/* il figlio deve ritornare la lunghezza (reale) della linea compreso il terminatore e quindi lung+1 */
			exit(lung+1);
  		}	
	}

	/* Codice del padre */
	/* Il padre chiude i lati delle pipe che non usa */
	for (i=0; i < N; i++)
		close(piped[i][1]);

	/* Il padre recupera le informazioni dai figli in ordine dei file e quindi di indice */

	for (i=0; i < N; i++)
	{
		/* il padre recupera la linea spedita da ogni figlio leggendo da ogni pipe con una unica read */
		read(piped[i][0], linea, 250);
		/* per poter scrivere sul file creato solo le linee bisogna che il padre recuperi via via i caratteri fino al terminatore di linea */
		j=0;
		while (linea[j] != '\n')
		{
			/* si scrivono via via i caratteri della ultima linea inviata dal figlio i-esimo */
			write(fdout, &(linea[j]), 1);
			j++;
		}
		/* si deve quindi scrivere anche il terminatore di linea */
	        write(fdout, &(linea[j]), 1);

	}	
	/* Il padre aspetta i figli */
	for (i=0; i < N; i++)
	{
		pid = wait(&status);
		if (pid < 0)
		{
		printf("Errore in wait\n");
		exit (10);
		}

		if ((status & 0xFF) != 0)
    		printf("Figlio con pid %d terminato in modo anomalo\n", pid);
    		else
		{ 
			ritorno=(int)((status >> 8) &	0xFF); 
		  	if (ritorno==255)
 				printf("Il figlio con pid=%d ha ritornato %d e quindi vuole dire che il figlio non e' riuscito ad aprire il proprio file\n", pid, ritorno);
		  	else  printf("Il figlio con pid=%d ha ritornato %d\n", pid, ritorno);
		}
	}
	exit(0);
}