#!/bin/sh
#FCR.sh 
#file comandi ricorsivo 
L= 		#variabile in cui salviamo il numero di linee del file corrente
trovato=	#variabile che ci serve per capire se ci sono tutti i caratteri nel file 

cd $1 	#ci posizioniamo nella directory giusta
#ora dobbiamo di nuovo usare shift per eliminare il nome della gerarchia e avere solo i caratteri; NON serve salvare il primo parametro!
shift

for F in *
do
	if test -f $F #solo file 
	then
		#ora dobbiamo fare un for per tutti i caratteri perche' ci deve essere una occorrenza di tutti i caratteri perche' un file sia quello giusto!
		trovato=true	#settiamo la variabile a true 
		for i	#per tutti i caratteri
		do
			if ! grep $i $F > /dev/null 2>&1 #se il carattere corrente non e' presente, mettiamo trovato a false
			then
				trovato=false
			fi
		done
		#se trovato e' rimasto a true vuole dire che il file e' giusto e quindi stampiamo il nome assoluto e chiamiamo la parte C come richiesto (dopo aver calcolato il numero di linee): N.B. deciso di stampare anche la sua lunghezza in linee per maggior chiarezza
		if test $trovato = true
		then
			L=`wc -l < $F`
			echo Trovato un file che soddisfa le specifiche e il suo nome assoluto e\' `pwd`/$F e la sua lunghezza in linee e\' $L
			echo chiamiamo la parte in C con $F $L e $*
			9Set20 $F $L $*
		fi
	fi
done

for i in *
do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva cui passiamo come primo parametro il nome assoluto del direttorio 
		$0 `pwd`/$i $* 
	fi
done