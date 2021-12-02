#pragma once

/// Global processus configuration data
class GlobalConfig {
public:
    /**
     * @brief Initializes the global configuration
     * @param rank          GlobalConfig::rank
     * @param nb_node       Number of nodes
     * @param nb_client     Number of clients
     */
    static void initConfig(const int& rank, const int& nb_node, const int& nb_client);

    /**
     * @brief Check if the current process is the controller
     * @return True if the current process is the controller
     */
    static bool is_controller();

    /**
     * @brief Check if the current process is a client
     * @return True if the current process is a client
     */
    static bool is_client();

    /**
    * @brief Check if the current process is a server
    * @return True if the current process is a server
    */
    static bool is_node();

    /**
     * @brief Check if the process ID is the controller
     * @param rank          The process ID
     * @return True if the process ID is the controller
     */
    static bool is_controller(const int& rank);

    /**
     * @brief Check if the process ID is a client
     * @param rank          The process ID
     * @return True if the process ID is a client
     */
    static bool is_client(const int& rank);

    /**
     * @brief Check if the process ID is a node
     * @param rank          The process ID
     * @return True if the process ID is a node
     */
    static bool is_node(const int& rank);

    /**
     * @brief Get the number of nodes
     * @return The number of node
     */
    static int nb_node();

    /// Current node id
    static int rank;
    /// Total number of node
    static int nb_node_min;
    static int nb_node_max;
    /// Total number of client
    static int nb_client_min;
    static int nb_client_max;
};

