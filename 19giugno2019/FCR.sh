#!/bin/sh
#FCR.sh 
#file comandi ricorsivo della prova del 19 Giugno 2019

cd $1
#la variabile NF ci serve per contare il numero di file 
NF=0
#la variabile files ci serve per memorizzare i file che soddisfano la specifica 
files=
#la variabile NR ci serve per il numero di linee
NR=

for i in *
do
	#controlliamo solo i nomi dei file perche' cosi' richiede il testo!
	if test -f $i 
	then 
		NR=`wc -l < $i`
		if test $NR -lt $2
		then
			#aggiorniamo la variabile del conteggio	
			NF=`expr $NF + 1`
			#salviamo il nome del file nella variabile predisposta
			files="$files $i"		
		fi	
	fi
done

if test $NF -ge $2 
then
	echo TROVATO direttorio  `pwd`
	#dobbiamo invocare la parte C: inseriamo una stampa di DEBUGGING
	echo 19Giu19 $files
	19Giu19 $files
fi

for i in *

do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva
		$0 `pwd`/$i $2
	fi
done