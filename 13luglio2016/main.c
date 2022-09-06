/* Soluzione della Prova d'esame del 13 Luglio 2016 - Parte C */
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
	int P; 				/* numero di parametri passati sulla riga di comando */
	int N; 				/* numero di file passati sulla riga di comando (uguale al numero di lunghezze) */
	int *X;				/* numeri richiesti all'utente: li memorizziamo perche' servono al padre per stampare la posizione all'interno del file */
	int fd; 			/* file descriptor per apertura file */
	long int pos; 			/* valore per la lseek */
	int status;			/* variabile di stato per la wait */
	pipe_t *pipedFP;		/* array dinamico di pipe descriptors per comunicazioni figli-padre */
	pipe_t *pipedPF;		/* array dinamico di pipe descriptors per comunicazioni padre-figli */
	int i, j;			/* indici per i cicli */
	int valore; 			/* variabile che viene usata dai figli per recuperare il valore comunicato dal padre e che rappresenta il dividendo da utilizzare */
	char ch; 			/* variabile che viene usata dai figli per leggere dal file */
	int ritorno=0; 			/* variabile che viene ritornata da ogni figlio al padre e che contiene il numero di caratteri letti dal file (supposta minore di 255 e garantito dalla parte shell) */
  	int nr;     			/* variabile che serve al padre per sapere se non ha letto nulla */
  	int finito;     		/* variabile che serve al padre per sapere se non ci sono piu' caratteri da leggere */
      
	/* ------------------------------------ */
	
	/* Controllo sul numero di parametri */
	if (argc < 3) /* Meno di tre parametri */  
	{
		printf("Errore nel numero dei parametri: non almeno due\n");
		exit(1);
	}
	/* Calcoliamo il numero di parametri passati */
	P = argc - 1;
	if (P % 2) /* Non pari */  
	{
		printf("Errore nel numero dei parametri: non pari\n");
		exit(2);
	}
	/* Calcoliamo il numero di file/lunghezze passati */
	N = P/2;

	printf("Sono il padre con pid %d e creo %d figli\n", getpid(), N);

	/* Allocazione dei due array di N pipe descriptor */
	pipedFP = (pipe_t *) malloc (N*sizeof(pipe_t));
	pipedPF = (pipe_t *) malloc (N*sizeof(pipe_t));
	if ((pipedFP == NULL) || (pipedPF == NULL))
	{
		printf("Errore nella allocazione della memoria per le pipe\n");
		exit(3);
	}
	
	/* Creazione delle N pipe figli-padre e delle N pipe padre-figli */
	for (i=0; i < N; i++)
	{
		if (pipe(pipedFP[i]) < 0)
		{
			printf("Errore nella creazione della pipe figli-padre\n");
			exit(4);
		}
		if (pipe(pipedPF[i]) < 0)
		{
			printf("Errore nella creazione della pipe padre-figli\n");
			exit(5);
		}

	}
	
	/* Allocazione dell'array di numeri che verranno chiesti all'utente */
	X = (int *) malloc (N*sizeof(int));
	if (X== NULL)
	{
		printf("Errore nella allocazione della memoria per gli interi\n");
		exit(6);
	}
	/* Ciclo di generazione dei figli */
	for (i=0; i < N; i++)
	{
		if ((pid = fork()) < 0)
		{
			printf("Errore nella fork\n");
			exit(7);
		}
		
		if (pid == 0) 
		{
			/* codice del figlio */
			printf("Sono il processo figlio di indice %d e pid %d e sono associato al file %s\n", i, getpid(), argv[i*2+1]);
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
			/* apertura del file associato in sola lettura: l'indice da usare NON e' il solito i+1, ma va moltiplicato per 2 per saltare le lunghezze! */
			if ((fd=open(argv[i*2+1], O_RDONLY)) < 0)
			{
                                printf("Errore nella open del file %s\n", argv[i*2+1]);
                                exit(-1); /* torniamo -1 che verra' interpretato come 255 dal padre che e' un valore non accettabile */
                       	}
			/* adesso il figlio deve aspettare dal padre il dividendo da usare per la lettura */
		 	read(pipedPF[i][0], &valore, sizeof(valore));
                        /* printf("FIGLIO RICEVUTO %d\n", valore); */
			/* adesso il figlio legge dal file i caratteri multipli del valore ricevuto dal padre */
			j=1; /* ci serve per mantenere traccia del moltiplicatore del divisore */
			pos=(long int)0; /* inizializzazione per entrare il ciclo */
			while (pos < atoi(argv[i*2+2])) /* la lunghezza del file per questo figlio si trova in argv usando l'indice usato per individuare il file incrementato di 1 e chiaramente bisogna usare atoi per convertirlo da stringa a numero intero */
			{
			/* calcoliamo la posizione del carattere che deve essere letto */
		        pos=(long int)(j * valore);
		 	/* printf("Processo di indice %d ha calcolato pos = %ld per il file di lunghezza %d\n", i, pos, atoi(argv[i*2+2]));  */
			/* chiamiamo la lseek passando come offset pos-1 dall'inizio del file: dobbiamo considerare -1 altrimenti leggeremmo il carattere sbagliato */
			lseek(fd, pos-1, SEEK_SET);
			read(fd, &ch, 1);
		 	/* printf("Processo di indice %d ha letto il carattere = %c dal file\n", i, ch); */ 
			/* il figlio Pi deve mandare il carattere letto padre */
			write(pipedFP[i][1], &ch, 1);
			j++; /* se j fosse stato inizializzato a 0 allora questo incremento lo dovevamo inserire come prima istruzione del while */
			/* il figlio incrementa il valore di ritorno */
			ritorno++;
		        }
			/* il figlio Pi deve ritornare al padre il valore corrispondente al numero di caratteri inviati al padre */
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

	/* Il padre deve richiedere all'utente i divisori per ogni figlio/file/lunghezza */
	for (i=0; i < N; i++)
	{ 
		/* NOTA BENE: all'utente deve essere indicato sia il nome del file che la sua lunghezza! */
		printf("Per il processo di indice %d e per il file %s di lunghezza %d, dammi un divisore corretto\n", i, argv[i*2+1], atoi(argv[i*2+2]));
		/* ATTENZIONE: la cosa piu' semplice per leggere una serie di numeri da standard input e' di usare la funzione di libreria scanf; usando la read da 0, bisognava leggere almeno 3 char in un array di almeno 4 char, poi si doveva inserire il terminatore di stringa e quindi convertire questa stringa in un numero con la funzione atoi! */
        	scanf("%d", &X[i]);
		/* printf("Ricevuto %d che mando al figlio corretto\n", X[i]); */
		/* per controllare che sia un divisore corretto, oltre che usare l'operatore modulo (%), bisogna anche controllare che il valore immesso NON sia minore o uguale a 0! */
		if (X[i] <= 0 || atoi(argv[i*2+2])%X[i])
		{
			printf("Errore nel numero fornito: %d minore o uguale a 0 o non divisore di %d\n",  X[i], atoi(argv[i*2+2]));
			exit(8);
		}
		else 	/* se il divisore e' corretto, manda al figlio */
			write(pipedPF[i][1], &X[i], sizeof(int));
 	}
		
	/* Il padre recupera le informazioni dai figli: prima in ordine di caratteri e quindi in ordine di indice */
 	finito = 0; /* all'inizio supponiamo che non sia finito nessun figlio */
        j = 1;
        while (!finito)
        {
		finito = 1;
                for (i=0; i<N; i++)
                {
                 	/* si legge il carattere inviato  dal figlio i-esimo */
                	nr = read(pipedFP[i][0], &ch, 1);
			if (nr != 0)
                        { 
				finito = 0; /* almeno un processo non e' finito */
                		printf("Il figlio di indice %d ha letto dal file %s dalla posizione %d il carattere %c\n", i,  argv[i*2+1], X[i]*j, ch);
                        }
                }
                j++; /* si incrementa il contatore dei caratteri */
         }

	/* Il padre aspetta i figli */
	for (i=0; i < N; i++)
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
			ritorno=(int)((status >> 8) &	0xFF); 
		  	printf("Il figlio con pid=%d ha ritornato %d\n", pid, ritorno);
		}
	}
	exit(0);
}