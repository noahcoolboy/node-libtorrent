#ifndef NODE_LIBTORRENT_RSS_HPP_INCLUDED
#define NODE_LIBTORRENT_RSS_HPP_INCLUDED

#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/rss.hpp>

using namespace v8;

namespace nodelt {
    libtorrent::feed_settings feed_settings_from_object(Local<Object> obj);
    Local<Object> feed_settings_to_object(const libtorrent::feed_settings& fs);

    class FeedHandleWrap: public Nan::ObjectWrap {
        public:
            static void Initialize(Local<Object> target);
            static Local<Object> New(const libtorrent::feed_handle& fh);
            static libtorrent::feed_handle* Unwrap(const Local<Object>& obj) {
                return Nan::ObjectWrap::Unwrap<FeedHandleWrap>(obj)->obj_;
            };

        private:
            libtorrent::feed_handle* obj_;
            FeedHandleWrap();
            ~FeedHandleWrap();

            static Nan::Persistent<Function> constructor;

            static NAN_METHOD(NewInstance);
            static NAN_METHOD(update_feed);
            static NAN_METHOD(get_feed_status);
            static NAN_METHOD(set_settings);
            static NAN_METHOD(settings);
    };
};

#endif // NODE_LIBTORRENT_RSS_HPP_INCLUDED
