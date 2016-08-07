#!/bin/bash

function checkRequiredPrograms() {
	type ffmpeg >/dev/null 2>&1 || { echo >&2 "FFmpeg is required but is not found on your system. Abort..."; exit 1; }
	type getopts >/dev/null 2>&1 || { echo >&2 "Getopts is required but it is not found on your system. Abort..."; exit 1; }
}

function checkImageMagick {
	type convert >/dev/null 2>&1 || { echo >&2 "Convert (ImageMagick) is required but it is not found on your system. Abort..."; exit 1; }
}

animationName=''
needsRescale=0
scale=100
includePower=0
deleteBmps=0
directory='..'
framesPerSecond=24
makeGif=0
outputFileName="output"
fontColor=green
fontSize=30

checkRequiredPrograms
while getopts "hgdpa:s:w:f:n:c:" opt; do
    case "$opt" in
    h|\?)
        echo "Simple utility to convert an png image sequence to a video or a gif. Optionally writing power to the image"
        echo "Converting required a copy of all images, which could take up a lot of space. Currently only png is supported and all images have to be the same size. Also image format should follow that outputted by fractal drawing program."
        echo "Required dependencies: FFmpeg and ImageMagick"
        echo "Options: "
        echo "   -a         Animation name (same that you specified in the \"frak\" file"
        echo "   -h         Displays this help menu"
        echo "   -p         Include powers as text, defaults to false"
        echo "   -c           Color of the power text, defaults to 'green'"
        echo "   -n           Font size of the power text, defaults to 30"
        echo "   -s         Size in percent of the original animation, defaults to 100"
        echo "   -d         Force delete all png files"
        echo "   -w         Animation directory, defaults to \"..\""
        echo "   -f         Frames per seconds, defaults to 24"
        echo "   -g         Make gif instead of the default mp4 video"
        echo "   -o         Output file name (without extension)"
        exit 0
        ;;
    a)  animationName=$OPTARG
        ;;
    s)  scale=$OPTARG;
			  needsRescale=1
			  checkImageMagick
        ;;
    p)  checkImageMagick
        includePower=1
        ;;
    d)  deleteBmps=1
			  ;;
		w)  directory=$OPTARG;
		    if [ ! -d "$directory" ]; then
					echo "[ERROR] Directory doesn't exist. Abort..."
					exit 1
				fi
				;;
		f)  framesPerSecond=$OPTARG;
				;;
		g)  makeGif=1
				;;
		o)  outputFileName=$OPTARG
				;;
		c)	fontColor=$OPTARG
				;;
		n)	fontSize=$OPTARG;
				;;
    esac
done
if [ "$animationName" = "" ]; then
	echo "[ERROR] No animation name. Give it with '-a'. Abort..."
	exit 1
fi
numberOfBmpsInCurrentFolder=`ls -l *.png 2>/dev/null | wc -l`
if [ $numberOfBmpsInCurrentFolder -gt 0 ]; then
		echo "[INFO] There are BMPs in current folder"
		if [ $deleteBmps -eq 1 ]; then
			echo "[WARNING] Deleting all png files in this folder"
			rm *.png
		else
			echo "[ERROR] Refuse to delete png files in current folder. Remove BMP files or run with '-d' to remove them. Abort..."
			exit 1
		fi
fi
animationFrameNameExpression="$directory/$animationName*.png"
numberOfFramesOfAnimation=`ls -l $animationFrameNameExpression 2>/dev/null | wc -l`

if [ $numberOfFramesOfAnimation -eq 0 ]; then
	echo "[ERROR] No animation with that name. Abort..."
	exit 1;
fi

echo "[INFO] Number of frames in the animation: $numberOfFramesOfAnimation"
echo "[INFO] Copying files to current directory"
cp $animationFrameNameExpression .
echo "[INFO] Done"

if [ $includePower -eq 1 ]; then
	echo "[INFO] Including power"
	for i in `ls *.png`; do
		power=`echo "$i" | sed -e 's/.*_\(.*\)_.*/\1/'`
		convert $i -fill $fontColor -gravity NorthWest -pointsize $fontSize -annotate +0+100 "$power" $i
	done
	echo "[INFO] Done"
fi

if [ $needsRescale -eq 1 ]; then
	echo "[INFO] Scaling images"
	for i in `ls *.png`; do
		convert $i -resize "$scale%" $i
	done
	echo "[INFO] Done"
fi

echo "[INFO] Renaming frames"
j=0
for i in `ls -la *.png | awk '{print $9}' | sort`; do
	mv "$i" "frame_$j.png" 2>/dev/null
	let j=$j+1
done
echo "[INFO] Done"

echo "[INFO] Encoding image sequence"
timestamp=`date +%s`
resultName=${outputFileName}_${timestamp}
if [ $makeGif -eq 0 ]; then
	resultName="$resultName.mp4"
	ffmpeg -hide_banner -y -i frame_%d.png -r $framesPerSecond -vb 20M -vcodec mpeg4 -r $framesPerSecond "$resultName" 
else
	resultName="$resultName.gif"
	echo "[INFO] Generating palette"
	ffmpeg -y -i frame_%d.png -vf fps=10,scale=320:-1:flags=lanczos,palettegen palette.png -loglevel 0
	echo "[INFO] Creating gif"
	ffmpeg -y -i frame_%d.png -i palette.png -r $framesPerSecond -filter_complex "fps=$framesPerSecond,scale=1920:-1:flags=lanczos[x];[x][1:v]paletteuse"  "$resultName"
	rm "palette.png"
fi
echo "[INFO] Done"
echo "[INFO] Removing copied tmp files"
rm frame*.png
echo "[INFO] Output is $resultName"