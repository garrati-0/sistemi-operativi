#!/bin/sh
#FCR.sh 
#file comandi ricorsivo che scrive il nome assoluto dei file trovati sul file temporaneo passato come ultimo parametro
NC= 		#variabile in cui salviamo il numero di caratteri del file corrente
NL= 		#variabile in cui salviamo il numero di linee del file corrente
media= 		#variabile in cui salviamo il numero medio di lunghezza delle linee del file corrente

cd $1 	#ci posizioniamo nella directory giusta

for i in *
do
	if test -f $i -a -r $i 
	then
		NL=`wc -l < $i`
		if test $NL -eq $2  	#se numero di linee giusto - N.B. se si fa per primo questo controllo, ci evitiamo il problema dei file di dimensione nulla!
		then
			NC=`wc -c < $i`
			media=`expr $NC / $NL`
			if test $media -gt 10 		#se numero medio di caratteri per linea giusto
			then	
				#echo per il file $i abbiamo NC = $NC, NL = $NL e media = $media
				#abbiamo trovato un file con le caratteristiche richieste
				echo `pwd`/$i >> $3	#salviamo il nome assoluto nel file temporaneo
			fi
		fi
	fi
done

for i in *
do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva cui passiamo come primo parametro il nome assoluto del direttorio 
		$0 `pwd`/$i $2 $3 
	fi
done