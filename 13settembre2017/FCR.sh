#!/bin/sh
#FCR.sh
#file comandi ricorsivo che crea tanti file di nome xxx.testa quanti sono i file con lunghezza in linee strettamente maggiore di X (passato come secondo paramtro)

cd $1
#la variabile NR ci serve per il numero di linee dei file
NR=
#per prima cosa azzeriamo la variabile che conterra' i nomi dei file creati
files=

for i in *
do
        #controlliamo solo i nomi dei file (se si vuole inserire anche il controllo se leggibili per cautelarci dato che dopo usiamo il comando wc CI SI DEVE RICORDARE CHE bisogna commentarlo!)
        if test -f $i
        then
                #calcoliamo il numero di linee
                NR=`wc -l < $i`
                #controlliamo se il numero delle linee e' strettamente maggiore di $2 
                if test $NR -gt $2  
                then
                	#abbiamo trovato un file che soddisfa le specifiche 
                	head -$2 $i > $i.testa  #creiamo un file con le prime $2 linee
                        files="$files $i.testa" #inseriamo il nome del nuovo file creato nella variabile apposita
                fi
        fi
done

#se ho creato almeno un file
if test "$files" 
then
  echo TROVATO DIRETTORIO `pwd` #riportiamo su standard output il nome assoluto della directory corrente
  echo chiamiamo la parte C con $files
  13Set17 $files 
fi

for i in *
do
        if test -d $i -a -x $i
        then
                #chiamata ricorsiva cui passiamo come primo parametro il nome assoluto del direttorio e come secondo parametro il numero X
                FCR.sh `pwd`/$i $2
        fi
done