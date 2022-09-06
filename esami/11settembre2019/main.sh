#!/bin/sh
#Soluzione della Prova dell'11 Settembre 2019

case $# in
0|1|2|3) echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 H dirass1 dirass2 ...
         exit 1;;
*)       echo OK: da qui in poi proseguiamo con $# parametri ;;
esac

#Controllo $1
case $1 in
	*[!0-9]*) echo $1 non numerico o non positivo
		  exit 2;;
	*) if test $1 -eq 0 
	   then echo ERRORE: primo parametro $1 non strettamente positivo
		exit 3
	   fi ;;
esac
H=$1

case $2 in
        *[!0-9]*) echo $2 non numerico o non positivo
                  exit 4;;
        *) if test $2 -le 2 -o $2 -ge 255
           then echo ERRORE: secondo parametro $2 non strettamente maggiore di 1 o minore di 255
                exit 5
           fi ;;
esac
K=$2

#quindi ora possiamo usare il comando shift due volte per eliminare il primo parametro e il secondo parametro quindi i due numeri
shift
shift

#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sulle directory in un for
for i
do
        case $i in
        /*) if test ! -d $i -o ! -x $i
            then
            echo $i non directory
            exit 6
            fi;;
        *)  echo $i non nome assoluto; exit 7;;
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