#!/bin/sh
#File comandi ricorsivo

cd $1
#definiamo una variabile cont che contera' i file trovati 
cont=0
#definiamo una variabile che conterra' i file trovati 
files=

#controlliamo anche la radice della gerarchia
case $1 in
*/$2) 
	for i in *
	do
		if test -f $i 
		then
	  		nl=`wc -l < $i` 
			#controlliamo la lunghezza in linee
	  		if test $nl -eq $3 
	  		then 
	  		files="$files `pwd`/$i" 
			cont=`expr $cont + 1`
	  		fi
		fi
	done ;;
esac

if test $cont -ge 2
then
  echo TROVATO DIRETTORIO che soddisfa le specifiche `pwd`
#solo a questo punto scriviamo i nomi assoluti nel file temporaneo!
  echo $files >> $4
fi

for i in *
do
if test -d $i -a -x $i
then
#ricorsione
  $0 `pwd`/$i $2 $3 $4
fi
done