#ifndef NODE_LIBTORRENT_SESSION_SETTINGS_HPP_INCLUDED
#define NODE_LIBTORRENT_SESSION_SETTINGS_HPP_INCLUDED

#include <v8.h>
#include <nan.h>
#include <libtorrent/session_settings.hpp>

using namespace v8;

namespace nodelt {
    void bind_session_settings(Local<Object> target);

#ifndef TORRENT_DISABLE_DHT
    libtorrent::dht_settings const dht_settings_from_object(Local<Object> obj);
#endif

    Local<Object> proxy_settings_to_object(const libtorrent::proxy_settings& s);
    libtorrent::proxy_settings proxy_settings_from_object(Local<Object> obj);

    Local<Object> session_settings_to_object(const libtorrent::session_settings& s);
    libtorrent::session_settings session_settings_from_object(Local<Object> obj);
};

#endif // NODE_LIBTORRENT_SESSION_SETTINGS_HPP_INCLUDED
