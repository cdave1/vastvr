# Convert any wav to 16 bit wav

#!/bin/sh

IFS=$'\n'
for N in $(find . -name "*.wav" -maxdepth 1); 
do
    mkdir -p "./converted/"
    ffmpeg -i "$N" -vn -ar 44100 -ac 2 -ab 192 -f wav "./converted/$N"
done