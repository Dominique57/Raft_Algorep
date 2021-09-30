CC=mpicc
BIN=main
SRC=$(wildcard *.cc)

all: run
	@echo "Default behaviour of makefile is to run"

run: $(BIN)
	mpirun -hostfile hostfile --mca opal_warn_on_missing_libcuda 0 $(BIN)

clean:
	$(RM) $(BIN)