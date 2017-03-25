#ifndef NODE_LIBTORRENT_ALERT_HPP_INCLUDED
#define NODE_LIBTORRENT_ALERT_HPP_INCLUDED

#include <v8.h>
#include <node.h>

#include <exception>
#include <iostream>
#include <libtorrent/alert.hpp>
#include <libtorrent/torrent_handle.hpp>

using namespace v8;

namespace nodelt {
    void bind_alert(Local<Object> target);

    class AlertWrap: public Nan::ObjectWrap {
        public:
            static void Initialize(Local<Object> target);
            static Local<Object> New(const libtorrent::alert* a);
            static const libtorrent::alert* Unwrap(const Local<Object>& obj) {
                return Nan::ObjectWrap::Unwrap<AlertWrap>(obj)->obj_;
            };

        private:
            const libtorrent::alert* obj_;
            AlertWrap();
            ~AlertWrap();

            static Nan::Persistent<Function> constructor;

            static NAN_METHOD(NewInstance);

            static NAN_METHOD(what);
            static NAN_METHOD(type);
            static NAN_METHOD(handle);
            static NAN_METHOD(message);
            static NAN_METHOD(category);
            static NAN_METHOD(resume_data);


            /* some template trickery to workaround alert_cast stuff */
            /* getting the handle */
            template <class T> static const libtorrent::torrent_handle* getHandleInternal(const libtorrent::alert& p)
            {
                auto obj = dynamic_cast<const T*>(&p);
                return obj ? &obj->handle : nullptr;
            }

            template <class T1, class T2, class...Ts> static const libtorrent::torrent_handle* getHandleInternal(const libtorrent::alert& p)
            {
                auto v = getHandleInternal<T1>(p);
                return v ? v : getHandleInternal<T2, Ts...>(p);
            }

            template <class... Ts> static libtorrent::torrent_handle getHandle(const libtorrent::alert& p) // C++14 can return auto here
            {
                auto handle = getHandleInternal<Ts...>(p);

                if (!handle)
                    throw std::exception();

                return *handle;
            }

            /* getting the object */
            template <class T> static libtorrent::alert* getObjectInternal(const libtorrent::alert& p)
            {
                auto obj = dynamic_cast<const T*>(&p);

                return obj ? new T(*obj) : nullptr;
            }

            template <class T1, class T2, class...Ts> static libtorrent::alert* getObjectInternal(const libtorrent::alert& p)
            {
                auto v = getObjectInternal<T1>(p);
                return v ? v : getObjectInternal<T2, Ts...>(p);
            }

            template <class... Ts> static libtorrent::alert* getObject(const libtorrent::alert& p) // C++14 can return auto here
            {
                auto obj = getObjectInternal<Ts...>(p);

                if (!obj)
                    throw std::exception();

                return obj;
            }
    };

};

#endif // NODE_LIBTORRENT_ALERT_HPP_INCLUDED
