#!/bin/sh
# Soluzione della prova d'esame del 11 Luglio 2018
#
# File ricorsivo. Uso: $0 dirass CZ filetemp
#
cd $1

for i in *
do
   # analizzo i file del direttorio corrente
   if test -f $i
   then
      	grep $2 $i >/dev/null 2>&1 # cerchiamo il carattere $2 (CZ) nel file
        if test $? -eq 0 # se lo troviamo
        then
        	echo "Trovato file `pwd`/$i" #stampiamo contestualmente il nome assoluto, come richiesto dal testo
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