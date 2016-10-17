#ifndef NODE_LIBTORRENT_ALERT_HPP_INCLUDED
#define NODE_LIBTORRENT_ALERT_HPP_INCLUDED

#include <v8.h>
#include <node.h>

#include <typeinfo>
#include <exception>
#include <iostream>
#include <libtorrent/alert.hpp>
#include <libtorrent/torrent_handle.hpp>

using namespace v8;

namespace nodelt {
    Local<Object> alert_to_object(const libtorrent::alert& alert);
    Local<Value> alert_to_handle(const libtorrent::alert& alert);
    void bind_alert(Local<Object> target);

    class AlertWrap: public Nan::ObjectWrap {
        public:
            static void Initialize(Local<Object> target);
            static Local<Object> New(const libtorrent::alert& a);
            static libtorrent::alert* Unwrap(const Local<Object>& obj) {
                return Nan::ObjectWrap::Unwrap<AlertWrap>(obj)->obj_;
            };

        private:
            libtorrent::alert* obj_;
            AlertWrap();
            ~AlertWrap();

            static Nan::Persistent<Function> constructor;

            static NAN_METHOD(NewInstance);

            static NAN_METHOD(what);
            static NAN_METHOD(type);
            static NAN_METHOD(handle);
            static NAN_METHOD(message);
            static NAN_METHOD(category);


            /* some template trickery to workaround alert_cast stuff */
            template <class T> static bool getHandleInternal(const libtorrent::alert& p, libtorrent::torrent_handle& handle)
            {
                auto obj = dynamic_cast<const T*>(&p);
                if (obj){
                    //auto new T(*obj)
                  handle = obj->handle;
                  return true;
                } else {
                  return false;
                }
            }

            template <class T1, class T2, class...Ts> static bool getHandleInternal(const libtorrent::alert& p, libtorrent::torrent_handle& handle)
            {
                return getHandleInternal<T1>(p, handle) || getHandleInternal<T2, Ts...>(p, handle);
            }

            template <class... Ts> static libtorrent::torrent_handle getHandle(const libtorrent::alert& p) // C++14 can return auto here
            {
                libtorrent::torrent_handle handle;

                if (!getHandleInternal<Ts...>(p, handle)){
                    std::cout<<"matching class for " << typeid(p).name() << " not found"<<std::endl;
                    throw std::exception(); // needed type is not found in the list
                }
                return handle; // move may be called implictly here
            }
    };

};

#endif // NODE_LIBTORRENT_ALERT_HPP_INCLUDED
