# Algoreppppp

system distribuer:
Système clients/serveurs avec un mécanisme permettant
de contrôler ou d’injecter des fautes dans le système. L’idée générale est la suivante : les clients proposent des
valeurs/commandes aux serveurs.

# REPL: controller commands

* `STATUS [rank]`: display information for the given process or for all processes
* `CRASH [rank]`: crash the given process
* `SPEED [rank] [speed]`: set speed for the given process, available speed: low, medium, high
* `START [client_rank]`: start the given client
