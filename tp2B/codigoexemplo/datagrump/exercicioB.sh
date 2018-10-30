#!/bin/bash
TEAM_NAME=Nelson

#variaveis
controller=/home/tp2/codigoexemplo/datagrump/controller.cc



for i in $(seq 0 1 20); do
  echo $i
  if [ "$i" -eq "1" ]; 
  then
  echo "1"
    sed -i "s|: debug_( debug ), the_window_size(1), ai(0.1), md(20),boundary_window(12),time_out_ms(80)|: debug_( debug ), the_window_size(1), ai(0.$(($i))), md($((20-$i))),boundary_window(12),time_out_ms(80)|g" $controller


  echo $((20-$i))     

    make
    ./run-contest $TEAM_NAME-$i
     sleep 1
  else
  aux=$(($i-1))
 
    sed -i "s/: debug_( debug ), the_window_size(1), ai(0.$(($aux))), md($((20 - $aux))),boundary_window(12),time_out_ms(80)/: debug_( debug ), the_window_size(1), ai(0.$(($i))), md($((20 - $i))),boundary_window(12),time_out_ms(80)/g" $controller
    echo $((20-$i))
    echo $(($i-1))
    echo ": debug_( debug ), the_window_size(1), ai(0.$(($aux-1))), md($((20 - $aux))),boundary_window(12),time_out_ms(80)/: debug_( debug ), the_window_size(1), ai(0.$(($i))), md($((20 - $i))),boundary_window(12),time_out_ms(80)"  
    make
    ./run-contest $TEAM_NAME-$i
    sleep 1
 
    
   fi
done

