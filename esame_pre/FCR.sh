#!/bin/sh

L= 		#variabile in cui salviamo il numero di linee del file corrente
trovato=	#variabile che ci serve per capire se ci sono i caratteri nel file
cd $1 	#ci posizioniamo nella directory giusta
#ora dobbiamo di nuovo usare shift per eliminare il nome della gerarchia e avere solo i caratteri; NON serve salvare il primo parametro!

Lun=$2	#salviamo il secondo parametro
DirTemp=$3	#salviamo il dir temporanea
#ora possiamo usare il comando shift
shift
shift
shift

for File in *
do
	if test -f $File #solo file 
	then
        L=`wc -l < $File` #calcoliamo il numero di linee del file
        if test $L -eq $Lun #controllo che il file sia lungo Lun
        then
            #ora dobbiamo fare un for per tutti i caratteri perche' ci deve essere una occorrenza di tutti i caratteri perche' un file sia quello giusto
		    trovato=true	#settiamo la variabile a true 
		    for i	#per tutti i caratteri
		    do
			    if ! grep $i $File > /dev/null 2>&1 #se il carattere corrente non e' presente, mettiamo trovato a false
			    then
				trovato=false
			    fi
		    done
		    #se trovato e' rimasto a true vuole dire che il file e' giusto 
		    if test $trovato = true #dobbiamo salvarlo nel directory temporaneo
		    then
			#dobbiamo scrivere il nome della directory nel file temporaneo
			pwd >> $DirTemp
		    fi
        fi
		
	fi
done

for i in *
do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva 
		$0 `pwd`/$i $Lun $DirTemp $* 
	fi
done