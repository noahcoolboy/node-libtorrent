#ifndef NODE_LIBTORRENT_FINGERPRINT_HPP_INCLUDED
#define NODE_LIBTORRENT_FINGERPRINT_HPP_INCLUDED

#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/fingerprint.hpp>

using namespace v8;

namespace nodelt {
    void bind_fingerprint(Local<Object> target);

    class FingerprintWrap: public Nan::ObjectWrap {
        public:
            static void Initialize(Local<Object> target);
            static libtorrent::fingerprint* Unwrap(const Local<Object>& obj) {
                return Nan::ObjectWrap::Unwrap<FingerprintWrap>(obj)->obj_;
            };

        private:
            libtorrent::fingerprint* obj_;

            static Nan::Persistent<Function> constructor;
            FingerprintWrap(const char* id_string, int major, int minor, int revision, int tag);
            ~FingerprintWrap();

            static NAN_METHOD(NewInstance);
            static NAN_METHOD(to_string);
    };
};

#endif // NODE_LIBTORRENT_FINGERPRINT_HPP_INCLUDED
