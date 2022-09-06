#!/bin/sh
#file comandi principale che risolve la parte shell del 18 Gennaio 2017
#18Gen17.sh G D 

#Controllo sul numero dei parametri: controllo stretto, ci devono essere esattamente 2 parametri!
case $# in
2) ;;
*) 	echo Errore: Usage is $0 dirass D
	exit 1;;
esac

#Controllo primo parametro: nome assoluto e poi controllo dir e attraversabile!
case $1 in
/*) if test ! -d $1 -o ! -x $1
    then
    	echo $1 non directory o non attraversabile
    	exit 2
    fi;;
*)  echo $1 non nome assoluto
    exit 3;;
esac

#Controllo secondo parametro: nome relativo semplice; ATTENZIONE NON SI DEVE CONTROLLARE CHE SIA UNA DIRECTORY!
case $2 in
*/*) echo $2 NON relativo semplice
     exit 4;;
esac

#modifichiamo ed esportiamo la variabile PATH
PATH=`pwd`:$PATH
export PATH

#creiamo un file temporaneo che poi passeremo al file comandi ricorsivo
> /tmp/tmp$$

FCR.sh $* /tmp/tmp$$

#anche se non richiesto stampiamo il numero e i nomi dei file trovati
echo Abbiamo trovato `wc -l < /tmp/tmp$$` file che soddisfano la specifica
echo I file sono: `cat /tmp/tmp$$` 
echo Adesso chiamiamo la parte C passando come parametri i file trovati 
18Gen17 `cat /tmp/tmp$$` 

#da ultimo cancelliamo il file temporaneo
rm /tmp/tmp$$