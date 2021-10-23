[ ! -e hostfile ] && ln -s ../hostfile hostfile
echo "========================== BEGIN ======================================"
mpirun -hostfile hostfile --mca opal_warn_on_missing_libcuda 0 algoreppppp
echo "=========================== END ======================================="
