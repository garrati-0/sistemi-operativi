#!/bin/sh
#file comandi ricorsivo dirass fileass e D (dimensione di fileass (AF)

cd $1

#per prima cosa azzeriamo il conteggio dei file che soddisfano la specifica
conta=0

#definiamo una variabile per memorizzare i nomi dei file (che possono essere relativi dato che la parte C viene chiamata in questo file)
file=

for i in *
do
	if test -f $i -a -r $i
	then
	  	#se è un file leggibile controlliamo la sua lunghezza in byte
		dim=`wc -c < $i`
		if test $dim -eq $3
		then
			file="$file $i" 
			conta=`expr $conta + 1`
		fi
	fi
	done

#se ho trovato almeno due file
if test $conta -ge 2 
then
  echo TROVATO DIRETTORIO `pwd`
  echo CHIAMO PARTE C con i file trovati $file e AF
  9Set15 $file $2  
fi

for i in *
do
if test -d $i -a -x $i
then
  #echo RICORSIONE in `pwd`/$i 
  $0 `pwd`/$i $2 $3 
fi
done