#ifndef NODE_LIBTORRENT_ALERT_HPP_INCLUDED
#define NODE_LIBTORRENT_ALERT_HPP_INCLUDED

#include <v8.h>
#include <node.h>

#include <libtorrent/alert.hpp>

using namespace v8;

namespace nodelt {
    Local<Object> alert_to_object(const libtorrent::alert& alert);
    void bind_alert(Local<Object> target);
};

#endif // NODE_LIBTORRENT_ALERT_HPP_INCLUDED
