paralel:
	mpicc src/paralel_dijkstra.c -o paralel_dijkstra
	scp dijkstra 13517031@167.205.35.151:~
	scp dijkstra 13517031@167.205.35.152:~
	scp dijkstra 13517031@167.205.35.153:~
	scp dijkstra 13517031@167.205.35.154:~
	scp dijkstra 13517031@167.205.35.155:~

serial:
	gcc src/serial_dijkstra.c -o serial_dijkstra
	./serial_dijkstra

ifeq (mpirun, $(firstword $(NAMECMDGOALS)))
	runarg := $(wordlist 2, $(words $(MAKECMDGOALS)), $(MAKECMDGOALS))
	$(eval $(runarg):;@true)
endif
mpirun: src/paralel_dijkstra