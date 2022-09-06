#!/bin/sh
#file comandi ricorsivo dirass M

cd $1

#per prima cosa azzeriamo il conteggio dei file che soddisfano la specifica e il conteggio dei direttori
contafile=0
contadir=0

#definiamo una variabile per memorizzare i nomi dei file (che possono essere relativi dato che la parte C viene chiamata in questo file)
file=

for i in *
do
	if test -f $i -a -r $i
	then
	  	#se è un file leggibile 
		file="$file $i" 
		contafile=`expr $contafile + 1`
	else
		if test -d $i
		then 
		contadir=`expr $contadir + 1`
	 	fi
	fi	
done

#se ho trovato esattamente M=2N file e nessun sotto-direttorio
if test $contafile -eq $2 -a $contadir -eq 0
then
  echo TROVATO DIRETTORIO `pwd`
  echo CHIAMO PARTE C con i file trovati $file 
  22Gen16 $file   
else #stampa non richiesta 
  echo TROVATO DIRETTORIO `pwd`
  echo ma non giusto dato che file $contafile e dir $contadir
fi

for i in *
do
if test -d $i -a -x $i
then
  #echo RICORSIONE in `pwd`/$i 
  $0 `pwd`/$i $2 
fi
done