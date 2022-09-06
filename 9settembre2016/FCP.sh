#!/bin/sh
#Soluzione della prova del 9 Settembre 2016
#versione che usa un file temporaneo e usa FCR.sh per tutte le fasi

case $# in
0|1)	echo Errore: numero parametri is $# quindi pochi parametri. Usage is $0 dirass1 dirass2 ...
	exit 1;;
*) 	echo OK: da qui in poi proseguiamo con $# parametri ;;
esac
#in $* abbiamo i nomi delle gerarchie  
for i 
do
	#soliti controlli su nome assoluto e direttorio traversabile
	case $i in
	/*) 	if test ! -d $i -o ! -x $i
		then
	    	echo $i non direttorio
	    	exit 2
	    	fi;;
	*)  echo $i non nome assoluto; exit 3;;
	esac
done
#controlli sui parametri finiti possiamo passare alle N fasi
PATH=`pwd`:$PATH
export PATH
> /tmp/conta$$ #creiamo/azzeriamo il file temporaneo. NOTA BENE: SOLO 1 file temporaneo!

for i 
do
	echo fase per $i 
	#invochiamo il file comandi ricorsivo con la gerarchia e il file temporaneo
	FCR.sh $i /tmp/conta$$   
done

#terminate tutte le ricerche ricorsive cioe' le N fasi
#N.B. Andiamo a contare le parole (word) del file /tmp/conta$$
F=`wc -w < /tmp/conta$$` 
echo Il numero di file totali che soddisfano la specifica = $F 
for i in `cat /tmp/conta$$`
do
	#Stampiamo (come richiesto dal testo) i nomi assoluti dei file trovati
	echo "Trovato il file $i"
	#invochiamo la parte C passando il nome (assoluto) di ogni file trovato
	9Set16 $i
done 
#da ultimo eliminiamo il file temporaneo
rm /tmp/conta$$
