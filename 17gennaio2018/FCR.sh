#!/bin/sh
#File comandi ricorsivo che risolve il compito del 17 Gennaio 2018

cd $1
#definiamo una variabile nl per contenere il numero di linee
nl=
#definiamo una variabile count per contare quanti file troviamo che soddisfano le specifiche
count=0
#definiamo una variabile files per raccogliere i nomi dei file trovati (in una lista)
files=

for i in *
do
	if test -f $i #il testo non richiede la verifica di leggibilita'
	then
  		nl=`wc -l < $i` 
		#verifichiamo che il numero di linee sia uguale a quelle richieste
  		if test $nl -eq $3 
  		then 
			#verifichiamo che contenga il carattere passato come parametro
			if grep $4 $i > /dev/null 2>&1 #ridirigiamo standard output e standard error del grep su /dev/null
			then
				files="$files $i"		#salviamo il nome (relativo semplice) del file trovato
				count=`expr $count + 1`		#incrementiamo il conteggio
  			fi
  		fi
	fi
done 

#verifichiamo che il numero di file trovati soddisfi la specifica
if test $count -eq $2
then
  echo TROVATA DIRECTORY che soddisfa le specifiche `pwd`
  echo Invochiamo la parte C in questo modo 17Gen18 $files $4 $3	
  #attenzione all'ordine dei parametri richiesto dalla parte C	
  17Gen18 $files $4 $3	
fi

#ricorsione in tutta la gerarchia
for i in *
do
	if test -d $i -a -x $i
	then
		#invocazione ricorsiva passando come primo parametro il nome assoluto della dir traversabile individuata
  		$0 `pwd`/$i $2 $3 $4
	fi
done