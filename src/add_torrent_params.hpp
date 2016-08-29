#ifndef NODE_LIBTORRENT_ADD_TORRENT_PARAMS_HPP_INCLUDED
#define NODE_LIBTORRENT_ADD_TORRENT_PARAMS_HPP_INCLUDED

#include <v8.h>

#include <libtorrent/storage_defs.hpp>
#include <libtorrent/add_torrent_params.hpp>

using namespace v8;

namespace nodelt {
    libtorrent::add_torrent_params add_torrent_params_from_object(Local<Object> obj);
    void bind_add_torrent_params(Local<Object> target);
};

#endif // NODE_LIBTORRENT_ADD_TORRENT_PARAMS_HPP_INCLUDED
