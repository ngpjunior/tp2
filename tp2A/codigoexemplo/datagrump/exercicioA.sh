#!/bin/bash
TEAM_NAME=Nelson

#variaveis
mahi=/home/tp2/mahimahi/scripts/mm-throughput-graph
mahi2=/home/tp2/mahimahi/scripts/mm-delay-graph
controller=/home/tp2/codigoexemplo/datagrump/controller.cc
raiz=/home/tp2/codigoexemplo/
executa=/home/tp2/codigoexemplo/datagrump/
dados=/tmp/contest_uplink_log

meike() {
    cd $raiz
    make
    cd $executa
}

for i in $(seq 0 5 100); do
    echo $i
    if [ "$i" -eq "0" ];
    then
        continue
    elif [ "$i" -eq "5" ];
    then
	echo "MAHI $mahi"
        sed -i "s/saida/saida-$i/g" $mahi 
        cat $mahi | grep saida
        sed -i "s/throughput.svg/throughput-$i\.svg/g" $mahi
        cat $mahi | grep "\.svg"
        sed -i "s/delay.svg/delay-$i\.svg/g" $mahi2
        cat $mahi2 | grep "\.svg"
        sed -i "s/the_window_size = 20/the_window_size = $i/g" $controller
        meike
        ./run-contest $TEAM_NAME-$i
        sleep 1
        $mahi 500 $dados
        $mahi2 $dados
    else
        echo "else - i = $i"
        aux=$(($i-5))
        sed -i "s/saida-$aux/saida-$i/g" $mahi
        cat $mahi | grep saida
        sed -i "s/throughput-$aux\.svg/throughput-$i\.svg/g" $mahi
        cat $mahi | grep "\.svg"
        sed -i "s/delay-$aux.svg/delay-$i\.svg/g" $mahi2
        cat $mahi2 | grep "\.svg"
        sed -i "s/the_window_size = $aux/the_window_size = $i/g" $controller
        meike
        ./run-contest $TEAM_NAME-$i
        sleep 1
        $mahi 500 $dados
        $mahi2 $dados
    fi
done
