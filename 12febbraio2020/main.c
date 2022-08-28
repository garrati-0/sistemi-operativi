/* Soluzione della Prova d'esame del 12 Febbraio 2020 - Parte C */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <ctype.h>

typedef int pipe_t[2];

typedef struct
	{
	long int c1; /* c1 e c2 sono i conteggi effettuati dai figli pari e dispari e che devono essere inviati al padre */
	long int c2;
	} str_cont;

int main(int argc, char **argv)
{
        /* -------- Variabili locali ---------- */
        int pid;                        /* process identifier per le fork() */
        int N;                          /* numero di file passati sulla riga di comando */
        int pidFiglio, status;          /* variabili per la wait */
	pipe_t *p;			/* array dinamico di pipe per la comunicazione fra figli e padre */
	str_cont conteggi;    		/* struttura per figli e padre */
	long int cont1 = 0, cont2 = 0;	/* variabili di conteggio per i figli, entrambe inizializzate a 0 */
	int fd;                         /* file descriptor che serve ai figli */
	char ch;                        /* variabile che serve per leggere i caratteri da parte dei figli */
        int i, k;                       /* indici per i cicli */
        int ritorno;                    /* variabile che viene ritornata da ogni figlio al padre */
       	/* ------------------------------------ */

	/* controllo parametri: almeno due file */
	if (argc < 3) /* Meno di due parametri */
        {
                printf("Errore nel numero dei parametri %d\n", argc);
                exit(1);
        }

	/* Controllo sul numero di parametri: pari */
	if ((argc-1) % 2 != 0) /* dispari*/  
	{
		printf("Errore nel numero dei parametri %d\n", argc);
		exit(2);
	}

	/* Calcoliamo il numero di file passati */
	N=argc-1;

	printf("Numero processi da creare %d\n", N);
	
	/* allocazione dinamica memoria per pipe */
	p=(pipe_t*)malloc(N*sizeof(pipe_t));
	if (p==NULL)
	{
		printf("Errore nelle malloc\n");
		exit(3);
	}
		
	/* creazione pipe */
	for (i=0; i < N; i++)
	{
		if(pipe(p[i])!=0)
	        {
                	printf("Errore creazione delle pipe\n"); 
			exit(4);
        	}
	}

	/* creazione figli */
	for (i=0; i < N; i++)
	{
		if((pid=fork())<0)
		{
			printf("Errore nella fork %d-esima\n", i);
			exit(5);
		}	

		if(pid==0)
		{
			printf("Figlio con indice %d e pid %d analizza file %s\n",i,getpid(),argv[i+1]);
			for (k=0; k < N; k++)
			{
				/*chiudo i lati inutilizzati*/
				close(p[k][0]);
				if (k != i)
					close(p[k][1]);
			}
			 
			/* il figlio deve aprire il suo file in lettura */
			if((fd=open(argv[i+1],O_RDONLY))<0)
			{
				printf("Errore apertura filei %s", argv[i+1]);
				exit(-1);  /* decidiamo di tornare -1 che verra' interpretato dal padre come 255 e quindi un valore non ammissibile dato che il padre si aspetta o 0 o 1! */
			}

		/* dobbiamo distinguere il codice dei figli dispari da quello dei figli pari */
		if (i % 2) /* se figlio dispari, leggo a vuoto*/
			read(fd,&ch,1);	/* si puo' supporre (garantito dalla parte shell che i file dispari non siano mai vuoti! */
		while (read(fd,&ch,1))
		{
                	if ( (ch%2) == 0) /* se il carattere ha codice ASCII pari */
                	{
                        	if ((i%2) == 0) /* se figlio pari, aggiorniamo cont1 */
                                	cont1++;
                        	else	/* altrimenti se figlio dispari, aggiorniamo cont2 */
                      	             	cont2++;
                	}	
                	else /* se il carattere ha codice ASCII dispari */
                	{
                        	if (i%2)
                                	cont1++;  /* se figlio dispari, aggiorniamo cont1 */
                        	else 	  /* altrimenti se figlio pari, aggiorniamo cont2 */
                                	cont2++;
                	}	
			read(fd,&ch,1);	/* lettura a vuoto per saltare carattere inutile, sia per i processi pari che per quelli dispari */
		}
		/* terminato il file dobbiamo confezionare la struttura e la dobbiamo mandare al padre */
		conteggi.c1 = cont1;
		conteggi.c2 = cont2;
	  	write(p[i][1],&conteggi,sizeof(conteggi));
		/* stabiliamo quindi che valore dobbiamo ritornare al padre */
		if (cont1 > cont2)
			ritorno = 0;
		else	ritorno = 1;
		exit(ritorno);
		}
	}

	/* codice del padre */
	/* chiude lati che non usa */
	for(i=0;i < N; i++)
		close(p[i][1]);

	/* si legge prima da pipe pari poi da dispari, come specificato dal testo */
	for(i=0; i < N; i+=2)
	{
		read(p[i][0],&conteggi,sizeof(conteggi));
		printf("Figlio con indice pari %d ha calcolato: c1=%ld e c2=%ld\n",i,conteggi.c1,conteggi.c2);
	}
	for(i=1; i < N; i+=2)
	{
		read(p[i][0],&conteggi,sizeof(conteggi));
		printf("Figlio con indice dispari %d ha calcolato: c1=%ld e c2=%ld\n",i,conteggi.c1,conteggi.c2);
}	

	/* Attesa della terminazione dei figli */
	for(i=0; i < N; i++)
	{
   		pidFiglio = wait(&status);
   		if (pidFiglio < 0)
   		{
      			printf("Errore wait\n");
      			exit(6);
   		}
   		if ((status & 0xFF) != 0)
                	printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
   		else
   		{
               		ritorno=(int)((status >> 8) & 0xFF);
               		if (ritorno==255)
                                printf("Il figlio con pid=%d ha ritornato %d e quindi vuole dire che ci sono stati dei problemi\n", pidFiglio, ritorno);
                        else  printf("Il figlio con pid=%d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}	
exit(0);
}/* fine del main */