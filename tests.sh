# run make command
make clean
make

# run mc_single, mc_mpi, mc_thread with different values

# mc_single
echo "mc_single"
for i in 100 1000 10000 100000 1000000 10000000 100000000 1000000000
do
    # echo "n = $i"
    ./mc_single $i
done

# mc_mpi
echo "mc_mpi"
for process in 2 4 8 16 32 #64
do
    for i in 100 1000 10000 100000 1000000 10000000 100000000 1000000000
    do
        # echo "n = $i"
        mpiexec -n $process ./mc_mpi $i
    done
done

# mc_thread
echo "mc_thread"
for thread in 2 4 8 16 32 #64 128 256 512
do
    for i in 100 1000 10000 100000 1000000 10000000 100000000 1000000000
    do
        # echo "n = $i"
        ./mc_thread $thread $i
    done
done
