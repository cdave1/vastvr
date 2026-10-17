#!/bin/bash
# -l Mono 

find "." -iname "*.wav" |
while read F
do
	NEWFILE="${F/wav/caf}" 
	echo "wav to caf for $F"
	/usr/bin/afconvert -f caff -d LEI16@22050 $F $NEWFILE
	# mv -f $NEWFILE ../res/BaseDir/Sounds
done
