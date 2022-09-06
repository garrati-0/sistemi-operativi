#!/bin/sh

#controllo numero parametri: si devono passare N+1 con N >= 2
case $# in
0|1|2) 	echo "Errore numero parametri: devono essere almeno 3 (gerarchia e almeno 2 nomi di file)"
       	exit 1;;
*)    	echo da qui in poi proseguiamo con $# parametri;;
esac

#controllo primo argomento
case $1 in
        /*) if test ! -d $1 -o ! -x $1
            then echo Non directory o non eseguibile; exit 2
            fi;;
        *) echo Nome directory non assoluto; exit 3;;
esac
G=$1

#elimino il primo parametro per avere solo i nomi di file e quindi calcolarne la meta' oltre che procedere con i controlli che i nomi di file siano relativi semplici
shift
N=$#
echo Il numero di nomi di file passati uguale a $N
M=`expr $N / 2`
echo Si devono trovare almeno $M nomi di file 
#controllo su nomi relativi degli altri N parametri 
for i in $*
do
	case $i in
        */*) echo Nome file $i non relativo semplice; exit 4;; 
        *);; 
	esac
done

PATH=`pwd`:$PATH
export PATH

#chiamiamo la parte ricorsiva passando anche il numero minimo di nomi di file
FCR.sh $G $M $*
#NOTA BENE: il calcolo di M poteva anche essere fatto nel file comandi ricorsivo: farlo pero' nel file principale e' meglio perche' viene fatto una sola volta, mentre farlo nel file ricorsivo viene fatto tutte le volte che si va in ricorsione, ma il valore calcolato sara' comunque sempre lo stesso!