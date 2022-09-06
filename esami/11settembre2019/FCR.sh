#!/bin/sh
#FCR.sh 
#file comandi ricorsivo della prova dell'11 Settembre 2019

cd $1
#la variabile NF ci serve per contare il numero di file 
NF=0
#la variabile NC ci serve per il numero di caratteri
NC=
#le variabili filesDispari e filesPari ci servono per memorizzare i file che soddisfano la specifica 
filesDispari=
filesPari=

for i in *
do
	#controlliamo solo i nomi dei file perche' cosi' richiede il testo!
	if test -f $i 
	then 
	 	NC=`wc -c < $i`
                if test $NC -eq $3
                then
			#aggiorniamo la variabile del conteggio	
			NF=`expr $NF + 1`
			#salviamo il nome del file nella variabile predispostaa giusta a seconda se sia in posizione dispari o pari
			if test `expr $NF % 2` -eq 1 #caso dispari
			then
				filesDispari="$filesDispari $i"
			else #caso pari
				filesPari="$filesPari $i"
			fi
		fi
	fi
done

if test $NF -eq $2 
then
	echo TROVATO direttorio  `pwd`
	#dobbiamo invocare la parte C due volte come richiesto dal testo
       	#inseriamo stampe di DEBUGGING
	echo 11Set19 $filesDispari
	echo 11Set19 $filesPari
	#11Set19 $filesDispari
	#11Set19 $filesPari
fi

for i in *
do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva
		$0 `pwd`/$i $2 $3
	fi
done