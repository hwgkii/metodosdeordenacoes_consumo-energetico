#!/bin/bash

for i in {1..10}
do
    echo "================================"
    echo "Execução $i"
    echo "================================"

    sudo perf stat -a \
        -e power/energy-pkg/,duration_time,user_time,system_time \
        ./selecaoV1 < entradaCinquentaMil.in \
        2> "resultado_$i.txt"

done
