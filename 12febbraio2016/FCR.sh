#!/bin/sh
#file comandi ricorsivo dirass Char filetemp

cd $1

#per prima cosa azzeriamo il conteggio dei file che soddisfano la specifica 
contafile=0

for i in *
do
	if test -f $i -a -r $i
	then
	  	#se è un file leggibile controlliamo se contiene il carattere che dobbiamo cercare
		if grep $2 $i > /dev/null 2> /dev/null
		then
			echo `pwd`/$i >> $3 
			contafile=`expr $contafile + 1`
		fi
	fi	
done

#se ho trovato almeno un file 
if test $contafile -ge 1
then
  echo TROVATO DIRETTORIO `pwd`
fi

for i in *
do
if test -d $i -a -x $i
then
  #echo RICORSIONE in `pwd`/$i 
  $0 `pwd`/$i $2 $3
fi
done