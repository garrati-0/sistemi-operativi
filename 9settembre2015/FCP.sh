#!/bin/sh
#file comandi principale da invocare con dirass fileass

case $# in
2) ;;
*) 	echo Errore: Usage is $0 dirass fileass
	exit 1;;
esac

#Controllo primo parametro
case $1 in
/*) if test ! -d $1 -o ! -x $1
    then
    echo $1 non direttorio o non eseguibile
    exit 2
    fi;;
*)  echo $1 non nome assoluto
    exit 3;;
esac

#Controllo secondo parametro dato che deve essere il nome assoluto di un file leggibile
case $2 in
/*) if test ! -f $2 -o ! -r $2
    then
    echo $2 non file o non leggibile 
    exit 2
    fi;;
*)  echo $2 non nome assoluto
    exit 4;;
esac

PATH=`pwd`:$PATH
export PATH

#calcoliamo per prima cosa la lunghezza in byte di AF
D=`wc -c < $2`
FCR.sh $* $D
