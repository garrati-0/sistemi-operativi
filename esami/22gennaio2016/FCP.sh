#!/bin/sh
#file comandi principale da invocare con dirass N

case $# in
2) ;;
*) 	echo Errore: Usage is $0 dirass N 
	exit 1;;
esac

#Controllo primo parametro
case $1 in
/*) if test ! -d $1 -o ! -x $1
    then
    echo $1 non direttorio o non eseguibile
    exit 2
    fi;;
*)  echo $1 non nome assoluto
    exit 3;;
esac

#Controllo secondo parametro dato che deve essere un intero strettamente positivo maggiore o uguale a 2
case $2 in
	*[!0-9]*) echo $2 non numerico o non positivo
			exit 4;;
	*) if test $2 -lt 2
	   then echo ERRORE: secondo parametro $2 non maggiore o uguale a 2
		exit 5
	   fi ;;
esac

#controlli sui parametri finiti possiamo passare a settare il path
PATH=`pwd`:$PATH
export PATH
M=`expr $2 \* 2`
FCR.sh $1 $M