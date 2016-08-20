#ifndef NODE_LIBTORRENT_PEER_INFO_PARAMS_HPP_INCLUDED
#define NODE_LIBTORRENT_PEER_INFO_PARAMS_HPP_INCLUDED

#include <v8.h>

#include <libtorrent/peer_info.hpp>

using namespace v8;

namespace nodelt {
    void bind_peer_info(Local<Object> target);
    Local<Array> get_tcp_endpoint(const libtorrent::tcp::endpoint& ep);
    Local<Object> peer_info_to_object(const libtorrent::peer_info& pi);
    Local<Array> get_pieces(const libtorrent::peer_info& pi);
};

#endif // NODE_LIBTORRENT_PEER_INFO_PARAMS_HPP_INCLUDED
