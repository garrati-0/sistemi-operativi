#!/bin/sh
#controllo numeri di parametri passati dall' utente
case $# in
0|1|2|3) 	echo Errore: numero parametri is $# quindi pochi parametri. 
	exit 1;;
*) 	echo OK: da qui in poi proseguiamo con $# parametri ;;
esac

#Controllo che il primo parametro sia una directory
case $1 in
/*) 	if test ! -d $1 -o ! -x $1
	then
		echo $1 non directory
		exit 2
	fi;;
*) 	echo $1 non nome assoluto
	exit 3;;
esac

Dir=$1 #salviamo il primo parametro 

#controllo che il secondo parametro sia un numero intero strettamente positivo
case $2 in
	*[!0-9]*) echo $1 non numerico o non positivo
		  exit 2;;
	*) if test $1 -eq 0 
	   then echo ERRORE: primo parametro $1 non strettamente positivo
		exit 3
	   fi ;;
esac

Lun=$2 #salviamo il secondo parametro

#quindi ora possiamo usare il comando shift
shift
shift

#ora in $* abbiamo solo i caratteri e quindi possiamo se sono effetivamente tutti caratteri
for i 
do
	case $i in
	?)  echo OK $i e\' un carattere  ;;
	*)  echo $i non e\' un carattere 
	    exit 4;;
	esac
done

#controlli sui parametri finiti 
PATH=`pwd`:$PATH
export PATH

#azzeriamo il file temporaneo
> /tmp/temporaneo
#invochiamo il file comandi ricorsivo con la gerarchia e i caratteri
FCR.sh $Dir $Lun /tmp/temporaneo $* 

files= 	#varibile di appoggio che ci servira' per raccogliere i nomi dei file
for i in `cat /tmp/temporaneo`
do
	for j in *
	do
		if test -f $j #se e' un file lo salviamo in una variabile di appoggio
		then
			files="$files $j"
		fi
	done
	echo CHIAMIAMO LA PARTE C con $files
	main $files $Lun $*
done	

#cancelliamo il file temporaneo
rm /tmp/temporaneo