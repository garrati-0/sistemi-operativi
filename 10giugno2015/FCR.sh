#!/bin/sh
#File comandi ricorsivo

cd $1
#deliniamo una variabile trovato
trovato=false

#controlliamo anche la radice della gerarchia
case $1 in
*/$2) 
	for i in *
	do
		if test -f $i 
		then
	  		nl=`wc -l < $i` 
	  		if test $nl -eq $3 
	  		then 
	  		echo `pwd`/$i >> $4
			trovato=true
	  		fi
		fi
	done ;;
esac

if test $trovato = true
then
  echo TROVATO DIRETTORIO che soddisfa le specifiche `pwd`
fi

for i in *
do
if test -d $i -a -x $i
then
  $0 `pwd`/$i $2 $3 $4
fi
done