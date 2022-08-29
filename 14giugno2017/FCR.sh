#!/bin/sh
#FCR.sh 
#file comandi ricorsivo 
NB= 		#variabile in cui salviamo il numero di byte del file corrente
trovato=	#variabile che ci serve per capire se ci sono tutti i caratteri nel file 
contafile=0
cd $1 	#ci posizioniamo nella directory giusta
#ora dobbiamo di nuovo usare shift per eliminare il nome della gerarchia e avere solo i caratteri; NON serve salvare il primo parametro!
shift

for F in *
do
	if test -f $i -a -r $i -a -w $i #si deve cercare un file leggibile e SCRIVIBILE!
	then
		#ora dobbiamo fare un for per tutti i caratteri perche' ci deve essere una occorrenza di tutti i caratteri perche' un file sia quello giusto!
	
        NB=`wc -c < $F`
        if test $NB -eq $1
            then
            trovato=true	#settiamo la variabile a true   
		    if ! grep $2 $F > /dev/null 2>&1 #se il carattere corrente non e' presente, mettiamo trovato a false
		    then
			trovato=false
		    fi
		#se trovato e' rimasto a true vuole dire che il file e' giusto e quindi stampiamo il nome assoluto e chiamiamo la parte C come richiesto (dopo aver calcolato il numero di linee): N.B. deciso di stampare anche la sua lunghezza in linee per maggior chiarezza
		    if test $trovato = true
		    then
                echo `pwd`/$F >> $3 #inseriamo il nome assoluto nel file temporaneo
                contafile=`expr $contafile + 1`	#incrementiamo il numero di file trovati
            fi
		fi
	fi
done

#se ho trovato almeno un file 
if test $contafile -ge 1
then
  echo TROVATO DIRETTORIO `pwd` #riportiamo su standard output il nome assoluto
fi

for i in *
do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva cui passiamo come primo parametro il nome assoluto del direttorio 
		$0 `pwd`/$i $* 
	fi
done