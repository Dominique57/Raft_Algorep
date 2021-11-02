#pragma once


namespace Client {

    class Client {
    public:
        /**
         * @brief Default constructor.
         */
        Client() = default;

        int request_leader_id();

    protected:
        int timeout;
        int leaderId;
    };
}
