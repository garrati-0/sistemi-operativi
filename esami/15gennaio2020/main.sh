#!/bin/sh
# Soluzione della prova d'esame del 15 Gennaio 2020 
#
# File comandi. Uso: $0 H dirass1 dirass2 ...
#

# controllo sul numero dei parametri
case $# in
0|1|2)  echo "Errore nel numero dei parametri $#"
      	exit 1;;
*)  	echo "Numero parametri corretti $#";;
esac

#Controllo $1
case $1 in
	*[!0-9]*) echo $1 non numerico o non positivo
		  exit 2;;
	*) if test $1 -eq 0
	   then echo ERRORE: primo parametro $1 uguale a zero
		exit 3
	   fi ;;
esac
#salviamo il primo parametro
H=$1

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


for i
do
	# azzeriamo il file temporaneo: usiamo un solo file temporaneo, che perÃ² deve essere azzerato per ogni gerarchia!
	> /tmp/nomiAssoluti$$ 
        echo fase per $i
        #invochiamo il file comandi ricorsivo con la gerarchia, il numero e il file temporaneo
        FCR.sh $i $H /tmp/nomiAssoluti$$
	NF=`wc -l < /tmp/nomiAssoluti$$`
	echo Il numero globale di file trovati nelle gerarchia $i che rispettano le specifiche e\' $NF
	# Invoco la parte C, passando il contenuto del file temporaneo (con stampa di debugging), ma solo se il numero di file trovati e' pari!
	if test `expr $NF % 2` -eq 0
	then
		echo stiamo per invocare la parte C con `cat /tmp/nomiAssoluti$$` 
		15Gen20 `cat /tmp/nomiAssoluti$$` 
	else	echo $NF non pari
	fi
done

#cancelliamo da ultimo il file temporaneo
rm /tmp/nomiAssoluti$$