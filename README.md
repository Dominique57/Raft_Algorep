# Algoreppppp

system distribuer:
Système clients/serveurs avec un mécanisme permettant
de contrôler ou d’injecter des fautes dans le système. L’idée générale est la suivante : les clients proposent des
valeurs/commandes aux serveurs.

# Usage
```sh
42sh$ make [slots=${nb_slots}] [nodes=${nb_nodes}] [clients=${nb_clients}]
```

The default number of slots is 8 with 4 nodes and 3 clients.

The default test_file is "tests/simple\_1\_entry.cmd".

To launch a test and still have an interactive session:
```sh
42sh$ make test [TEST_FILE=${test_file}]
```

To launch a test and check if entries are update to date. 
```sh
42sh$ ./tests/test.sh [test_file]
```

To launch all tests:
```sh
42sh$ ./tests/test_all.sh [test_directory]
```

# REPL: controller commands

* `HELP`: display help message
* `CRASH [rank]`: crash the given process
* `ENTRY [client_rank] [cmd]`: add given cmd to log of a given client
* `PRINT_LOG [node_rank]`: prints the log of a given node or all nodes
* `RECOVERY [rank]`: un-crashes the given process
* `SLEEP [milliseconds]`: sleep the controller for milliseconds
* `SPEED [rank] [speed]`: set speed for the given process, available speed: low, medium, high
* `START [client_rank]`: start the given client
* `STATUS [rank]`: display information for the given process or for all processes
