#! /bin/bash
# split_text_to_array_and_iterate.sh

	text="ONE_TWO_THREE"
	readarray -d "_" -t WORDS <<< "$text" # single char not str "_"
	n=${#WORDS[*]}
	for (( i = 0; i < n; i++ )) # OK with/out $n / n # OK SP =
	do
		echo "WORDS[$i] ${WORDS[i]}" # NB no $ in $i
	done


