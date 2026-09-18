#!/bin/bash

# rodas as 10 vezes
for i in {1..10}
do
    echo "================================"
    echo "Execução $i"
    echo "================================"

# apenas para separar as execucoes

    sudo perf stat -a \
        -e power/energy-pkg/,duration_time,user_time,system_time \
        ./bubblesort < entradaDezMil.in \
        2> "resultado_$i.txt"
# comando para pegar o resultado do perf e transformar em um arquivo txt
done

