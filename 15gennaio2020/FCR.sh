#!/bin/sh
# Soluzione della prova d'esame del 15 Gennaio 2020
#
# File ricorsivo. Uso: $0 dirass H filetemp
#
L= #lunghezza in caratteri di un file
cd $1

for i in *
do
   # analizzo i file della directory corrente
   if test -f $i	#nota bene la specifica con richiede che sia un file leggibile ... chiaramente si puo' poi avere un porblema con il comando wc!
   then
      	L=`wc -c < $i` # calcoliamo la lunghezza del file in caratteri
        if test $L -eq $2 # se la lunghezza e' uguale al secondo parametro
        then
        	echo "Trovato file `pwd`/$i" 		#stampa richiesta dal testo 
		# aggiungiamo il nome assoluto del file nel file temporaneo
		echo `pwd`/$i >> $3
        fi
   fi
done

# invocazione ricorsiva nella gerarchia
for i in *
do 
    if test -d $i -a -x $i
    then
        FCR.sh `pwd`/$i $2 $3 
    fi
done