#ifndef NODE_LIBTORRENT_SESSION_HPP_INCLUDED
#define NODE_LIBTORRENT_SESSION_HPP_INCLUDED

#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/session.hpp>

using namespace v8;

namespace nodelt {
    void bind_session(Local<Object> target);

    class SessionWrap: public Nan::ObjectWrap {
        public:
            static void Initialize(Local<Object> target);
            static libtorrent::session* Unwrap(const Local<Object>& obj) {
                return Nan::ObjectWrap::Unwrap<SessionWrap>(obj)->obj_;
            };

        private:
            libtorrent::session* obj_;
            SessionWrap(Local<Array> args = Nan::New<Array>());
            ~SessionWrap();

            static NAN_METHOD(NewInstance);

            static NAN_METHOD(save_state);
            static NAN_METHOD(load_state);

            static NAN_METHOD(post_torrent_updates);

            static NAN_METHOD(get_torrents);

            static NAN_METHOD(find_torrent);

            static NAN_METHOD(add_torrent);
            static NAN_METHOD(async_add_torrent);

            static NAN_METHOD(pause);
            static NAN_METHOD(resume);
            static NAN_METHOD(is_paused);

            static NAN_METHOD(status);
            static NAN_METHOD(get_cache_status);

            static NAN_METHOD(get_cache_info);

            static NAN_METHOD(add_feed);
            static NAN_METHOD(remove_feed);
            static NAN_METHOD(get_feeds);

#ifndef TORRENT_DISABLE_DHT
            static NAN_METHOD(start_dht);
            static NAN_METHOD(stop_dht);
            static NAN_METHOD(set_dht_settings);
            static NAN_METHOD(add_dht_node);
            static NAN_METHOD(add_dht_router);
            static NAN_METHOD(is_dht_running);
#endif

#ifndef TORRENT_DISABLE_EXTENSIONS
            static NAN_METHOD(add_extension);
#endif

#ifndef TORRENT_DISABLE_GEO_IP
            static NAN_METHOD(load_asnum_db);
            static NAN_METHOD(load_country_db);
#endif

            static NAN_METHOD(set_ip_filter);
            static NAN_METHOD(get_ip_filter);

            static NAN_METHOD(set_peer_id);
            static NAN_METHOD(id);

            static NAN_METHOD(is_listening);
            static NAN_METHOD(listen_port);
            static NAN_METHOD(listen_on);

            static NAN_METHOD(remove_torrent);

            static NAN_METHOD(set_settings);
            static NAN_METHOD(settings);

            static NAN_METHOD(set_proxy);
            static NAN_METHOD(proxy);

#if TORRENT_USE_I2P
            static NAN_METHOD(set_i2p_proxy);
            static NAN_METHOD(i2p_proxy);
#endif

            static NAN_METHOD(pop_alert);
            static NAN_METHOD(pop_alerts);
            static NAN_METHOD(set_alert_mask);
            static NAN_METHOD(wait_for_alert);

            static NAN_METHOD(start_lsd);
            static NAN_METHOD(start_natpmp);
            static NAN_METHOD(start_upnp);

            static NAN_METHOD(stop_lsd);
            static NAN_METHOD(stop_natpmp);
            static NAN_METHOD(stop_upnp);
    };
};

#endif // NODE_LIBTORRENT_SESSION_HPP_INCLUDED
