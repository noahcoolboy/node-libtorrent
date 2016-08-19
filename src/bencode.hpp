#ifndef NODE_LIBTORRENT_BENCODE_HPP_INCLUDED
#define NODE_LIBTORRENT_BENCODE_HPP_INCLUDED

#include <node.h>
#include <nan.h>
#include <v8.h>

using namespace v8;

namespace nodelt {
    static NAN_METHOD(bdecode);
    static NAN_METHOD(bencode);
    void bind_bencode(Local<Object> target);
};

#endif // NODE_LIBTORRENT_BENCODE_HPP_INCLUDED
