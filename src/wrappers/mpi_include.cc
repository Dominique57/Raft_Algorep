// Created by dominique on 10/24/21.

#include "mpi_include.hh"

#include <chrono>
#include <unistd.h>
#include <spdlog/spdlog.h>
#include <rpc/message.hh>

namespace MPI {

    void Send_Rpc(const Rpc::Rpc &rpc, int dest, int tag, MPI_Comm comm) {
        // FIXME: check MPI_ISend + MPI_Request_free
        const std::string &message = rpc.serialize().dump();
        MPI_Send(message.c_str(), (int) message.size(), MPI_CHAR, dest, tag, comm);
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

    std::unique_ptr<Rpc::RpcResponse> Recv_Rpc(int src, int tag, MPI_Comm comm) {
        size_t messageLen = AnyMessageWaiting(src, tag, comm);
        if (messageLen <= 0)
            return nullptr;

        std::vector<char> buffer(messageLen + 1);
        MPI_Status status;
        MPI_Recv(buffer.data(), (int) messageLen, MPI_CHAR, src, tag, comm, &status);
        buffer[messageLen] = '\0';

        std::string recvMessage(buffer.data());
        json recvJson = json::parse(recvMessage);

        auto recvType = recvJson["type"].get<Rpc::TYPE>();
        auto res = std::unique_ptr<Rpc::Rpc>();
        switch (recvType) {
            case Rpc::TYPE::MESSAGE:
                res = std::make_unique<Rpc::Message>(recvJson);
                break;
        }
        if (res == nullptr) {
            auto errMsg = std::string("Unhandled message type: ") + Rpc::getTypeName(recvType);
            spdlog::error(errMsg);
            throw std::runtime_error(errMsg);
        }

        return std::make_unique<Rpc::RpcResponse>(res, status.MPI_SOURCE);
    }

    std::unique_ptr<Rpc::RpcResponse>
    Recv_Rpc_Timeout(int src, long timeout, int tag, MPI_Comm comm) {
        auto start = std::chrono::steady_clock::now();
        while (true) {
            auto end = std::chrono::steady_clock::now();
            auto countTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                    end - start).count();

            auto msg = Recv_Rpc(src, tag, comm);
            if (msg != nullptr)
                return msg;

            if (timeout < countTime)
                break;
            usleep(100);
        }

        return nullptr;
    }

}