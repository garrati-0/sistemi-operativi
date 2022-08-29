#!/bin/sh
#Soluzione dell'appello del 14 Febbraio 2018

#controllo sul numero di parametri: X deve essere maggiore o uguale a 2 e quindi X+1 deve essere maggiore o uguale a 3!
case $# in
0|1|2)	echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 numero dirass1 dirass2 ...
	exit 1;;
*) 	echo OK: da qui in poi proseguiamo con $# parametri ;;
esac
#Controllo primo parametro sia un numero
case $1 in
*[!0-9]*)       echo Primo parametro $1 non numerico o non positivo
                exit 2;;
*)              if test $1 -eq 0 -o $1 -ge 255  
                then    echo Secondo parametro $2 non diverso da 0 oppure non minore di 255 
                        exit 3
                fi;;
esac

L=$1 #salviamo il primo parametro
#quindi ora possiamo usare il comando shift
shift
#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sui direttori in un for
for i 
do
	case $i in
	/*) if test ! -d $i -o ! -x $i
	    then
	    echo $i non direttorio
	    exit 4
	    fi;;
	*)  echo $i non nome assoluto; exit 5;;
	esac
done
#controlli sui parametri finiti possiamo passare alle X fasi
PATH=`pwd`:$PATH
export PATH

#creiamo un file temporaneo dove salveremo i nomi dei file trovati nelle invocazioni ricorsive delle varie gerarchie
> /tmp/file$$

for i
do
	echo fase per $i
 	#invochiamo il file comandi ricorsivo con la gerarchia, il numero e il file temporaneo 
	FCR.sh $i $L /tmp/file$$
done

#terminate tutte le ricerche ricorsive cioe' le X fasi
#N.B. Andiamo a contare le linee del file /tmp/file$$ anche se non richiesto e stampiamo i nomi assoluti dei file
files=`wc -l < /tmp/file$$` 
echo Il numero di file totali che soddisfano la specifica $files 
cat /tmp/file$$ 
#chiamiamo la parte in C
main `cat /tmp/file$$` $L 

#da ultimo eliminiamo il file temporaneo
rm /tmp/file$$