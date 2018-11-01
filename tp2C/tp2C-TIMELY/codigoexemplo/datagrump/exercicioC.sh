#!/bin/bash
TEAM_NAME=Nelson

#variaveis
controller=/home/tp2/codigoexemplo/datagrump/controller.cc



for i in $(seq 1 1 9); do
  echo $i
  if [ "$i" -eq "1" ]; 
  then
  echo $i
  sed -i "s|BETA(0.0)|BETA(0.$((i)))|g" $controller

    make
    ./run-contest $TEAM_NAME-$i
     sleep 10
  else
  aux=$(($i-1))
     
    sed -i "s|BETA(0.$((i-1)))|BETA(0.$((i)))|g" $controller

    make
    ./run-contest $TEAM_NAME-$i
    sleep 10
   fi
done

sed -i "s|BETA(0.9)|BETA(1)|g" $controller

for i in $(seq 1 1 9); do
  echo $i
  if [ "$i" -eq "1" ];
  then
  echo "1"
    sed -i "s|ALPHA(1)|ALPHA(0.$((i)))|g" $controller

    make
    ./run-contest $TEAM_NAME-$i
     sleep 10
  else
  aux=$(($i-1))

    sed -i "s|ALPHA(0.$((i-1)))|ALPHA(0.$((i)))|g" $controller

    make
    ./run-contest $TEAM_NAME-$i
    sleep 10
   fi
done

sed -i "s|ALPHA(0.9)|ALPHA(1)|g" $controller

for i in $(seq 1 1 10); do
  echo $i
  if [ "$i" -eq "1" ];
  then
  echo "1"
    sed -i "s|DELTA(1)|DELTA(0.$((i)))|g" $controller

    make
    ./run-contest $TEAM_NAME-$i
     sleep 10
  else
  aux=$(($i-1))

    sed -i "s|DELTA(0.$((i-1)))|DELTA(0.$((i)))|g" $controller

    make
    ./run-contest $TEAM_NAME-$i
    sleep 10
   fi
done
