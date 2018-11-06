for i in {1..5}; do
    echo $i
    ../bin/gen spec.blif data.txt out$i setting.txt > log$i
done

./print_result.sh | awk '{m += $1} END{print m/NR}'
