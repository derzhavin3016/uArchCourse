#!/usr/bin/bash

WARMUP=5000000
SIM=20000000

./bin/champsim -w $WARMUP -i $SIM $1 --json "$1.json"
