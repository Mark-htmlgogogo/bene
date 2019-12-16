data_name=$1

edge_visiable=$2

echo DP
./run.sh $data_name $edge_visiable

echo GREEDY
./grun.sh $data_name
