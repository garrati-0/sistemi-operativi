#!/bin/sh
# Soluzione della prova d'esame del 11 Luglio 2018 
#
# File comandi. Uso: $0 CZ dirass1 dirass2 ...
#

# controllo sul numero dei parametri
case $# in
0|1|2)  echo "Errore nel numero dei parametri $#"
      	exit 1;;
*)  	echo "Numero parametri corretti $#";;
esac

#controllo sul primo parametro
case $1 in
?);;
*)
    echo "Parametro $1 non e' un singolo carattere"
    exit 2;;
esac

#salviamo il primo parametro
CZ=$1
#ora possiamo usare il comando shift
shift

#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sulle directory in un for
for i
do
        case $i in
        /*) if test ! -d $i -o ! -x $i
            then
            echo $i non directory
            exit 3
            fi;;
        *)  echo $i non nome assoluto; exit 4;;
        esac
done

#controlli sui parametri finiti possiamo passare alle N fasi

# imposto la path
PATH=`pwd`:$PATH
export PATH

# azzeriamo il file temporaneo
> /tmp/nomiAssoluti$$ 

for i
do
        echo fase per $i
        #invochiamo il file comandi ricorsivo con la gerarchia, il carattere e il file temporaneo
        FCR.sh $i $CZ /tmp/nomiAssoluti$$
done

NF=`wc -l < /tmp/nomiAssoluti$$`
echo Il numero globale di file trovati che contengono almeno un carattare $CZ e\' $NF
# Invoco la parte C, passando CZ e il contenuto del file temporaneo (con stampa di debugging)
echo stiamo per invocare la parte C con $CZ e `cat /tmp/nomiAssoluti$$` 
11Lug18 $CZ `cat /tmp/nomiAssoluti$$` 
#cancelliamo da ultimo il file temporaneo
rm /tmp/nomiAssoluti$$