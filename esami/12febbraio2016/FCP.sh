#!/bin/sh
#file comandi principale da invocare con dirass Char

case $# in
2) ;;
*) 	echo Errore: Usage is $0 dirass char
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

#Controllo secondo parametro dato che deve essere un singolo carattere alfaberito minuscolo
case $2 in
[a-z]) echo $2 alfabetico minuscolo;;
*) echo ERRORE: secondo parametro $2 non alfabetico minuscolo
   exit 4;;
esac

#controlli sui parametri finiti possiamo passare a settare il path
PATH=`pwd`:$PATH
export PATH

#creazione file temporaneo
> /tmp/tmp$$

#invocazione della parte ricorsiva
FCR.sh $1 $2 /tmp/tmp$$

#calcoliamo il numero di file trovati
Nfile=`wc -l <  /tmp/tmp$$`

#se sono almeno due invochiamo la parte C
if test $Nfile -ge 2
then
echo Stiamo per chiamare la parte C sui file `cat /tmp/tmp$$`
12Feb16 `cat /tmp/tmp$$` $2
fi

#rimuoviamo il file temporaneo
rm /tmp/tmp$$