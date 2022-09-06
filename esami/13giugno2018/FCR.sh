#!/bin/sh
#FCR.sh 
#file comandi ricorsivo che scrive il nome dei file trovato sul file temporaneo passato come ultimo parametro
NL= 		#variabile in cui salviamo il numero di linee del file corrente
trovato=false 	#variabile che serve per sapere se abbiamo trovato almeno un file che soddisfa le specifiche

cd $1 	#ci posizioniamo nella directory giusta

case $1 in
*/$2)	#se il nome corrisponde a quello che stiamo cercando
	for i in *
	do
		if test -f $i #solo file
		then
			NL=`wc -l < $i`
			if test $NL -eq $3 		#se numero di linee giusto
			then	
				trovato=true		#abbiamo trovato almeno un file
				echo `pwd`/$i >> $4	#salviamo il nome assoluto nel file temporaneo
			fi
		fi
	done
	if test trovato = true
	then 
		echo Trovato directory che soddisfa le specifiche: `pwd`
	fi
	;;
*);;
esac

for i in *
do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva cui passiamo come primo parametro il nome assoluto del direttorio 
		$0 `pwd`/$i $2 $3 $4
	fi
done