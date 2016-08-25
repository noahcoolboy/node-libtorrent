#ifndef NODE_LIBTORRENT_STORAGE_HPP_INCLUDED
#define NODE_LIBTORRENT_STORAGE_HPP_INCLUDED

#include <v8.h>

using namespace v8;

namespace nodelt {
    void bind_storage(Local<Object> target);
}

#endif // NODE_LIBTORRENT_STORAGE_HPP_INCLUDED
