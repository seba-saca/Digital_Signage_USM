#!/bin/bash
path_video=$1
path_imagen=$2

yes | ffmpeg -i $path_video -vf "thumbnail,scale=720:480" -frames:v 1 $path_imagen

