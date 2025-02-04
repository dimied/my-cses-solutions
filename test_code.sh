#!/bin/bash

pn=`basename $1`
echo "App: "$pn
test_code_folder="$pn"_tests
bp="./current_tests/"
fp=$bp$test_code_folder

if [ ! -d $bp ]; then
 mkdir $bp
fi

if [ ! -d $fp ]; then
 mkdir $fp
fi

#echo "Tests: "$2
tp="$2"

unzip -o $tp -d $fp

numfails=0

for f in `ls -1 $fp/*.in | sort`; do
    
    output=${f//in/out}
    res=${f//in/res}

    echo "> "$f" > "$output
    
    start=`date +%s.%N`

    $1 < $f > $res

    end=`date +%s.%N`

    runtime=$( echo "$end - $start" | bc -l )
    echo "Time: "$runtime

    cmpres=`diff $output $res`
    
    if [[ "$cmpres" -eq "0" ]]; then
    echo "OK"
    else
    echo "FAIL"
    echo "***"
    diff $output $res
    echo "***"
    numfails=$((numfails+1))
    fi
done

echo "Fails: "$numfails

