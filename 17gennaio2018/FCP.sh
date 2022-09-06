#!/bin/sh
#file comandi principale che risolve la parte shell del 17 Gennaio 2018
#FCP.sh G N H Cx

#Controllo sul numero dei parametri: controllo stretto, ci devono essere esattamente 4 parametri!
case $# in
4) ;;
*) 	echo Errore: Usage is $0 dirass N H Cx
	exit 1;;
esac

#Controllo primo parametro
case $1 in
/*) if test ! -d $1 -o ! -x $1
    then
    	echo $1 non directory o non attraversabile
    	exit 2
    fi;;
*)  echo $1 non nome assoluto
    exit 3;;
esac

#Controllo secondo parametro
expr $2 + 0  > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -a $N1 -ne 3
then 	echo numerico N $2 #siamo sicuri che numerico
     	if test $2 -le 0
     	then 	echo $2 non positivo
          	exit 4
     	fi
else
  	echo $2 non numerico
  	exit 5
fi

#Controllo terzo parametro
#facciamo un controllo alternativo per il terzo parametro che deve essere numerico e strettamente positivo. NOTA BENE: in un compito, scegliere uno dei due modi ed usare in modo congruente solo uno dei due modi
case $3 in
*[!0-9]*) echo $3 non numerico o non positivo
          exit 6;;
*) if test $3 -eq 0
   then	echo $3 non diverso da 0
        exit 7
   else echo numerico H $3 #siamo sicuri che numerico
   fi;;
esac

#controllo quarto parametro
case $4 in
?) echo carattere $4;;
*) echo $4 non singolo carattere
   exit 8;;
esac

#settiamo la variabile PATH
PATH=`pwd`:$PATH
export PATH

#invochiamo la parte ricorsiva passando tutti i parametri
FCR.sh $* 