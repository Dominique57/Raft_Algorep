// Created by dominique on 10/24/21.

#pragma once

/// Global processus configuration data
class GlobalConfig {
public:
    /** Initializes the global configuration
     * @param rank : GlobalConfig::rank
     * @param size : GlobalConfig::size
     */
    static void initConfig(int rank, int size);

    /// Current node id
    static int rank;
    /// Total number of node
    static int size;
};

