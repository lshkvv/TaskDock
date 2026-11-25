#!/bin/sh
# Проверка примера без TaskDock: эталон и решение на двух готовых входах.
# Согласие на ok.in и расхождение на fail.in — пример живой.
set -eu
cd "$(dirname "$0")"
CXX="${CXX:-c++}"
"$CXX" -std=c++20 -O2 reference.cpp -o /tmp/taskdock-pair-ref
"$CXX" -std=c++20 -O2 solution.cpp -o /tmp/taskdock-pair-sol
"$CXX" -std=c++20 -O2 generator.cpp -o /tmp/taskdock-pair-gen

check_file() {
    name="$1"
    expected_diff="$2"
    ref_out=$(/tmp/taskdock-pair-ref < "cases/$name")
    sol_out=$(/tmp/taskdock-pair-sol < "cases/$name")
    echo "$name: reference=$ref_out solution=$sol_out"
    if [ "$ref_out" = "$sol_out" ]; then
        same=yes
    else
        same=no
    fi
    if [ "$same" != "$expected_diff" ]; then
        echo "неожиданный результат для $name" >&2
        exit 1
    fi
}

check_file ok.in yes
check_file fail.in no

echo "--- первые 8 тестов генератора (seed 42) ---"
i=1
while [ "$i" -le 8 ]; do
    /tmp/taskdock-pair-gen --seed 42 --case "$i" --size 6 > "/tmp/taskdock-case-$i.in"
    ref_out=$(/tmp/taskdock-pair-ref < "/tmp/taskdock-case-$i.in")
    sol_out=$(/tmp/taskdock-pair-sol < "/tmp/taskdock-case-$i.in")
    if [ "$ref_out" = "$sol_out" ]; then
        mark=AGREE
    else
        mark=DIFFER
    fi
    echo "case $i: reference=$ref_out solution=$sol_out $mark"
    i=$((i + 1))
done
