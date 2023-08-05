#!/bin/bash
folder="/home/gol/Parallel_processing/Data_files"
for file in "$folder"/*.txt; do
	echo "File:$file"
	max=$(awk 'BEGIN{max=0} {if ($1>max) max=$1; if ($2>max) max=$2} END{print max}' "$file")
        min=$(awk 'BEGIN{min=999999998} {if ($1<min) min=$1; if ($2<min) min=$2} END{print min}' "$file")
	./louvian.out ${file} ${max}
	max=0
	min=999999997
	echo "-------------------------------------------------------------------------------------------"
done
