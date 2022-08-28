#!/bin/sh
#Soluzione della Prova del 19 Giugno 2019

case $# in
0|1|2)   echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 H dirass1 dirass2 ...
         exit 1;;
*)       echo OK: da qui in poi proseguiamo con $# parametri ;;
esac

#Controllo $1
case $1 in
	*[!0-9]*) echo $1 non numerico o non positivo
		  exit 2;;
	*) if test $1 -le 1 -o $1 -ge 255 
	   then echo ERRORE: primo parametro $1 non strettamente maggiore di 1 o minore di 255
		exit 3
	   fi ;;
esac
H=$1

#quindi ora possiamo usare il comando shift per eliminare il primo parametro e quindi il numero
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
#controlli sui parametri finiti possiamo passare alle W fasi, dopo aver settato il path
PATH=`pwd`:$PATH
export PATH
for i
do
        echo fase per $i
	FCR.sh $i $H 
done