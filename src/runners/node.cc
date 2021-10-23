// Created by dominique on 10/22/21.

#include "node.hh"

void Node::receive_message()
{
    char buffer[1025] = {0};
    MPI_Status status;
    MPI_Recv(buffer, 1024, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    auto recievedSmthing = MPI_Recv_Timeout(buffer, 1024, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, 0,
                                            MPI_COMM_WORLD, &status, 100);
    if (recievedSmthing) {
        std::cout << "[RECV] from " << status.MPI_SOURCE << ": `" << buffer << '`' << std::endl;
    } else {
        std::cout << "Received nothing !" << std::endl;
    }
}

void Node::send_message()
{
    int timeToWait = std::rand() % 1000 + 500;
    usleep(timeToWait * 1000);

    std::stringstream buffer{};
    buffer << "Node: " << rank_ << " sent a message !";
    std::string msg = buffer.str();
    MPI_Send(msg.c_str(), msg.size(), MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
    std::cout << "[SEND] (" << rank_ << "): " << msg << std::endl;
}

bool
Node::MPI_Recv_Timeout(void *data, int count, MPI_Datatype datatype, int source,
                       int tag, MPI_Comm communicator, MPI_Status *status,
                       unsigned long timeout)
{
    MPI_Request request;
    MPI_Irecv(data, count, datatype, source, tag, communicator, &request);

    int hasMessage = false;
    auto start = std::chrono::steady_clock::now();

    while (true) {
        auto end = std::chrono::steady_clock::now();
        auto count = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        MPI_Test(&request, &hasMessage, status);
        if (hasMessage)
            break;

        if (timeout < count) {
            MPI_Cancel(&request);
            break;
        }
        usleep(100);
    }
    return hasMessage;
}