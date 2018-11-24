for i in {1..5}; do
    echo $i
    ../bin/gen spec.blif data.txt out$i setting.txt > log$i
done

command=`dirname $0`'/print_result.sh'
$command | awk '{m += $1} END{print m/NR}'
