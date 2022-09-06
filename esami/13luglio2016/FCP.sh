#!/bin/sh
#Soluzione della Prova del 13 Luglio 2016

case $# in
0|1)	echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 dirass numerostrettpos 
	exit 1;;
2) 	echo OK: da qui in poi proseguiamo con $# parametri ;;
*)	echo Errore: numero parametri is $# quindi troppi parametri. Usage is $0 dirass numerostrettpos
	exit 1;;
esac

#controlliamo la gerarchia $1
case $1 in
	/*) if test ! -d $1 -o ! -x $1
	    then
	    echo $1 non direttorio
	    exit 2
	    fi;;
	*)  echo $1 non nome assoluto; exit 3;;
esac

#Controllo $2 
case $2 in
	*[!0-9]*) echo $2 non numerico o non positivo
			exit 4;;
	*) if test $2 -eq 0 -o $2 -gt 255
	   then echo ERRORE: secondo parametro $2 uguale a zero oppure maggiore di 255
		exit 5
	   fi ;;
esac

#controlli sui parametri finiti possiamo passare a settare il path
PATH=`pwd`:$PATH
export PATH
#invocazione file comandi ricorsivo
FCR.sh $1 $2   
#ATTENZIONE IL TESTO NON DICE CHE SI DEVONO COMPIERE DELLA AZIONI DOPO AL TERMINE DELL'INTERA ESPLORAZIONE RICORSIVA (COME INVECE RICHIEDEVA, AD ESEMPIO IL TESTO DELL'8 GIUGNO 2016 E QUINDI IN QUESTO PRIMO FILE COMANDI NON SI DEVE FARE ALTRO!
