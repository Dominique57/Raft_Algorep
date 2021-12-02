BUILD_TYPE = RELEASE

slots = 8
nodes = 4
clients = 3

all: force-build
	@echo "localhost slots=${slots}" > res/hostfile
	@echo "========================== BEGIN ======================================"
	mpirun -hostfile res/hostfile --mca opal_warn_on_missing_libcuda 0 build/algoreppppp ${nodes} ${clients}
	@echo "=========================== END ======================================="

force-build:
	cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -B build -S .
	cd build && make -j --no-print-directory

build:
	cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -B build -S .
	cd build && make -j --no-print-directory

# Default test file
TEST_FILE = tests/multiple_entries.cmd
test: build
	@echo "======================= TESTS FILES ==================================="
	@echo "Open files hostfile and ${TEST_FILE}"
	@echo "========================== BEGIN ======================================"
	mpirun -hostfile hostfile --mca opal_warn_on_missing_libcuda 0 build/algoreppppp ${TEST_FILE}
	@echo "=========================== END ======================================="
