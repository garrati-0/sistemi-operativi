#!/bin/sh
#Soluzione dell'appello del 12 Luglio 2017
#versione che usa un file temporaneo e usa FCR.sh per tutte le fasi

case $# in
0|1|2)	echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 numero dirass1 dirass2 ...
	exit 1;;
*) 	echo OK: da qui in poi proseguiamo con $# parametri ;;
esac
#Controllo primo parametro sia un numero
case $1 in
*[!0-9]*)       echo Primo parametro $1 non numerico o non positivo
                exit 2;;
*)              if test $1 -eq 0 -o $1 -ge 100 
                then    echo Secondo parametro $2 non strettamente maggiore di 0 oppure non strettamente minore di 100
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
#controlli sui parametri finiti possiamo passare alle N fasi
PATH=`pwd`:$PATH
export PATH
> /tmp/temp$$ #creiamo/azzeriamo il file temporaneo. NOTA BENE: SOLO 1 file temporaneo!

for i
do
	echo fase per $i
 	#invochiamo il file comandi ricorsivo con la gerarchia, il numero e il file temporaneo
	FCR.sh $i $L /tmp/temp$$   
done

#terminate tutte le ricerche ricorsive cioe' le N fasi
#N.B. Andiamo a contare le linee del file /tmp/temp$$ anche se non richiesto e stampiamo i nomi assoluti dei file
files=`wc -l < /tmp/temp$$` 
echo Il numero di file totali che soddisfano la specifica = `expr $files / 2 ` 
echo "che sono i seguenti (intervallati dalla dimensione in linee)"
echo `cat /tmp/temp$$` #usato questo comando invece che il semplice cat /tmp/temp$$ per avere una stampa compatta!
#chiamiamo la parte in C
12Lug17 `cat /tmp/temp$$`

#da ultimo eliminiamo il file temporaneo
rm /tmp/temp$$