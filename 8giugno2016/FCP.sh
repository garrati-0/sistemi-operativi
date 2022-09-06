#!/bin/sh
#file comando principare le risolve la parte shell del 8 Giugno 2016
#8Giu16.sh G! G2 D H

#Controllo numero dei parametri
case $# in
4) ;;
*) 	echo Errore: Usage is $0 dirass1 dirass2 D H
	exit 1;;
esac

#Controllo primo parametro
case $1 in
/*) if test ! -d $1 -o ! -x $1
    then
    echo $1 non direttorio
    exit 2
    fi;;
*)  echo $1 non nome assoluto
    exit 3;;
esac

#Controllo secondo parametro
case $2 in
/*) if test ! -d $2 -o ! -x $2
    then
    echo $2 non direttorio
    exit 4
    fi;;
*)  echo $2 non nome assoluto
    exit 5;;
esac

#Controllo terzo parametro
case $3 in
*/*) echo $3 NON relativo
     exit 6;;
esac

#Controllo quarto parametro
expr $4 + 0  > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -a $N1 -ne 3 
then echo numerico $4 #siamo sicuri che numerico
     if test $4 -le 0 -o $4 -ge 255
     then echo $4 non positivo o non minore di 255
       	  exit 7
     fi
else
  echo $4 non numerico
  exit 8
fi

PATH=`pwd`:$PATH
export PATH

#creiamo due file temporanei che poi passeremo al file comandi ricorsivo
> /tmp/tmp$$-1
> /tmp/tmp$$-2

#invochiamo il file ricorsivo due volte, uno per ogni gerarchia
FCR.sh $1 $3 $4 /tmp/tmp$$-1
FCR.sh $2 $3 $4 /tmp/tmp$$-2

#calcoliamo il numero di file trovati nelle due gerarchie
#Nota bene dobbiamo usare wc -w altrimenti non riusciamo a contare i file
F1=`wc -w < /tmp/tmp$$-1` 
F2=`wc -w < /tmp/tmp$$-2` 
#anche se non richiesto stampiamo il numero e i nomi dei file trovati
echo Abbiamo trovato per la gerarchia $1, $F1 file che soddisfano la specifica
echo I file della gerarchia $1 che soddisfano la specifica sono: `cat /tmp/tmp$$-1` 
echo Abbiamo trovato per la gerarchia $2, $F2 file che soddisfano la specifica
echo I file della gerarchia $2 che soddisfano la specifica sono: `cat /tmp/tmp$$-2` 
if test $F1 -ge 2 -a $F2 -ge 2
then
	echo Adesso chiamiamo la parte C passando come parametri i nomi di tutti i file trovati e il numero H
	8Giu16 `cat /tmp/tmp$$-1` `cat /tmp/tmp$$-2` $4
fi
#da ultimo cancelliamo i file temporanei
rm /tmp/tmp$$-1
rm /tmp/tmp$$-2