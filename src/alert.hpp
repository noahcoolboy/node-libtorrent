#ifndef NODE_LIBTORRENT_ALERT_HPP_INCLUDED
#define NODE_LIBTORRENT_ALERT_HPP_INCLUDED

#include <v8.h>
#include <node.h>

#include <vector>
#include <utility>
#include <libtorrent/alert.hpp>
#include <libtorrent/torrent_handle.hpp>

using namespace v8;

namespace nodelt {
    Local<Object> alert_to_object(const libtorrent::alert& alert);
    Local<Object> alert_to_handle(const libtorrent::alert& alert);
    void bind_alert(Local<Object> target);

    /* some template trickery to workaround alert_cast stuff */
    template <class T> bool getHandleInternal(const libtorrent::alert& p, libtorrent::torrent_handle& handle)
    {
        auto obj = dynamic_cast<const T*>(&p);
        if (obj){
          handle = obj->handle;
          return true;
        } else {
          return false;
        }
    }

    template <class T1, class T2, class...Ts>  bool getHandleInternal(const libtorrent::alert& p, libtorrent::torrent_handle& handle)
    {
        return getHandleInternal<T1>(p, handle) || getHandleInternal<T2, Ts...>(p, handle);
    }

    template <class... Ts> libtorrent::torrent_handle getHandle(const libtorrent::alert& p) // C++14 can return auto here
    {
        libtorrent::torrent_handle handle;

        if (!getHandleInternal<Ts...>(p, handle))
            throw "Unknown type!";
        return handle; // move may be called implictly here
    }
};

#endif // NODE_LIBTORRENT_ALERT_HPP_INCLUDED
