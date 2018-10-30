#!/bin/bash

#construção
#cd ~/codigoexemplo/
#make

#execução

cd ~/codigoexemplo/datagrump/
#sudo sysctl -w net.ipv4.ip_forward=1
for j in {1..30}; do
	./run-contest nelson
done

