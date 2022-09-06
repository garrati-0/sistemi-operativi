#!/bin/sh
#FCR.sh 
#file comandi ricorsivo della prova del 10 Luglio 2019

cd $1
#la variabile NF ci serve per contare il numero di file
NF=0
#la variabile NR ci serve per il numero di linee in cui compare il carattere da cercare
NR=

for i in *
do
	#controlliamo solo i nomi dei file leggibili perche' cosi' richiede il testo!
	if test -f $i -a -r $i
	then 
		#echo FILE CORRENTE `pwd`/$i
		NR=`grep $2 $i | wc -l`
		#controlliamo che il numero di linee che contengono il carattere Cz sia almeno 2!
		#in alternativa al piping di comandi sato sopra, si poteva usare lÃ¬opzione -c del comando grep
		if test $NR -ge 2
		then
			#aggiorniamo la variabile del conteggio
                        NF=`expr $NF + 1`
			#salviamo il nome assoluto del file nel file temporaneo
			echo `pwd`/$i >> $3
		fi	
	fi
done

if test $NF -ge 1 
then
	#se abbiamo trovato almeno un file che soddisfa le specifiche, andiamo a stampare il nome assoluto della dir
	echo TROVATO direttorio  `pwd`
fi

for i in *

do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva
		$0 `pwd`/$i $2 $3
	fi
done