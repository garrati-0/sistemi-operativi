#!/bin/sh
#FCR.sh
#file comandi ricorsivo per l'esame del 14 Febbraio 2018

cd $1
#la variabile NR ci serve per il numero di linee dei file
NR=
#la variabile trovato ci serve per capire se abbiamo trovato almeno un file
TROVATO=false

for i in *
do
        #controlliamo solo i nomi dei file (se si vuole inserire anche il controllo se leggibili per cautelarci dato che dopo usiamo il comando wc CI SI DEVE RICORDARE CHE bisogna commentarlo!)
        if test -f $i
        then
                #calcoliamo il numero di linee
                NR=`wc -l < $i`
                #controlliamo se il numero delle linee e' uguale a $2 
                if test $NR -eq $2  
                        then
                        #abbiamo trovato un file che soddisfa le specifiche 
                	echo `pwd`/$i >> $3 #inseriamo il nome assoluto nel file temporaneo
			TROVATO=true #aggiorniamo la variabile TROVATO
        	fi
        fi
done

#se ho trovato almeno un file
if test $TROVATO = true 
then
  echo TROVATO DIRETTORIO `pwd` #riportiamo su standard output il nome assoluto
fi

for i in *
do
        if test -d $i -a -x $i
        then
                #chiamata ricorsiva cui passiamo come primo parametro il nome assoluto del direttorio
                FCR.sh `pwd`/$i $2 $3
        fi
done