#!/bin/sh
#Soluzione della Prova del 10 Luglio 2019

case $# in
0|1|2)   echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 H dirass1 dirass2 ...
         exit 1;;
*)       echo OK: da qui in poi proseguiamo con $# parametri ;;
esac

#Controllo $1
case $1 in
	?) echo $1 singolo carattere ;;
	*) echo $1 non singolo carattere
	   exit 2;;
esac
Cz=$1

#quindi ora possiamo usare il comando shift per eliminare il primo parametro e quindi il carattere  
shift

#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sulle directory in un for
for i
do
        case $i in
        /*) if test ! -d $i -o ! -x $i
            then
            echo $i non directory
            exit 4
            fi;;
        *)  echo $i non nome assoluto; exit 5;;
        esac
done
#controlli sui parametri finiti possiamo passare alle W fasi, dopo aver settato il path
PATH=`pwd`:$PATH
export PATH

#azzeriamo/creiamo il file temporaneo
> /tmp/files$$

for i
do
        echo fase per $i
	FCR.sh $i $Cz /tmp/files$$ 
done

#N.B. Andiamo a contare le linee del file /tmp/temp$$ anche se non richiesto e stampiamo tale numero
NF=`wc -l < /tmp/files$$`
echo Il numero totale di file trovati e\' $NF
#stampa di DEBUG
echo Chiamo la parte in C con `cat /tmp/files$$`
10Lug19 `cat /tmp/files$$` $C

#rimuviamo il file temporaneo
rm /tmp/files$$