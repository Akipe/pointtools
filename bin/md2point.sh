#!/bin/sh

mkfilename() {
	printf "%.4d-%s" "$1" "$2" | tr '# :.()/' '_'
	printf ".txt"
}

mkunderline() {
	i=$1
	while [ $i -gt 1 ];
	do
		printf "%s" "$2"
		i=$(($i - 1))
	done
}

fname=""
snum=0

while IFS='
' read -r line;
do
	case $line in
	\#\#*)
		titlelen="$(utf8expr length "$line")"
		title="$(utf8expr substr "$line" 3 $titlelen)"
		fname="$(mkfilename "$snum" "$title")"
		snum=$((snum + 1))
		printf "\n  %s\n  %s\n\n" \
			"$title" "$(mkunderline $titlelen "=")" > $fname
		;;
	\#pause)
		fname="$(mkfilename "$snum" "$title")"
		cat "$(mkfilename "$((snum - 1))" "$title")" >> $fname
		snum=$((snum + 1))
		;;
	*)
		[ -n "$fname" ] && \
			printf " %s\n" "$line" \
			| sed 's,	,        ,' \
			| sed 's,^\([[:blank:]]*\)\*,\1o,' >> $fname
		;;
	esac
done

