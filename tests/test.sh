#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"

if [ -z "$1" ]; then
    echo "Usage: ./test.sh [test directory name]"
    exit
fi

if [ ! -f tester ]; then
    c++ -Wall -Wextra -Werror -std=c++98 main.cpp -o tester
fi

echo "${1}"
tot=0
pass=0
for d in ${1}/*/; do
    cnt=$(find $d -name "*.cpp" | wc -l)
    if [[ $cnt -eq 0 ]]; then
        continue
    fi
    ((tot++))
    for f in $d*.cpp; do
        n=$(basename -s .cpp $f)
        c++ -Wall -Wextra -Werror -std=c++98 $f -o ${d}std_${n}
        c++ -Wall -Wextra -Werror -std=c++98 -DFT $f -o ${d}ft_${n}
        if [ ! -f ${d}ft_${n} ]; then
            echo -e " │"
            echo -e " ├── ${n} ${RED}Compilation Error!${ENDCOLOR}"
        else
            ./tester ${d}std_${n} ${d}ft_${n}
            ret=$?
            if [[ $ret -lt 3 ]]; then
                check=$(diff ${d}/logs/std_${n}.log ${d}/logs/ft_${n}.log | wc -l)
                if [[ $check -eq 0 ]]; then
                    ((pass++))
                    echo -e " │"
                    echo -e " ├── ${n} ${GREEN}OK${ENDCOLOR}"
                else
                    echo -e " │"
                    echo -e " ├── ${n} ${RED}KO${ENDCOLOR}"
                fi
            fi
        fi
        mkdir -p ${d}/logs
        rm -rf ${d}/std_${n}
        rm -rf ${d}/ft_${n}
        rm -rf tester
    done
done

if [[ ${pass} -eq ${tot} ]]; then
    echo -e " │"
    echo -e " └── Summary ${GREEN}${pass}${ENDCOLOR}/${tot}"
else
    echo -e " │"
    echo -e " └── Summary ${RED}${pass}${ENDCOLOR}/${tot}"
fi
