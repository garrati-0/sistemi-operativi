#!/bin/sh

#controlliamo il numero di paremetri che deve essere 1 + z con z>1 
case $# in
0|1|2)   echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 H dirass1 dirass2 ...
         exit 1;;
*)       echo OK: da qui in poi proseguiamo con $# parametri ;;
esac

#Controllo $1, il quale deve essere un numero strettamente positivo e pari
case $2 in
	*[!0-9]*) echo $2 non numerico o non positivo
		  exit 5;;
	*) if test $2 -eq 0
	   then echo ERRORE: secondo parametro $2 uguale a zero
		exit 6
	   else 
	   	if test `expr $2 % 2` -ne 0
	   	then echo ERRORE: secondo parametro $2 NON pari
		     exit 4
		fi
	   fi ;;
esac
N=$1  #NOME STABILITO DAL TESTO, e ci assegno il valore del primo parametro

#quindi ora possiamo usare il comando shift per eliminare il primo parametro e quindi il numero
shift

#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sulle directory in un for
for G
do
        case $G in
        /*) if test ! -d $G -o ! -x $G
            then
            echo $G non directory
            exit 5
            fi;;
        *)  echo $G non nome assoluto; exit 6;;
        esac
done
#controlli sui parametri finiti possiamo passare alle Z fasi, dopo aver settato il path
PATH=`pwd`:$PATH
export PATH

# azzeriamo il file temporaneo: usiamo un solo file temporaneo
> /tmp/nomiAssoluti$$ 
for G
do
        echo fase per $G	
	#invochiamo il file comandi ricorsivo  il primo parametro (cioe' il numero pari strettamente maggiore di zero) e con la gerarchia
	FCR.sh $N $G /tmp/nomiAssoluti$$
done

rm /tmp/nomiAssoluti$$