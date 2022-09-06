#!/bin/sh
#FCR.sh 
#file comandi ricorsivo della prova del 12 Settembre 2018

cd $1
#la variabile NF ci serve per contare il numero di file leggibili
NF=0
#la variabile files ci serve per memorizzare i file leggibili
files=

for i in *
do
	#controlliamo solo i nomi dei file leggibili perche' cosi' richiede il testo!
	if test -f $i -a -r $i
	then 
		#aggiorniamo la variabile del conteggio	
		NF=`expr $NF + 1`
		#salviamo i nomi del file nella variabile apposita
		files="$files $i"		
	fi
done

#dobbiamo verificare se in questa dir abbiamo trovato il numero giusto di file leggibili
if test $NF -ge $2 -a $NF -le $3
then
	echo TROVATO direttorio `pwd`
	#dobbiamo invocare la parte C passando i nomi dei file precedentemente salvati
	12Set18 $files
fi

for i in *
do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva
		$0 `pwd`/$i $2 $3
	fi
done