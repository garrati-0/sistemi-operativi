#!/bin/sh
#FCR.sh 
#file comandi ricorsivo che cerca i file che hanno nome F.1 e F.2 con F passato come secondo parametro

primo=0 	#variabile che se a 1 significa che e' stato trovato il file con nome F.1 nella dir corrente
secondo=0 	#variabile che se a 1 significa che e' stato trovato il file con nome F.2 nella dir corrente
dimC1=		#variabile che conta il numero di caratteri di F.1
dimL1=		#variabile che conta il numero di linee di F.1
dimC2=		#variabile che conta il numero di caratteri di F.2
dimL2=		#variabile che conta il numero di linee di F.2

cd $1 	#ci posizioniamo nella directory giusta

for i in * 
do
	if test -f $i	#solo file 
	then
		case $i in
		$2.1) 	primo=1
			dimC1=`wc -c < $i`
			dimL1=`wc -l < $i`
			;;
		$2.2) 	secondo=1
			dimC2=`wc -c < $i`
			dimL2=`wc -l < $i`
			;;
		esac
	fi
done

if test $primo -eq 1 -a $secondo -eq 1 -a $dimC1 -eq $dimC2 -a $dimL1 -eq $dimL2
then
	echo "TROVATO-il direttorio `pwd` soddisfa la condizione"
	echo Invochiamo la parte C 16Gen19 $2.1 $2.2
	16Gen19 $2.1 $2.2
fi

for i in *
do
	if test -d $i -a -x $i
	then
		#chiamata ricorsiva cui passiamo come primo parametro il nome assoluto del direttorio 
		$0 `pwd`/$i $2
	fi
done