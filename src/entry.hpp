#ifndef NODE_LIBTORRENT_ENTRY_HPP_INCLUDED
#define NODE_LIBTORRENT_ENTRY_HPP_INCLUDED

#include <v8.h>

#include <libtorrent/entry.hpp>

using namespace v8;

namespace nodelt {
    Local<Value> entry_to_object(const libtorrent::entry& e_);
    libtorrent::entry entry_from_object(Local<Value> obj);
};

#endif // NODE_LIBTORRENT_ENTRY_HPP_INCLUDED
