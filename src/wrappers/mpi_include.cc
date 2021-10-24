// Created by dominique on 10/24/21.

#include "mpi_include.hh"

#include <chrono>
#include <unistd.h>

namespace MPI {
    bool MPI_Recv_Timeout(void *data, int count, MPI_Datatype datatype, int source,
                          int tag, MPI_Comm communicator, MPI_Status *status,
                          unsigned long timeout) {
        MPI_Request request;
        MPI_Irecv(data, count, datatype, source, tag, communicator, &request);

        int hasMessage = false;
        auto start = std::chrono::steady_clock::now();

        while (true) {
            auto end = std::chrono::steady_clock::now();
            auto countTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                    end - start).count();
            MPI_Test(&request, &hasMessage, status);
            if (hasMessage)
                break;

            if (timeout < countTime) {
                MPI_Cancel(&request);
                break;
            }
            usleep(100);
        }
        return hasMessage;
    }

    size_t AnyMessageWaiting(int source, int tag, MPI_Comm comm) {
        MPI_Status mpi_status;
        int flag;
        MPI_Iprobe(source, tag, comm, &flag, &mpi_status);
        if (!flag)
            return 0;

        int messageLen = 0;
        MPI_Get_count(&mpi_status, MPI_CHAR, &messageLen);
        return messageLen;
    }

    void Send_Rpc(const Rpc &rpc, int dest, int tag, MPI_Comm comm) {
        const json serializeRpc = rpc.serialize();
        const std::string &message = serializeRpc.dump();
        MPI_Send(message.c_str(), (int) message.size(), MPI_CHAR, dest, tag, comm);
    }

    Rpc Recv_Rpc(int src, int tag, MPI_Comm comm) {
        size_t messageLen = AnyMessageWaiting(src, tag, comm);
        if (messageLen <= 0)
            throw std::logic_error("FIXME: TODO: Not implemented !");

        std::vector<char> buffer(messageLen + 1);
        MPI_Recv(buffer.data(), (int) messageLen, MPI_CHAR, src, tag, comm, MPI_STATUSES_IGNORE);
        buffer[messageLen] = '\0';

        std::string recvMessage(buffer.data());
        json recvJson = json::parse(recvMessage);
        return Rpc(recvJson);
    }
}