EXECS=ring

all: ${EXECS}

ring: ring.cpp
	mpic++ ring.cpp -o ring
	mpirun -n 4 ./ring 20

server: centralserver.cpp
	mpic++ centralserver.cpp -o server
	mpirun -n 4 ./server

clean:
	rm -f ${EXECS}
