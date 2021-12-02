# Algoreppppp

system distribuer:
Système clients/serveurs avec un mécanisme permettant
de contrôler ou d’injecter des fautes dans le système. L’idée générale est la suivante : les clients proposent des
valeurs/commandes aux serveurs.

# Usage
```sh
make [slots=${nb_slots}] [nodes=${nb_nodes}] [clients=${nb_clients}]
```

The default number of slots is 8 with 4 nodes and 3 clients.

```sh
make test [TEST_FILE=${test_file}]
```
The default test is tests/multiple\_entries.cmd with its hostfile tests/multiple\_entries.hostfile

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
