#!/usr/bin/bash

rm -rf traces/
wget2 --progress=bar -i ./tracelist.txt -P traces/
