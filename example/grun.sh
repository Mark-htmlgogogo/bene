greedy_dir="/mnt/d/Repo/bene/src/greedySearch"
resdir="/mnt/d/Repo/bene/example/resdir/"
file_dir="/mnt/d/Repo/bene/example"
name=$1

vdfile=$file_dir/$name.vd

nof_vars=`cat $vdfile | wc -l`
echo "$name: $nof_vars"
$greedy_dir/greedy $nof_vars $resdir

#/mnt/d/Repo/bene/src/greedySearch/greedy 3 /mnt/d/Repo/bene/example/resdir/