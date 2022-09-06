#!/bin/sh
#Soluzione della Prova del 12 Settembre 2018

case $# in
0|1|2|3) echo Errore: numero parametri $# quindi pochi parametri. Usage is $0 H K dirass1 dirass2 ...
         exit 1;;
*)       echo OK: da qui in poi proseguiamo con $# parametri ;;
esac

#Controllo $1
case $1 in
	*[!0-9]*) echo $1 non numerico o non positivo
		  exit 2;;
	*) if test $1 -eq 0
	   then echo ERRORE: secondo parametro $1 uguale a zero
		exit 3
	   fi ;;
esac
H=$1	#salviamo il primo parametro in una variabile di nome H

#Controllo $2
case $2 in
	*[!0-9]*) echo $2 non numerico o non positivo
		  exit 4;;
	*) if test $2 -eq 0
	   then echo ERRORE: terzo parametro $2 uguale a zero
		exit 5
	   fi ;;
esac
K=$2	#salviamo il primo parametro in una variabile di nome K			

#controllo che H sia strettamente minore di K
if test $H -ge $K
then
	echo ERRORE: primo parametro $H NON strettamente minore del secondo parametro $K
	exit 6
fi

#quindi ora possiamo usare il comando shift due volte per eliminare i due numeri
shift
shift

#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sulle directory in un for
for i
do
        case $i in
        /*) if test ! -d $i -o ! -x $i
            then
            echo $i non directory
            exit 7
            fi;;
        *)  echo $i non nome assoluto; exit 8;;
        esac
done
#controlli sui parametri finiti possiamo passare alle W fasi, dopo aver settato il path
PATH=`pwd`:$PATH
export PATH
for i
do
        echo fase per $i
	FCR.sh $i $H $K
done