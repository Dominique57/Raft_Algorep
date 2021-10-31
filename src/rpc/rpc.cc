#include "rpc.hh"

namespace Rpc {
    json Rpc::serialize() const {
        json j;
        j["type"] = type_;
        j["data"] = serialize_self();
        return j;
    }
}
