#include "mpi_include.hh"

#include <chrono>
#include <spdlog/spdlog.h>
#include <unistd.h>

#include "rpc/message.hh"
#include "rpc/requestVote.hh"
#include "rpc/appendEntries.hh"
#include "rpc/requestLeader.hh"
#include "rpc/controllerRequest.hh"

namespace MPI {

    void Send_Rpc(const Rpc::Rpc &rpc, const int& dest, const int tag, const MPI_Comm comm) {
        const std::string &message = rpc.serialize().dump();
        MPI_Request request;
        MPI_Isend(message.c_str(), (int) message.size(), MPI_CHAR, dest, tag, comm, &request);

        const auto res = MPI_Request_free(&request);
        if (res != MPI_SUCCESS)
            spdlog::error("Failed to free request: {}", res);
    }

    size_t AnyMessageWaiting(const int& src, const int tag, const MPI_Comm comm) {
        MPI_Status mpi_status;
        int flag;
        MPI_Iprobe(src, tag, comm, &flag, &mpi_status);
        if (!flag)
            return 0;

        int messageLen = 0;
        MPI_Get_count(&mpi_status, MPI_CHAR, &messageLen);
        return messageLen;
    }

    std::unique_ptr<Rpc::RpcResponse> Recv_Rpc(const int& src, const int tag, const MPI_Comm comm) {
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
                res = std::make_unique<Rpc::Message>(recvJson["data"]);
                break;
            case Rpc::TYPE::REQUEST_VOTE:
                res = std::make_unique<Rpc::RequestVote>(recvJson["data"]);
                break;
            case Rpc::TYPE::REQUEST_VOTE_RESPONSE:
                res = std::make_unique<Rpc::RequestVoteResponse>(recvJson["data"]);
                break;
            case Rpc::TYPE::APPEND_ENTRIES:
                res = std::make_unique<Rpc::AppendEntries>(recvJson["data"]);
                break;
            case Rpc::TYPE::APPEND_ENTRIES_RESPONSE:
                res = std::make_unique<Rpc::AppendEntriesResponse>(recvJson["data"]);
                break;
            case Rpc::TYPE::REQUEST_LEADER:
                res = std::make_unique<Rpc::RequestLeader>(recvJson["data"]);
                break;
            case Rpc::TYPE::REQUEST_LEADER_RESPONSE:
                res = std::make_unique<Rpc::RequestLeaderResponse>(recvJson["data"]);
                break;
            case Rpc::TYPE::CONTROLLER_REQUEST:
                res = std::make_unique<Rpc::ControllerRequest>(recvJson["data"]);
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
    Recv_Rpc_Timeout(const int& src, const long& timeout, const int tag, const MPI_Comm comm) {
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
