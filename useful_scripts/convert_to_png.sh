scale=100%
if [ "$#" -ne 0 ]; then
	scale=$1
fi
echo $scale

for i in `ls ../*.bmp`; do convert "$i" -resize "$scale" "$i.png"; rm $i; echo "Converted $i to $i.png"; done
