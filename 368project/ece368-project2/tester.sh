#!/bin/bash
div ()  # Arguments: dividend and divisor
{
        if [ $2 -eq 0 ]; then echo division by 0; exit; fi
        local p=12                            # precision
        local c=${c:-0}                       # precision counter
        local d=.                             # decimal separator
        local r=$(($1/$2)); echo -n $r        # result of division
        local m=$(($r*$2))
        [ $c -eq 0 ] && [ $m -ne $1 ] && echo -n $d
        [ $1 -eq $m ] || [ $c -eq $p ] && return
        local e=$(($1-$m))
        let c=c+1
        div $(($e*10)) $2
} 
clear
echo "Compiling huff.c unhuff.c"

gcc -Werror -Wall -o3 huff.c -o huff
gcc -Werror -Wall -o3 unhuff.c -o unhuff

echo "Compilation Complete"

echo
for i in {0..5}
do
	./huff input/text$i.txt
	COMPRESSED_FILENAME=input/text$i.txt.huff
	COMPRESSED_FILESIZE=$(stat -c %s "$COMPRESSED_FILENAME")
	echo "Size of text$i.txt.huff=$COMPRESSED_FILESIZE bytes."
done

echo
echo "The compression ratio: (original file size / compressed file size)"
for i in {0..5}
do
	FILENAME=input/text$i.txt
	COMPRESSED_FILENAME=input/text$i.txt.huff

	FILESIZE=$(stat -c %s "$FILENAME")
	COMPRESSED_FILESIZE=$(stat -c %s "$COMPRESSED_FILENAME")
	echo "Compression Ratio for text$i: $(div $FILESIZE $COMPRESSED_FILESIZE)."
done

echo
for i in {0..5}
do
	./unhuff input/text$i.txt.huff
	COMPRESSED_FILENAME=input/text$i.txt.huff.unhuff
	COMPRESSED_FILESIZE=$(stat -c %s "$COMPRESSED_FILENAME")
	echo "Size of text$i.txt.huff.unhuff=$COMPRESSED_FILESIZE bytes."
done

echo
echo "After Decoding compression: (original file size / compressed file size)"
echo "should be 1, as decoded file must be same as orignal file."
for i in {0..5}
do
	FILENAME=input/text$i.txt.huff
	COMPRESSED_FILENAME=input/text$i.txt.huff.unhuff

	FILESIZE=$(stat -c %s "$FILENAME")
	COMPRESSED_FILESIZE=$(stat -c %s "$COMPRESSED_FILENAME")
	echo "Compression Ratio for text$i: $(div $FILESIZE $COMPRESSED_FILESIZE)."
done
