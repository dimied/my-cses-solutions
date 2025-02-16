#!/bin/bash

pn=`basename $1`
echo "App: "$pn
test_code_folder="$pn"_tests
bp="./current_tests/"
rtf="./current_runtimes/"
fp=$bp$test_code_folder

if [ ! -d $bp ]; then
 mkdir $bp
fi

if [ ! -d $rtf ]; then
 mkdir $rtf
fi

rtf=$rtf$pn

if [ ! -d $rtf ]; then
 mkdir $rtf
fi

if [ ! -d $fp ]; then
 mkdir $fp
fi

#echo "Tests: "$2
tp="$2"

unzip -o $tp -d $fp
#unzip -f $tp -d $fp

numfails=0
numall=0
ct=`date +%m%d_%H%M%S`
dts=`date +%d_%H%M%S`
rtf=$rtf"/"$ct".txt"
echo "Times: "$rtf

for f in `ls -1 $fp/*.in | sort`; do
    
    output=${f//in/out}
    res=${f//in/res}
    err=${f//in/err}
    tlog=${f//in/time.log}
    err=$err"."$dts

    rt=`basename $f`

    echo "> "$f" > "$output
    
    start=`date +%s.%N`

    $1 < $f > $res 2>$err

    end=`date +%s.%N`
    
    if [[ "$3" -eq "p" ]]; then
    gprof $1 ./gmon.out > $tlog
    fi

    runtime=$( echo "$end - $start" | bc -l )
    echo "Time: "$runtime
    echo $rt": "$runtime >> $rtf

    cmpres=`diff $output $res`

    numall=$((numall+1))

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

echo "Fails: "$numfails" of "$numall

