/* soluzione che non ha bisogno di usare array dinamici dato che il numero di processi e' noto staticamente e pari a due */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define N 2 /* numero di figli da creare: il valore e' COSTANTE! */

typedef int pipe_t[2];

int main(int argc, char **argv)
{
	int pid; 			/* process identifier dei processi generati tramite fork */
	int pidFiglio, status;		/* per valore di ritorno figli */
	pipe_t pipedFP[N]; 		/* array STATICO di pipe usate da primo figlio e dal secondo figlio per inviare le informazioni al padre */
	pipe_t pipedPF[N]; 		/* array STATICO di pipe usate dal padre ai figli per richiedere o meno l'invio della linea (si poteva in alternativa usare un segnale) */
	int i, j; 			/* variabili di supporto usate nei cicli for */
	int fd; 			/* file descritor del file aperto dai figli in lettura */
	char linea[255]; 		/* linea letta dai figli (supposta al massimo di 255 compreso il terminatore di linea) */
	int L;				/* indice per la lettura di un singolo carattere da file che poi rappresentera' la lunghezza della linea corrente (in questo caso compreso il terminatore di linea) da mandare al padre */
	char ok;			/* variabile per inviare 's' o 'n' ai figli: rispettivamente manda o non mandare la linea corrente */
	int L1, L2;			/* variabili usate dal padre per leggere le lunghezze della linea corrente inviate da ognuno dei due figli */
	int nroLinee=0;			/* numero di linee calcolato dal padre */
	int ritorno=0;			/* variabile per valore di ritorno dei figli: numero di linee inviate al padre, su sua richiesta */
	
if (argc != 3)
{
	printf("Numero di parametri errati: ci vogliono solo i nomi dei due file\n");
	exit(1);
}

/* creazione delle 2 pipe per la comunicazione figli-padre e delle 2 pipe padre-figli: nota bene non abbiamo bisogno dell'allocazione dinamica perche' abbiamo definito due array statici di pipe! */
for(i = 0; i < N; i++)
{
	if(pipe(pipedFP[i]) < 0)
	{
		printf("Errore nella creazione della pipe figlio-padre numero %d\n", i);
		exit(2);
	}
	if(pipe(pipedPF[i]) < 0)
	{
		printf("Errore nella creazione della pipe padre-figlio numero %d\n", i);
		exit(3);
	}
}

/* genero i 2 figli */
for(i = 0; i < N; i++)
{
	if((pid = fork()) < 0)
	{
		printf("Errore nella creazione di un figlio\n");
		exit(4);
	}

	if(pid == 0)
	{
	/* codice del figlio */
		printf("Sono il figlio %d di indice %d\n", getpid(), i);

		/* chiusura pipes inutilizzate */
		for (j=0;j<N;j++)
		{ /* il figlio non legge dai nessuna pipeFP e non scrive su nessuna pipePF */
			close (pipedFP[j][0]);
			close (pipedPF[j][1]);
			if (j != i) 
 			{ /* inoltre non scrive e non legge se non su/dalle sue pipe */
				close(pipedFP[j][1]);
				close(pipedPF[j][0]);
	 		}
		}

		if ((fd = open(argv[i+1], O_RDONLY)) < 0)
		{   	printf("Errore in apertura file %s\n", argv[i+1]);
    			exit(-1); /* in caso di errore decidiamo di tornare -1 che sara' interpretato come 255, che NON e' un valore accettabile per il padre */
		}

		/* con un ciclo leggiamo tutte le linee e ne calcoliamo la lunghezza */
		L=0; /* valore iniziale dell'indice */
		while(read(fd,&(linea[L]),1) != 0)
		{	
			if (linea[L] == '\n') /* siamo arrivati alla fine di una linea */
        		{	
 				L++; /* incrementiamo L per tenere conto anche del terminatore di linea */
      				/* comunichiamo L al processo padre */
      				write(pipedFP[i][1],&L,sizeof(L));
				/* il figlio deve aspettare l'indicazione dal padre se mandare o meno la linea */
				read(pipedPF[i][0], &ok, sizeof(ok));
				//printf("DEBUG FIGLIO %d-valore di ok %c\n", getpid(), ok);
				if (ok == 's') /* bisogna mandare la linea al padre */
      				/* comunichiamo la linea al processo padre */
      			 	{
					write(pipedFP[i][1],linea, L); /* N.B. Si scrivono un numero di caratteri uguali a L, che Ã¨ la lunghezza inviata al padre! */
                			ritorno++;    /* se abbiamo inviato la linea incrementiamo il conteggio */
 				}
                		L = 0;  /* azzeriamo l'indice per la prossima linea */
        		}
        		else L++;
		}

		exit(ritorno);
	}
} /* chiusura for */

/* codice del padre */
/* chiusura pipes inutilizzate */
for (j=0;j<N;j++)
{ /* il padre non legge dai nessuna pipePF e non scrive su nessuna pipeFP */
	close (pipedFP[j][1]);
	close (pipedPF[j][0]);
}

/* Nota bene: dato che i file letti dai figli hanno lunghezza in linee uguale, basta fare un ciclo while sulla lettura dal primo figlio; se tale lettura va a buon fine, significa che anche quella dal secondo figlio andra' a buon fine */
while (read(pipedFP[0][0], &L1, sizeof(L1)))
{ 	
	nroLinee++; /* incrementiamo il numero di linee */
	/* ricevuta la lunghezza dal primo figlio, il padre puo' passare a ricevere la lunghezza inviata dal secondo figlio */
	read(pipedFP[1][0], &L2, sizeof(L2));
	//printf("DEBUG-valore di L1 %d e L2 %d\n", L1, L2);
	if (L1 == L2)	/* se sono uguali il padre chiede ad entrambi i figli di inviare le linee */
		ok='s';
	else 	ok='n';
	//printf("DEBUG-valore di ok %c\n", ok);
	for (i=0;i<N;i++)
		write(pipedPF[i][1], &ok, sizeof(ok));
	if (ok == 's') 
	{	/* se il padre ha inviato 's' allora deve ricevere le due linee e stamparle con l'indicazione del numero di linea */
		printf("PADRE-Ho ricevuto per la linea numero %d la seguente informazione dal primo figlio\n", nroLinee);
		read(pipedFP[0][0], linea, L1); /* N.B. Si legge un numero di caratteri uguale a L1 (che a sua volta e' uguale a L2 */
		/* il padre deve convertire la linea in una stringa per poterla stampare con %s e quindi dobbiamo sostituire il terminatore di linea con il terminatore di stringa: lo sostituiamo e non lo aggiungiamo tanto il Ã¨padre usera' \n per andare a capo */
     		linea[L1-1]='\0';
        	printf("%s\n", linea);
		printf("PADRE-mentre ho ricevuto la seguente informazione dal secondo figlio\n");
		read(pipedFP[1][0], linea, L1);
 		/* il padre deve convertire la linea in una stringa come spiegato sopra */
    		linea[L1-1]='\0';
        	printf("%s\n", linea);
     	}
	else 
 	 	/* se ha inviato 'n' allora deve stampare una indicazione generica sempre con l'indicazione del numero di linea */
		printf("PADRE-La linea numero %d risulta essere di lunghezza diversa nel primo e nel secondo file\n", nroLinee);
}

/* Il padre aspetta i figli */
for (i=0; i < N; i++)
{
        pidFiglio = wait(&status);
        if (pidFiglio < 0)
        {
                printf("Errore in wait\n");
                exit(5);
        }

        if ((status & 0xFF) != 0)
                printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else
                { 
			ritorno=(int)((status >> 8) & 0xFF);
                  	printf("Il figlio con pid=%d ha ritornato il valore %d (se 255 problemi!)\n", pidFiglio, ritorno);
                }
}

exit(0);
}