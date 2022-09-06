#!/bin/sh
#File comandi ricorsivo

cd $1
#definiamo una variabile trovato: questa variabile verra' posta a true tutte le volte che viene trovato un file con le specifiche richieste
#questo consentira' di fare la stampa del nome della directory, se trovato e' uguale a true, SOLO fuori del ciclo for; la stampa fatta dentro al for e' assolutamente SCORRETTO!
trovato=false

#controlliamo anche la radice della gerarchia
case $1 in
*/$2) 
#solo nella directory di nome relativo corretto cerchiamo i file con le specifiche richieste
	for i in *
	do
		if test -f $i 	#N.B.: il testo non richiedere di cercare i file leggibili; nel caso si consideri di aggiungere la verifica di leggibilita' dato che poi si deve usare il comando grep, si deve giustificare questa aggiunta!
		then
			#ridirigiamo standard output e standard error del grep su /dev/null
	  		if grep [0-9] $i > /dev/null 2>&1	#verifichiamo che ci sia almeno una occorrenza di un carattere numerico!
	  		then 
				#salviamo il nome assoluto nel file temporaneo
	  			echo `pwd`/$i >> $3
				#settiamo trovato a true
				trovato=true
	  		fi
		fi
	done ;;
esac

if test $trovato = true
#se trovato uguale a true vuol dire che abbiamo trovato la directory DR che contiene almeno un file con un carattere numerico
then
  #quindi stampiamone il nome assoluto
  echo TROVATO UNA DIRECTORY che soddisfa le specifiche `pwd`
fi

#ricorsione in tutta la gerarchia
for i in *
do
if test -d $i -a -x $i
then
	#invocazione ricorsiva passando come primo parametro il nome assoluto della dir traversabile individuata
  	$0 `pwd`/$i $2 $3 
fi
done