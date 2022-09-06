#!/bin/sh
#Soluzione dell'esame del 16 Gennaio 2019

case $# in
0|1) echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 dirass F
     exit 1;;
2)   echo OK: da qui in poi proseguiamo con $# parametri ;;
*)   echo Errore: numero parametri is $# quindi troppi parametri. Usage is $0 dirass F
     exit 2;;
esac

#Controllo primo parametro sia dato in forma assoluta
case $1 in
/*) if test ! -d $1 -o ! -x $1
	    then
	    echo $1 non directory
	    exit 3
	    fi;;
*)  echo $i non nome assoluto; exit 4;;
esac

#conviene che controlliamo che F sia un nome relativo semplice (visto l'uso che se ne deve fare come F.1 e F.2)
#controllo secondo parametro relativo semplice
case $2 in
*/*) echo Errore: $2 non in forma relativa semplice
     exit 5;;
*) ;;
esac

#controlli sui parametri finiti 
PATH=`pwd`:$PATH
export PATH

#invochiamo il file comandi ricorsivo con la gerarchia e il secondo parametro, quindi con tutti i parametri passati
FCR.sh $*