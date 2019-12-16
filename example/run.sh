#!/bin/bash

#echo $1
#echo $2

d=`dirname $0`

# if [ $# -eq 1 ]; then
# 	ess=$1
# else
#     ess=1
# fi

data_name=$1

edge_visible=$2

ess=1



#DO NOT set selected file 5
#score=`$d/../bin/data2net.sh $d/$data_name.vd $d/$data_name.idt $ess $d/resdir`

# SET selected file 3
#score=`$d/../bin/data2net.sh -s $d/$data_name.sel $d/$data_name.vd $d/$data_name.idt $ess $d/resdir`

#echo Score : $score

$d/../bin/data2net.sh $d/$data_name.vd $d/$data_name.idt $ess $d/resdir

if [ $edge_visible -eq 1 ]; then
	echo Arcs : 
	$d/../bin/net2parents $d/resdir/net - | $d/../bin/parents2arcs - -

	if which dot > /dev/null; then
		$d/../bin/net2parents $d/resdir/net - \
		| $d/../bin/parents2arcs - - \
		| $d/../bin/arcs2dot $d/$data_name.vd - - \
		| dot -Tpng -o $d/resdir/$data_name.png
    echo See $d/resdir/$data_name.png for a png picture of the net.
	fi
fi
