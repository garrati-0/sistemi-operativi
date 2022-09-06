#!/bin/sh
#Soluzione dell'esame del 13 Giugno 2018
#uso di un file temporaneo che va azzerato per ogni gerarchia e usa FCR.sh per tutte le fasi

case $# in
0|1|2|3) echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 nomedir Y dirass1 dirass2 ...
	 exit 1;;
*) 	 echo OK: da qui in poi proseguiamo con $# parametri ;;
esac

#Controllo primo parametro sia dato in forma relativa semplice
case $1 in
*/*) echo Errore: $1 non in forma relativa semplice
     exit 2;;
*) ;;
esac

#Controllo secondo parametro sia un numero
case $2 in
*[!0-9]*) echo Errore: $2 non numero
          exit 3;;
*) if test $2 -eq 0
   then
        echo $2 non diverso da 0
        exit 4
   fi;;
esac

D=$1 #salviamo il primo parametro
#quindi ora possiamo usare il comando shift
shift
Y=$1 #salviamo il secondo parametro, diventato primo!
#quindi ora possiamo usare il comando shift
shift

#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sulle directory in un for
for i 
do
	case $i in
	/*) if test ! -d $i -o ! -x $i
	    then
	    echo $i non directory
	    exit 5
	    fi;;
	*)  echo $i non nome assoluto; exit 6;;
	esac
done
#controlli sui parametri finiti possiamo passare alle W fasi
PATH=`pwd`:$PATH
export PATH

for i
do
	echo fase per $i
	> /tmp/conta$$ #N.B. creiamo/azzeriamo il file temporaneo tutte le volte
 	#invochiamo il file comandi ricorsivo con la gerarchia, il nome relativo della directory, il numero e il file temporaneo
	FCR.sh $i $D $Y /tmp/conta$$
	#terminate la ricerca in una gerarchia andiamo a contare le linee del file /tmp/conta$$
	files=`wc -l < /tmp/conta$$`
	echo Il numero di file totali che sono stati trovati e\' = $files
	echo Chiamiamo la parte in C con `cat /tmp/conta$$` e $Y
	13Giu18 `cat /tmp/conta$$` $Y
done
#da ultimo eliminiamo il file temporaneo
rm /tmp/conta$$