// Created by dominique on 10/24/21.

#pragma once

class GlobalConfig {
public:
    static void initConfig(int rank, int size);

    static int rank;
    static int size;
};

