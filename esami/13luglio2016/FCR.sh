#!/bin/sh
#FCR.sh 
#file comandi ricorsivo che chiamera' la parte C

cd $1
#la variabile NC ci serve per il numero di caratteri 
NC=
#variabile in cui accumuliamo i file trovati e la loro lunghezza in caratteri
params= 

#la variabile trovato ci serve per sapere se abbiamo trovato almeno un file
trovato=false

for i in *
do
	#controlliamo solo i nomi dei file (il testo NON richiede che siano leggibili e quindi NON lo controlliamo!)
	if test -f $i 
	then 	
		NC=`wc -c < $i`
		if test $NC -gt 0 -a $NC -le $2
		#i file devono avere un numero di caratteri maggiore di zero (aggiunto questo controllo per sicurezza!) e minore o uguale al secondo parametro
		then
			#abbiamo trovato un file che soddisfa le specifiche e quindi inseriamo il suo nome e la sua lunghezza in caratteri nella variabile
        		params="$params $i $NC"
			#e aggiorniamo la variabile trovato
			trovato=true
		fi
	fi
done

if test $trovato = true
then
echo TROVATO direttorio  `pwd`
echo CHIAMIAMO LA PARTE C
echo 13Lug16 $params
13Lug16 $params
fi

for i in *

do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva
		$0 `pwd`/$i $2
	fi
done