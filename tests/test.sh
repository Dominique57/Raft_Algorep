#!/bin/sh

# Check arguments
[ $# -lt 1 ] && echo "Missing test file as first argument" && exit 1
[ ! -f "$1" ] && echo "First argument is not a file" && exit 1


# Create build
SCRIPT_PATH=$(readlink -f "$0" | xargs dirname)
cd "$SCRIPT_PATH/.."
if [ -n "$NO_FORCE_REBUILD" ]; then
  make build
else
  make force-build
fi

# Remove old log files
rm -f ./*.log

# Create hostfile
nb_node=$(head -n 1 "$1")
nb_client=$(head -n 2 "$1" | tail -n 1)
nb_total=$((nb_node + nb_client + 1))
nb_entry=$(grep -c '^ENTRY' "$1")
echo "localhost slots=$nb_total" > hostfile

# Start timer (ms precision)
START=$(date +%s%3N)
MAX_END=$((START + 30000))
make test TEST_FILE="$1" 2> /dev/null &
child_pid=$!

clean_exit() {
  kill $child_pid
  exit "$1"
}

success=1
while [ "$success" -ne "0" ] ; do
  END=$(date +%s%3N)

  if [ "$END" -ge "$MAX_END" ]; then
    echo "FAILED: timer has exceeded 30 seconds"
    clean_exit 1
  fi

  success=0
  for i in $(seq 1 "$nb_node"); do
    if [ ! -f "node_$i.log" ]; then
      success=1
      break
    fi

    log_length=$(wc -l < "node_$i.log")
    if [ "$log_length" -lt "$nb_entry" ]; then
      success=1
      break
    fi
  done

  if [ "$success" -ne "0" ]; then
    continue
  fi

  success=0
  for i in $(seq 2 "$nb_node"); do
    diff "node_1.log" "node_$i.log" 1> /dev/null 2> /dev/null
    if [ "$?" -ne "0" ] ; then
      echo "!! node_1.log differs with node_$i.log"
      success=1
    fi
  done

  if [ "$success" -ne "0" ]; then
    echo "Some log differed !"
    clean_exit 1
  fi
done

echo "================================== SUCCESS =================================="
echo "================= THIS IS THE BEST RAFT IMPLEMENTATION EVER ================="
echo "Consensus took $((END - START)) ms"
echo "Consensus has $nb_entry entries"
echo "Consensus had $nb_node nodes"
echo "Consensus has $nb_client clients"

clean_exit 0
