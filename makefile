EXECS=ring

all: ${EXECS}

ring: ring.cpp
	mpic++ ring.cpp -o ring

run: ring
	mpirun -n 4 ./ring 20

clean:
	rm -f ${EXECS}
