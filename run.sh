#!/bin/bash

for i in 1 2 3
do
	sh exploit.sh $i | ./authenticate
done