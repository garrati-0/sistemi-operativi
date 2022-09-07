#!/bin/sh

N=$1 #NOME STABILITO DAL TESTO, e ci assegno il valore del primo parametro
shift #elimino il primo parametro e quindi il numero
cd $1 #mi sposto nella directory 
trovato=0 #inizializzo la variabile trovato a 0 perche' non ho ancora trovato il file 0 file non trovatp 1 file trovato
cont=0 #inizializzo il contatore a 0
for item in *
do
    cont=`expr $cont + 1` #incremento il contatore
	#controlliamo solo i nomi dei file leggibili perche' cosi' richiede il testo!
	if test -f $item -a -r $item
	then 
		#echo FILE CORRENTE `pwd`/$i
		NC=`wc -c < $item` #numero di caratteri nel file
        #controllo se il numero di caratteri e' pari al numero N
		if test $N -eq $NC
		then
                 #controllo di trovarmi in posizione pari
                 if test `expr $cont % 2` -ne 0
                 #abbiamo trovato un file, allora impostiamo un trovato uguale a uno, stampiamo il file,e lo riposrtiamo nel file temporaneo
	   	         then trovato=1 #abbiamo trovato un file
                 echo `pwd`/$item #stampo il nome assoluto del file
                 #salviamo il nome del file trovato nel file temporaneo
		         echo `pwd`/$item >> $3
		         fi
		fi	
	fi
done

if test $trovato -eq 1 
then
#invochiamo la parte c
main `cat /tmp/nomiAssoluti$$` 
fi

for i in *

do
	if test -d $i -a -x $i
	then
        > /tmp/nomiAssoluti$$ #azzeriamo il file temporaneo
		#chiamata ricorsiva
		$0 $N `pwd`/$i $3
	fi
done