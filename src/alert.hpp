#ifndef NODE_LIBTORRENT_ALERT_HPP_INCLUDED
#define NODE_LIBTORRENT_ALERT_HPP_INCLUDED

#include <v8.h>
#include <node.h>

#include <vector>
#include <libtorrent/alert.hpp>

using namespace v8;

namespace nodelt {
    Local<Object> alert_to_object(const libtorrent::alert& alert);
    Local<Object> alert_to_handle(const libtorrent::alert& alert);
    void bind_alert(Local<Object> target);

    std::vector<libtorrent::alert> vector_alert_t;
};

#endif // NODE_LIBTORRENT_ALERT_HPP_INCLUDED
