#!/bin/sh
#file comandi principale da invocare con dirass K

case $# in
2) ;;
*) 	echo Errore: Usage is $0 dirass K
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
expr $2 + 0  > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -a $N1 -ne 3 
then echo numerico $2 #siamo sicuri che numerico
     if test $2 -le 0 
     then echo $2 non positivo
       	  exit 4
     fi
else
  echo $2 non numerico
  exit 5
fi

PATH=`pwd`:$PATH
export PATH

#definiamo una variabile che conterà il livello corrente e la passiamo come terzo parametro al file comandi ricorsivo
conta=0
#il primo livello  (gerarchia G) verra' contato come livello 1: se invece avessimo voluto contarlo come livello 0 allora conta doveva essere inizalizzata a -1

FCR.sh $* $conta

