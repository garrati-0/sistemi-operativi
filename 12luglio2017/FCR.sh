#!/bin/sh
#FCR.sh
#file comandi ricorsivo che scrive il nome dei file trovati insieme con la lunghezza in linee
#sul file temporaneo il cui nome e' passato come terzo argomento

cd $1
#la variabile NR ci serve per il numero di linee dei file
NR=
#per prima cosa azzeriamo il conteggio dei file che soddisfano la specifica
contafile=0

for i in *
do
        #controlliamo solo i nomi dei file (se si vuole inserire anche il controllo se leggibili per cautelarci dato che dopo usiamo il comando wc CI SI DEVE RICORDARE CHE bisogna commentarlo!)
        if test -f $i
        then
                #calcoliamo il numero di linee
                NR=`wc -l < $i`
                #controlliamo se il numero delle linee maggiore o uguale a L, ma strettamente minore di 255
                if test $NR -ge $2 -a $NR -lt 255 
                        then
                        #abbiamo trovato un file che soddisfa le specifiche 
                	echo `pwd`/$i >> $3  #salviamo il nome nel file temporaneo
                	echo $NR >> $3  #salviamo anche la dimensione in linee
                        contafile=`expr $contafile + 1` #incrementiamo il numero di file trovati
                fi
        fi
done

#se ho trovato almeno un file
if test $contafile -ge 1
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