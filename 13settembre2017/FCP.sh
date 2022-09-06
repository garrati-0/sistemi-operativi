#!/bin/sh
#Soluzione dell'appello del 13 Settembre 2017

case $# in
0|1|2)	echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 numero dirass1 dirass2 ...
	exit 1;;
*) 	echo OK: da qui in poi proseguiamo con $# parametri ;;
esac
#Controllo primo parametro sia un numero
case $1 in
*[!0-9]*)       echo Primo parametro $1 non numerico o non positivo
                exit 2;;
*)              if test $1 -eq 0  
                then    echo Secondo parametro $2 non diverso da 0 
                        exit 3
                fi;;
esac

X=$1 #salviamo il primo parametro
#quindi ora possiamo usare il comando shift
shift
#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sui direttori in un for
for i  #ci ricordiamo che il for senza l'indicazione della lista significa implicitamente in $* cioe' per tutti i parametri!
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
#controlli sui parametri finiti possiamo passare alle W fasi
PATH=`pwd`:$PATH
export PATH

for i
do
	echo fase per $i
 	#invochiamo il file comandi ricorsivo con la gerarchia e il numero 
	FCR.sh $i $X    
done
