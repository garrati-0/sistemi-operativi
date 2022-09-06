#!/bin/sh
#FCR.sh 
#file comandi ricorsivo della prova del 17 Febbraio 2021

cd $1
#la variabile cont ci serve per contare il numero di file leggibili
cont=0
#la variabile files ci serve per memorizzare i file leggibili
files=

for F in *
do
	#controlliamo solo i nomi dei file leggibili perche' cosi' richiede il testo!
	if test -f $F -a -r $F
	then 
		#calcoliamo la lunghezza in linee del file per vedere se soddisfa le specifiche
		L=`wc -l < $F`
		if test $L -eq $3
		then
			#aggiorniamo la variabile del conteggio	
			cont=`expr $cont + 1`
			#salviamo i nome del file nella variabile apposita
			files="$files $F"		
		fi
	fi
done

if test $cont -ge $2 
then
	echo TROVATO direttorio  `pwd`
	#dobbiamo invocare la parte C passando i nomi dei file precedentemente salvati
	echo invochiamo 17Feb21 $files
	17Feb21 $files
fi

for i in *
do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva
		$0 `pwd`/$i $2 $3
	fi
done