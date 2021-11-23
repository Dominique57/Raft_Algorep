[ $# -ne 1 ] && echo "Missing argument (1 required)" && exit 1

resPath="$1"
[ ! -e hostfile ] && ln -s "$resPath/hostfile" hostfile
echo "========================== BEGIN ======================================"
mpirun -hostfile hostfile --mca opal_warn_on_missing_libcuda 0 algoreppppp 4 3
echo "=========================== END ======================================="
