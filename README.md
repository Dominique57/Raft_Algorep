# Distrubted Systems: Raf implementation

Project for ALGOREP course at [EPITA](https://www.epita.fr/).

## What is it ?

This is a group project where we implemented the basics of the
[RAFT](https://raft.github.io/) paper. This implementation is able to achieve
consensus for leader election and log replication.

## Setup

To be able to compile and run the program you need :
- [CMake](https://cmake.org/) (build system)
- [MPI](https://www.open-mpi.org/) (library)
- Make (launch / build / tests automation)
- Doxygen (for documentation)

## Documentation generation

Documentation is handled by in-code comments. To generate interactive
documentation, use [Doxygen](https://www.doxygen.nl/index.html). For simplicty you can simply write :
```bash
42sh$ make doc
```
and navigate "res/docs/html/index.html" with your favorite browser.


## Usage

### Starting the system

- The default number of slots is 8 with 4 nodes and 3 clients (there is always
  a controller).
- The default test_file is "tests/simple\_1\_entry.cmd".

> To launch an interactive session :
```sh
42sh$ make [slots=${nb_slots}] [nodes=${nb_nodes}] [clients=${nb_clients}]
```

> To launch a test setup in an interactive session (required hostfile copied
> from res/ with correct slots specified !)
```sh
42sh$ make test [TEST_FILE=${test_file}]
```

> To launch a test and automatically check for correct behaviour :

```sh
42sh$ ./tests/test.sh [test_file]
```

> You can launch all tests :

```sh
42sh$ ./tests/test_all.sh [test_directory]
```

### REPL: controller commands

The interactive session gives you control of a controller, which is a node in
the System. The controller is the one that interacts with the system to emulate
real-case behaviours.

* `HELP`: display help message
* `CRASH [rank]`: crash the given process
* `ENTRY [client_rank] [cmd]`: add given cmd to log of a given client
* `PRINT_LOG [node_rank]`: prints the log of a given node or all nodes
* `RECOVERY [rank]`: un-crashes the given process
* `SLEEP [milliseconds]`: sleep the controller for milliseconds
* `SPEED [rank] [speed]`: set speed for the given process, available speed:
  low, medium, high
* `START [client_rank]`: start the given client
* `STATUS [rank]`: display information for the given process or for all processes

## Known issues
- The system can fail if there are too many messages to be sent from the leader
  to a follower. This issue is related to the fact that there are too much data
  and the json isn't fully sent and thus doesn't get parsed correctly.
> A possible fix would be to limit the maximum ammount of message to be sent
> from a leader during synchronisation
- The command parsing is very manual and could use streaming operators ">>".

## Contributions
- Dominique MICHEL <dominique.michel@epita.fr>
- Léa MASSELLES <lea.masselles@epita.fr>
- Sabine HU <sabine.hu@epita.fr>
- Macha CALLISTO <macha.callisto@epita.fr>

The raft consensus for leader election has been written by *Dominique MICHEL*
and *Lea MASSELLES*.

The raft consensus for log replication has been written
by *Dominique MICHEL* and *Macha CALLISTO*.

The raft Controller, Client and
Node communication system have been written by *Sabine HU, *Macha CALLISTO**
and *Lea MASSELLES*.

The raft test system with consensus checking has been written by *Dominique
MICHEL*.
