#!/bin/sh
# Soluzione della prova d'esame del 12 Febbraio 2020
#
# File ricorsivo. Uso: $0 dirass K1 K2 filetemp
#
L= 	 #lunghezza in caratteri di un file
count1=0 #numero di file trovati con lunghezza uguale a K1
F1=	 #nome del file che ha lunghezza K1
count2=0 #numero di file trovati con lunghezza uguale a K2
F2=	 #nome del file che ha lunghezza K2

cd $1

for i in *
do
   # analizzo i file del direttorio corrente
   if test -f $i
   then
      	L=`wc -c < $i` 				# calcoliamo la lunghezza del file in caratteri
	if test $L -eq $2 			# se la lunghezza e' uguale al secondo parametro (K1)
        then
        	F1=`pwd`/$i 			#salviamo il nome assoluto
		count1=`expr $count1 + 1`	#aggiorniamo il contatore
	else
		if test $L -eq $3 		# se la lunghezza e' uguale al terzo parametro (K2)
        	then
        		F2=`pwd`/$i 			#salviamo il nome assoluto
			count2=`expr $count2 + 1`	#aggiorniamo il contatore
		fi
	fi
   fi
done

if test $count1 -eq 1 -a $count2 -eq 1  #se entrambi i contatori sono a 1 vuol dire che sono stati trovati esattamente i due file che stiamo cercando!
then
	echo Direttorio trovato `pwd`
	# aggiungiamo il nome assoluto dei due file nel file temporaneo, nell'ordine richiesto dal testo!
	echo $F2 >> $4
	echo $F1 >> $4
else
	#else non richiesto, ma introdotto solo per questioni di debugging!
	echo Dir non giusto dato che count1=$count1 e count2=$count2
fi

# invocazione ricorsiva nella gerarchia
for i in *
do 
    if test -d $i -a -x $i
    then
        FCR.sh `pwd`/$i $2 $3 $4 
    fi
done