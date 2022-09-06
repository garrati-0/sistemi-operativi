#!/bin/sh
#file comando principare le risolve la parte shell del 10 Giugno 2015
#10Giu15.sh G D H

case $# in
3) ;;
*) 	echo Errore: Usage is $0 dirass D H
	exit 1;;
esac

#Controllo primo parametro
case $1 in
/*) if test ! -d $1 -o ! -x $1
    then
    echo $1 non direttorio
    exit 2
    fi;;
*)  echo $1 non nome assoluto
    exit 3;;
esac

#Controllo secondo parametro
case $2 in
*/*) echo $2 NON relativo
     exit 4;;
esac

#Controllo terzo parametro
expr $3 + 0  > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -a $N1 -ne 3 
then echo numerico $3 #siamo sicuri che numerico
     if test $3 -le 0 
     then echo $3 non positivo
       	  exit 5
     fi
else
  echo $3 non numerico
  exit 6
fi

PATH=`pwd`:$PATH
export PATH

#creiamo un file temporaneo che poi passeromo al file comandi ricorsivo
> /tmp/tmp$$

FCR.sh $* /tmp/tmp$$

#anche se non richiesto stampiamo il numero e i nomi dei file trovati
echo Abbiamo trovato `wc -l < /tmp/tmp$$` file che soddisfano la specifica
echo I file sono: `cat /tmp/tmp$$` file che soddisfano la specifica
echo Adesso chiamiamo la parte C passando come parametri i file trovati e il numero H

10Giu15 `cat /tmp/tmp$$` $3

#da ultimo cancelliamo il file temporaneo
rm /tmp/tmp$$
