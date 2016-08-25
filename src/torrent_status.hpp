#ifndef NODE_LIBTORRENT_TORRENT_STATUS_HPP_INCLUDED
#define NODE_LIBTORRENT_TORRENT_STATUS_HPP_INCLUDED

#include <v8.h>

#include <libtorrent/torrent_handle.hpp>

using namespace v8;

namespace nodelt {
    void bind_torrent_status(Local<Object> target);

    Local<Array> bitfield_to_array(const libtorrent::bitfield& bf);
    Local<Object> time_duration_to_object(const boost::posix_time::time_duration& td);
    Local<Value> torrent_status_to_object(const libtorrent::torrent_status& st);
};

#endif // NODE_LIBTORRENT_TORRENT_STATUS_HPP_INCLUDED
