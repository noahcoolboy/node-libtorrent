#ifndef NODE_LIBTORRENT_IP_FILTER_HPP_INCLUDED
#define NODE_LIBTORRENT_IP_FILTER_HPP_INCLUDED

#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/ip_filter.hpp>

using namespace v8;

namespace nodelt {
    void bind_ip_filter(Local<Object> target);

    class IpFilterWrap: public Nan::ObjectWrap {
        public:
            static void Initialize(Local<Object> target);
            static Local<Object> New(const libtorrent::ip_filter& f);
            static libtorrent::ip_filter* Unwrap(const Local<Object>& obj) {
                return Nan::ObjectWrap::Unwrap<IpFilterWrap>(obj)->obj_;
            };

        private:
            libtorrent::ip_filter* obj_;
            IpFilterWrap();
            ~IpFilterWrap();

            static Nan::Persistent<Function> constructor;

            static NAN_METHOD(NewInstance);
            static NAN_METHOD(add_rule);
            static NAN_METHOD(access);
            static NAN_METHOD(export_filter);
    };
};

#endif // NODE_LIBTORRENT_IP_FILTER_HPP_INCLUDED
