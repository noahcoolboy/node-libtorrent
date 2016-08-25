#ifndef NODE_LIBTORRENT_SESSION_STATUS_HPP_INCLUDED
#define NODE_LIBTORRENT_SESSION_STATUS_HPP_INCLUDED

#include <v8.h>

#include <libtorrent/session_status.hpp>

using namespace v8;

namespace nodelt {
    Local<Object> session_status_to_object(const libtorrent::session_status& st);
    Local<Object> utp_status_to_object(const libtorrent::utp_status& st);

#ifndef TORRENT_DISABLE_DHT
    Local<Object> dht_lookup_to_object(const libtorrent::dht_lookup& dht);
    Local<Object> dht_routing_bucket_to_object(const libtorrent::dht_routing_bucket& dht);
#endif
};

#endif // NODE_LIBTORRENT_SESSION_STATUS_HPP_INCLUDED
