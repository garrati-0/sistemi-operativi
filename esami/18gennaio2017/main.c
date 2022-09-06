/* Soluzione della parte C del compito del 18 gennaio 2017 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

typedef int pipe_t[2];
typedef struct{
        int id; 	/* indice figlio (campo c1 del testo) */
	int num; 	/* numero corrispondente al carattere numerico (campo c2 del testo) */
		} s;

int main (int argc, char **argv)
{
int N; 				/* numero di file */
int *pid;			/* array di pid per fork */
pipe_t *pipes;  		/* array di pipe usate a pipeline da primo figlio, a secondo figlio .... ultimo figlio e poi a padre: ogni processo legge dalla pipe i-1 e scrive sulla pipe i */
int i,j; 			/* contatori */
int fd; 			/* file descriptor */
int pidFiglio, status, ritorno;	/* per valore di ritorno figli */
char Cn, ch;  			/* carattere trovato e carattere letto */
s *cur; 			/* array di strutture usate dal figlio corrente */
long int somma=0;		/* variabile per salvare la somma dei numeri comunicati dai figli */
int nr;				/* variabile per salvare valori di ritorno di read su pipe */

/* controllo sul numero di parametri: almeno 1 file */
if (argc < 2)
{
	printf("Errore numero di parametri\n");
	exit(1);
}

N = argc-1;
printf("Numero di processi da creare %d\n", N);

/* allocazione pid */
if ((pid=(int *)malloc(N*sizeof(int))) == NULL)
{
	printf("Errore allocazione pid\n");
	exit(2); 
}

/* allocazione pipe */
if ((pipes=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
{
	printf("Errore allocazione pipe\n");
	exit(3); 
}

/* creazione pipe */
for (i=0;i<N;i++)
	if(pipe(pipes[i])<0)
	{
		printf("Errore creazione pipe\n");
		exit(4);
	}

/* creazione figli con salvataggio pid per fare in modo che il padre possa stampare i pid dei figli ricevendo il loro indice */
for (i=0;i<N;i++)
{
	if ((pid[i]=fork())<0)
	{
		printf("Errore creazione figli\n");
		exit(5);
	}
	else if (pid[i]==0)
	{ /* codice figlio */
	printf("Sono il figlio %d\n", getpid());
	/* nel caso di errore in un figlio decidiamo di ritornare il valore -1 che non puo' derivare dalla conversione di un carattere numerico */
	/* chiusura pipes inutilizzate */
	for (j=0;j<N;j++)
	{
		if (j!=i)
			close (pipes[j][1]);
		if ((i == 0) || (j != i-1))
			close (pipes[j][0]);
	}
 
	/* allocazione dell'array di strutture specifico di questo figlio */
	/* creiamo un array di dimensione i+1 anche se leggeremo i strutture, dato che poi ci servira' riempire la i+1-esima struttura con le info del processo corrente! */
	if ((cur=(s*)malloc((i+1)*sizeof(s))) == NULL)
	{
        	printf("Errore allocazione cur\n");
        	exit(-1);
	}
	/* inizializziamo solo il primo campo dell'ultima struttura che e' quella specifica del figlio corrente (nel caso del primo figlio sara' l'unica struttura) */
	cur[i].id = i;
	
	/* apertura file */
	if ((fd=open(argv[i+1],O_RDONLY))<0)
	{
		printf("Impossibile aprire il file %s\n", argv[i+1]);
		exit(-1);
	}
	while(read(fd,&ch,1) > 0)
	{
		/* cerco il primo carattere numerico */
		if (isdigit(ch))
	 	{
		Cn=ch; /* non strettamente necessario, ma fatto per maggior leggibilita' */
		cur[i].num=(int)(Cn-'0');
		 /*
		printf("Sono il figlio di indice %d e pid %d e ho trovato il carattere %c\n", cur[i].id, getpid(), Cn);
		*/
		break; /* al primo carattere numerico trovato smettiamo di leggere dal file */
   		}	
	}
	if (i != 0)
	/* lettura da pipe dell'array di strutture per tutti i figli a parte il primo */
 {		nr=read(pipes[i-1][0],cur,i*sizeof(s));
		if (nr != i*sizeof(s))
        	{	
        	printf("Figlio %d ha letto un numero di strutture sbagliate %d\n", i, nr);
        	exit(-1);
        	}
 /*
		for(j=0;j<i;j++)
		printf("Ho ricevuto da figlio di indice %d trovato il numero %d\n", cur[j].id, cur[j].num);
  */
} 

	/* tutti i figli mandano in avanti, l'ultimo figlio manda al padre un array di strutture (i strutture ricevute dal processo precedente e la i+1-esima la propria) */
/*
	printf("Sto per mandare al figlio seguente %d strutture che sono: \n", i+1);
for(j=0;j<i+1;j++)
	printf("Sto per mandare processo %d trovato il numero %d\n", cur[j].id, cur[j].num);
*/
		write(pipes[i][1],cur,(i+1)*sizeof(s));
	/* si deve tornare il primo carattere numerico letto dal file */
	exit(Cn);
	}
} /* fine for */

/* codice del padre */
/* chiusura pipe: tutte meno l'ultima in lettura */
for(i=0;i<N;i++)
	{
	close (pipes[i][1]);
	if (i != N-1) close (pipes[i][0]);
	}
/* allocazione dell'array di strutture specifico per il padre */
/* creiamo un array di dimensione N cioe' quanto il numero di figli! */
if ((cur=(s*)malloc(N*sizeof(s))) == NULL)
{
  	printf("Errore allocazione cur nel padre\n");
       	exit(6);
}

/* il padre deve leggere l'array di strutture che gli arriva dall'ultimo figlio (con una unica read!) */
nr=read(pipes[N-1][0],cur,N*sizeof(s));
if (nr != N*sizeof(s))
{
        printf("Padre ha letto un numero di strutture sbagliate %d\n", nr);
        exit(7);
}
printf("Padre ha letto un numero di strutture %d\n", N);
/* il padre deve stampare i campi delle strutture ricevute, oltre che calcolare la somma dei numeri comunicati dai figli */
for(i=0;i<N;i++)
{
	printf("Il figlio di indice %d e pid %d ha trovato il numero %d\n", cur[i].id, pid[cur[i].id], cur[i].num);
	somma=somma+(long int)cur[i].num;
}
printf("La somma dei numeri comunicati dai figli e' %ld\n", somma);

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
        { 	ritorno=(int)((status >> 8) & 0xFF);
                printf("Il figlio con pid=%d ha ritornato il carattere numerico %c\n", pidFiglio, ritorno);
        }
}
exit(0);
}