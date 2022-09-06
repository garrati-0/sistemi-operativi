#!/bin/sh
#Soluzione dell'esame del 15 Luglio 2020
#uso di un file temporaneo che va azzerato per ogni gerarchia e usa FCR.sh per tutte le fasi

case $# in
0|1|2) 	echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 L dirass1 dirass2 ...
	exit 1;;
*) 	echo OK: da qui in poi proseguiamo con $# parametri ;;
esac

#Controllo primo parametro sia un numero strettamente positivo
case $1 in
*[!0-9]*) echo Errore: $1 non numero
          exit 2;;
*) if test $1 -eq 0
   then
        echo $1 non diverso da 0
        exit 3
   fi;;
esac

L=$1 #salviamo il primo parametro (N.B. nella variabile il cui nome viene specificato nel testo)
#quindi ora possiamo usare il comando shift
shift

#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sulle directory in un for
for i 
do
	case $i in
	/*) if test ! -d $i -o ! -x $i
	    then
	    echo $i non directory
	    exit 4
	    fi;;
	*)  echo $i non nome assoluto; exit 5;;
	esac
done

#controlli sui parametri finiti possiamo passare alle Q fasi
PATH=`pwd`:$PATH
export PATH

for i
do
	echo fase per $i
	> /tmp/nomiAssoluti$$ #N.B. creiamo/azzeriamo il file temporaneo tutte le volte (N.B. con il nome specificato nel testo)
 	#invochiamo il file comandi ricorsivo con la gerarchia, il numero e il file temporaneo
	FCR.sh $i $L /tmp/nomiAssoluti$$
	#terminate la ricerca in una gerarchia andiamo a contare le linee del file /tmp/conta$$
	echo Il numero di file totali che sono stati trovati in $i e\' = `wc -l < /tmp/nomiAssoluti$$`
	#ora dobbiamo invocare la parte C per ogni file trovato!
	for file in `cat /tmp/nomiAssoluti$$`	#il nome della variabile del ciclo viene specificato nel testo!
	do
		echo Chiamiamo la parte in C con $file e $L
		15Lug20 $file $L
	done
done

#da ultimo eliminiamo il file temporaneo
rm /tmp/nomiAssoluti$$