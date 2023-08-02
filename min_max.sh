#!/bin/bash
awk 'BEGIN{max=0} {if ($1>max) max=$1; if ($2>max) max=$2} END{print "MAX:" max}' $1
awk 'BEGIN{max=99999999} {if ($1<max) max=$1; if ($2<max) max=$2} END{print "MIN:" max}' $1

