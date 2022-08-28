/* Soluzione della parte C del compito del 20 Gennaio 2021 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

typedef int pipe_t[2];

int main (int argc, char **argv)
{
int Q; 			/* numero di file */
int pid;		/* pid per fork */
pipe_t *pipes;		/* array di pipe usate a ring da primo figlio, a secondo figlio .... ultimo figlio e poi a primo figlio: ogni processo legge dalla pipe q e scrive sulla pipe (q+1)%Q */
int q,j; 		/* indici */
int fd; 		/* file descriptor */
int pidFiglio, status, ritorno;	/* per valore di ritorno figli */
char ch;        	/* carattere letto dai figli dal proprio file */
char ok;	      	/* carattere letto dai figli dalla pipe precedente e scritta su quella successiva */
int k=0;		/* fattore moltiplicativo */ 
long int cur;		/* conteggio caratteri letti per capire quando siamo su quello giusto che deve essere stampato: nota bene deve essere un long int! */ 
			/* OSSERVAZIONE: poichÃ¨ il testo specifica che ogni figlio "legge tutti i caratteri del proprio file associato per operare una selezione" non e' corretto, secondo la specifica, utilizzare solo la lseek per posizionarsi sul carattere giusto e leggere solo quello! */
int nr,nw;              /* variabili per salvare valori di ritorno di read/write da/su pipe */

/* controllo sul numero di parametri almeno 2 file */
if (argc < 3)
{
	printf("Errore numero di parametri\n");
	exit(1);
}

Q = argc-1;
printf("Numero di processi da creare %d\n", Q);

/* allocazione pipe */
if ((pipes=(pipe_t *)malloc(Q*sizeof(pipe_t))) == NULL)
{
	printf("Errore allocazione pipe\n");
	exit(2); 
}

/* creazione pipe */
for (q=0;q<Q;q++)
	if(pipe(pipes[q])<0)
	{
		printf("Errore creazione pipe\n");
		exit(3);
	}

/* creazione figli */
for (q=0;q<Q;q++)
{
	if ((pid=fork())<0)
	{
		printf("Errore creazione figlio\n");
		exit(4);
	}
	else if (pid == 0)
	{ /* codice figlio */
	printf("Sono il figlio %d e sono associato al file %s\n", getpid(), argv[q+1]);
	/* nel caso di errore in un figlio decidiamo di ritornare un valore via via crescente rispetto al massimo valore di q (che e' Q-1) */
	/* chiusura pipes inutilizzate */

	for (j=0;j<Q;j++)
	{	/* si veda commento nella definizione dell'array pipes per comprendere le chiusure */
		if (j!=q)
			close (pipes[j][0]);
		if (j != (q+1)%Q)
			close (pipes[j][1]);
	}
	
	/* apertura file */
	if ((fd=open(argv[q+1],O_RDONLY))<0)
	{
		printf("Impossibile aprire il file %s\n", argv[q+1]);
		exit(Q);
	}
	
	/* inizializziamo il contatore dei caratteri letti che rappresenta la posizione all'interno del file e, come specificato nel testo, la prima posizione e' 0 (in termini di costante LONG!) */
	cur = 0L;
	 /* con un ciclo leggiamo tutti i caratteri, come richiede la specifica */
        while(read(fd,&ch,1) != 0)
        {
          	if (cur == (q+k*Q)) /* siamo sul carattere giusto */
                {
                	nr=read(pipes[q][0],&ok,sizeof(char));
        		/* per sicurezza controlliamo il risultato della lettura da pipe */
                	if (nr != sizeof(char))
                	{
                        	printf("Figlio %d ha letto un numero di byte sbagliati %d\n", q, nr);
                        	exit(Q+1);
                	}
			/* a questo punto si deve riportare su standard output il carattere letto, l'indice e il pid del processo */
                	printf("Figlio con indice %d e pid %d ha letto il carattere %c\n", q, getpid(), ch);
			/* ora si deve mandare l'OK in avanti: nota che il valore della variabile ok non ha importanza */
        		nw=write(pipes[(q+1)%Q][1],&ok,sizeof(char));
        		/* anche in questo caso controlliamo il risultato della scrittura */
        		if (nw != sizeof(char))
        		{
               			printf("Figlio %d ha scritto un numero di byte sbagliati %d\n", q, nw);
               			exit(Q+2);
        		}
			/* NOTA BENE: nell'ultima iterazione l'ultimo figlio mandera' un OK al primo figlio che pero' non verra' ricevuto, ma non creera' alcun problemaa patto che il padre mantenga aperto il lato di lettura di pipes[0]: in questo modo, l'ultimo figlio non incorrera' nel problema di scrivere su una pipe che non ha lettori */
			/* si deve incrementare il fattore di moltiplicazione per individuare il prossimo carattere giusto */
			k++;
        	}	
		cur++; /* incrementiamo il numero di caratteri */
        }
	/* ogni figlio deve tornare il proprio indice d'ordine */
	exit(q);
	}
} /* fine for */

/* codice del padre */
/* chiusura di tutte le pipe che non usa, a parte la prima perche' il padre deve dare il primo OK al primo figlio. N.B. Si lascia aperto sia il lato di scrittura che viene usato (e poi in effetti chiuso) che il lato di lettura (che non cerra' usato ma serve perche' non venga inviato il segnale SIGPIPE all'ultimo figlio che terminerebbe in modo anomalo)  */
for(q=1;q < Q;q++)
{
	close (pipes[q][0]);
	close (pipes[q][1]); 
}
/* ora si deve mandare l'OK al primo figlio (P0): nota che il valore della variabile ok non ha importanza */
nw=write(pipes[0][1],&ok,sizeof(char));
/* anche in questo caso controlliamo il risultato della scrittura */
if (nw != sizeof(char))
{
	printf("Padre ha scritto un numero di byte sbagliati %d\n", nw);
        exit(7);
}
/* ora possiamo chiudere anche l'ultimo lato rimasto aperto */
close(pipes[0][1]);
/* OSSERVAZIONE SU NON CHIUSURA DI pipes[0][0]: se si vuole procedere con la chiusura di tale lato nel padre, bisognerebbe introdurre del codice ulteriore solo nel primo figlio che vada a fare la lettura dell'ultimo OK prima di terminare! */
/* Il padre aspetta i figli */
for (q=0; q < Q; q++)
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
        { 
		ritorno=(int)((status >> 8) & 0xFF);
        	printf("Il figlio con pid=%d ha ritornato %d (se > di %d problemi)\n", pidFiglio, ritorno, Q-1);
        } 
}
exit(0);
}