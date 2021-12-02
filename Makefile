BUILD_TYPE = RELEASE

slots = 8
nodes = 4
clients = 3

all:
	cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -B build -S .
	cd build && make -j --no-print-directory
	@echo "localhost slots=${slots}" > res/hostfile
	@echo "========================== BEGIN ======================================"
	mpirun -hostfile res/hostfile --mca opal_warn_on_missing_libcuda 0 build/algoreppppp ${nodes} ${clients}
	@echo "=========================== END ======================================="

# Default test file
TEST_FILE = tests/multiple_entries
TEST_FILE_HOSTFILE = ${TEST_FILE}.hostfile
TEST_FILE_CMD = ${TEST_FILE}.cmd
test:
	cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -B build -S .
	cd build && make -j --no-print-directory
	@echo "======================= TESTS FILES ==================================="
	@echo "Open files ${TEST_FILE_HOSTFILE} and ${TEST_FILE_CMD}"
	@echo "========================== BEGIN ======================================"
	mpirun -hostfile ${TEST_FILE_HOSTFILE} --mca opal_warn_on_missing_libcuda 0 build/algoreppppp ${TEST_FILE_CMD}
	@echo "=========================== END ======================================="
