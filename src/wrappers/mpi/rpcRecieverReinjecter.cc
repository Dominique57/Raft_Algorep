#include "rpcRecieverReinjecter.hh"

namespace Rpc {

    std::unique_ptr<RpcResponse> RpcRecieverReinjecter::get_rpc(int src, int tag, MPI_Comm comm) {
        if (reinjected_messages_.empty())
            return MPI::Recv_Rpc(src, tag, comm);

        auto res = std::move(reinjected_messages_.front());
        reinjected_messages_.pop();
        return res;
    }

    std::unique_ptr<RpcResponse> RpcRecieverReinjecter::get_rpc_timeout(int src, long timeout, int tag, MPI_Comm comm) {
        if (reinjected_messages_.empty())
            return MPI::Recv_Rpc_Timeout(src, timeout, tag, comm);

        auto res = std::move(reinjected_messages_.front());
        reinjected_messages_.pop();
        return res;
    }

}