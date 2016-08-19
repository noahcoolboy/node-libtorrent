#include <string>
#include <sstream>

#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/bencode.hpp>
#include <libtorrent/entry.hpp>

#include "entry.hpp"

using namespace v8;
using namespace node;


namespace nodelt {
    NAN_METHOD(bdecode) {
        Nan::HandleScope scope;

        std::string buf(*Nan::Utf8String(info[0]));
        libtorrent::entry e_ = libtorrent::bdecode(buf.c_str(), buf.c_str() + buf.size());

        info.GetReturnValue().Set(entry_to_object(e_));
    };

    NAN_METHOD(bencode) {
        Nan::HandleScope scope;

        std::ostringstream oss;
        libtorrent::bencode(std::ostream_iterator<char>(oss), entry_from_object(info[0]));

        info.GetReturnValue().Set(Nan::New<String>(oss.str()).ToLocalChecked());
    };

    void bind_bencode(Local<Object> target) {
        Nan::SetMethod(target, "bdecode", bdecode);
        Nan::SetMethod(target, "bencode", bencode);
    };
}; // namespace nodelt
