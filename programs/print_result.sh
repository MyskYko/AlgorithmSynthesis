cat log1 | grep -e "time:[0-9\.]*" | sed -e "s/time://g"
cat log2 | grep -e "time:[0-9\.]*" | sed -e "s/time://g"
cat log3 | grep -e "time:[0-9\.]*" | sed -e "s/time://g"
cat log4 | grep -e "time:[0-9\.]*" | sed -e "s/time://g"
cat log5 | grep -e "time:[0-9\.]*" | sed -e "s/time://g"


# ./print_result.sh | awk '{m += $1} END{print m/NR}'
