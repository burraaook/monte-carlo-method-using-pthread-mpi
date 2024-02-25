# description
- tests can be run with tests.sh
- mpi in your system may not support more than N processes, so modify tests.sh in this case
- analysis of the results and other details are in the report.pdf

# installation
```
sudo apt install mpich
```

# compilation
```
make
```

# run tests
```
chmod +x tests.sh
./tests.sh
```

# run mc_single
```
./mc_single <number_of_tosses>
```

# run mc_mpi
```
mpiexec -n <number_of_processes> ./mc_mpi <number_of_tosses>
```

# run mc_thread
```
./mc_thread <number_of_threads> <number_of_tosses>
```
