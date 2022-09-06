#!/bin/sh
# Soluzione della prova d'esame del 12 Febbraio 2020 
#
# File comandi. Uso: $0 K1 K2 dirass1 dirass2 ...
#

# controllo sul numero dei parametri
case $# in
0|1|2|3)  echo "Errore nel numero dei parametri $#"
      	  exit 1;;
*)  	  echo "Numero parametri corretti $#";;
esac

#Controllo $1
case $1 in
	*[!0-9]*) echo $1 non numerico o non positivo
		  exit 2;;
	*) if test $1 -eq 0
	   then echo ERRORE: primo parametro $1 uguale a zero
		exit 3
	   fi 
	   if test `expr $1 % 2` -ne 1
	   then echo ERRORE: primo parametro $1 non dispari 
		exit 4
	   fi 
		   ;;
esac

#Controllo $2
case $2 in
	*[!0-9]*) echo $2 non numerico o non positivo
		  exit 5;;
	*) if test $2 -eq 0
	   then echo ERRORE: secondo parametro $2 uguale a zero
		exit 6
	   fi 
	   if test `expr $2 % 2` -ne 0
	   then echo ERRORE: secondo parametro $2 non pari
		exit 7
	   fi 
		   ;;
esac
#salviamo il primo e secondo parametro
K1=$1
K2=$2

#ora possiamo usare il comando shift due volte
shift
shift

#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sulle directory in un for
for i
do
        case $i in
        /*) if test ! -d $i -o ! -x $i
            then
            echo $i non directory
            exit 8
            fi;;
        *)  echo $i non nome assoluto; exit 9;;
        esac
done

#controlli sui parametri finiti possiamo passare alle N fasi

# imposto la path
PATH=`pwd`:$PATH
export PATH

# azzeriamo il file temporaneo: usiamo un solo file temporaneo
> /tmp/nomiAssoluti$$ 

for i
do
        echo fase per $i
        #invochiamo il file comandi ricorsivo con la gerarchia, i due numeri e il file temporaneo
        FCR.sh $i $K1 $K2 /tmp/nomiAssoluti$$
done

# Invoco la parte C, passando il contenuto del file temporaneo (con stampa di debugging)
echo stiamo per invocare la parte C con `cat /tmp/nomiAssoluti$$` 
12Feb20 `cat /tmp/nomiAssoluti$$` 

#cancelliamo da ultimo il file temporaneo
rm /tmp/nomiAssoluti$$