// Created by dominique on 10/24/21.

#include "rpc.hh"

json Rpc::serialize() const {
    return json::object({{"value", message_ }});
}
