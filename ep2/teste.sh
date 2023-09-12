#! /usr/bin/env bash

if [[ ! -x $1  ]]; then
    echo "$1 não é executavél"
fi
acc=()
prog=$1
declare -i c
c=0
function execute_test {
    if [[ -n $acc ]]; then
        #echo $acc | $prog > teste_$c.log
        echo $acc | $prog
        echo "-----------------------------------------"
        c=c+1
    fi
}

# ISSO é bizarro, mas eu agrupei o while pq como o pipe cria um SubShell
# Então a variavel acc n vai ser atualizada depois do while, então vai estar vaiza
# dessa forma se chamar execute_test nao vai acontecer nada.
# Da para criar uma funcao e passar o pipe para ela tambem.
# function parse {
#   <codigo que usa acc>
# }
#
# grep -v "^#" teste.dat | parse
#
# doido!!!!!
#
# Vi como resolver aqui:
# https://serverfault.com/questions/259339/bash-variable-loses-value-at-end-of-while-read-loop
grep -v "^#" teste.dat | (while read line; do
    if [[ $line == "--break--" ]]; then
        execute_test
        exit 0
    elif [[ -n $line ]]; then
        acc+="$line "
    else
        execute_test
        acc=()
    fi
done
execute_test)
