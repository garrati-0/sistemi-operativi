#!/bin/sh
#entro nella directory
cd $1
#il secondo parametro e' il numero minimo di nomi di file da trovare
M=$2

#elimino i primi due parametri
shift 
shift
#azzero la variabile che serve per il conteggio dei file  
F=0
#variabile che serve per memorizzare i nomi dei file trovati per poi passarli alla parte C
files=

for i in $* #per tutti i nomi dei file passati
do
#per ogni nome di file lo cerchiamo nella directory corrente
        if test -f $i 
        then
		#se lo troviamo aggiorniamo il contatore e memorizzamo il nome 
		F=`expr $F + 1`
		files="$files $i"
        fi
done
                 
if test $F -ge $M 
then
#abbiamo trovato una directory giusta
#stampiamo il nome su standard output
       	echo Trovata directory giusta `pwd` 
       #inseriamo una sorta di stampa di debugging
	echo "Chiamiamo 13Feb19 $files"
       	13Feb19 $files
fi

#chiamata ricorsiva
for i in *
do
        if test -d $i -a -x $i
        then 
		#chiamata ricorsiva
		echo chiamo $0 `pwd`/$i $M $* 
		$0 `pwd`/$i $M $*
        fi
done
