
SRC = src/mc_single.c src/mc_estimation.c src/mc_mpi.c src/mc_thread.c tests_csv.c
HDR = include/mc_estimation.h
FLAGS = -Wall -Wextra -std=gnu99
LFLAGS = -lpthread
TRGT1 = mc_single
TRGT2 = mc_mpi
TRGT3 = mc_thread
VFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes

OBJS1 = obj/mc_single.o \
	   obj/mc_estimation.o

OBJS2 = obj/mc_mpi.o \
	   obj/mc_estimation.o

OBJS3 = obj/mc_thread.o \
	   obj/mc_estimation.o

all: $(TRGT1) $(TRGT2) $(TRGT3) $(TRGT4)

obj/mc_single.o: src/mc_single.c $(HDR)
	mkdir -p obj
	gcc $(FLAGS) -c -o $@ $<

obj/mc_estimation.o: src/mc_estimation.c $(HDR)
	mkdir -p obj
	gcc $(FLAGS) -c -o $@ $<

obj/mc_thread.o: src/mc_thread.c $(HDR)
	mkdir -p obj
	gcc $(FLAGS) -c -o $@ $<

obj/mc_mpi.o: src/mc_mpi.c $(HDR)
	mkdir -p obj
	mpicc $(FLAGS) -c -o $@ $<

$(TRGT1): $(OBJS1)
	gcc $(FLAGS) -o $(TRGT1) $(OBJS1)

$(TRGT2): $(OBJS2)
	mpicc $(FLAGS) -o $(TRGT2) $(OBJS2)

$(TRGT3): $(OBJS3)
	gcc $(FLAGS) -o $(TRGT3) $(OBJS3) $(LFLAGS)

clean:
	rm -rf obj $(TRGT1) $(TRGT2) $(TRGT3)

run: $(TRGT1)
	./$(TRGT1) 1000000

run2: $(TRGT2)
	mpiexec -n 4 ./$(TRGT2) 1000000

run3: $(TRGT3)
	./$(TRGT3) 10 1000000
	