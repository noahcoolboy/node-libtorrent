#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/disk_io_thread.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/extensions.hpp>
#include <libtorrent/fingerprint.hpp>
#include <libtorrent/ip_filter.hpp>
#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/rss.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/session_status.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/torrent_info.hpp>

#include <libtorrent/extensions/lt_trackers.hpp>
#include <libtorrent/extensions/metadata_transfer.hpp>
#include <libtorrent/extensions/smart_ban.hpp>
#include <libtorrent/extensions/ut_metadata.hpp>
#include <libtorrent/extensions/ut_pex.hpp>

#include "add_torrent_params.hpp"
#include "alert.hpp"
#include "entry.hpp"
#include "fingerprint.hpp"
#include "ip_filter.hpp"
#include "rss.hpp"
#include "session.hpp"
#include "session_settings.hpp"
#include "session_status.hpp"
#include "torrent_handle.hpp"
#include "torrent_info.hpp"

using namespace v8;

namespace nodelt {
    void bind_session(Local<Object> target) {
        SessionWrap::Initialize(target);

        // set libtorrent::session::listen_on_flags_t
        Local<Object> listen_on_flags_t = Nan::New<Object>();

        listen_on_flags_t->Set(Nan::New("listen_reuse_address").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::listen_reuse_address));
        listen_on_flags_t->Set(Nan::New("listen_no_system_port").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::listen_no_system_port));

        target->Set(Nan::New("listen_on_flags_t").ToLocalChecked(), listen_on_flags_t);

        // set libtorrent::session::save_state_flags_t
        Local<Object> save_state_flags_t = Nan::New<Object>();

        save_state_flags_t->Set(Nan::New("save_settings").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::save_settings));
        save_state_flags_t->Set(Nan::New("save_dht_settings").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::save_dht_settings));
        save_state_flags_t->Set(Nan::New("save_dht_state").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::save_dht_state));
        save_state_flags_t->Set(Nan::New("save_proxy").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::save_proxy));
        save_state_flags_t->Set(Nan::New("save_i2p_proxy").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::save_i2p_proxy));
        save_state_flags_t->Set(Nan::New("save_encryption_settings").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::save_encryption_settings));
        save_state_flags_t->Set(Nan::New("save_as_map").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::save_as_map));
        save_state_flags_t->Set(Nan::New("save_feeds").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::save_feeds));

        target->Set(Nan::New("save_state_flags_t").ToLocalChecked(), save_state_flags_t);

        // set libtorrent::session::options_t
        Local<Object> options_t = Nan::New<Object>();

        //    options_t->Set(Nan::New("none"),
        //      Nan::New<Integer>(libtorrent::session::none));
        options_t->Set(Nan::New("delete_files").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::delete_files));

        target->Set(Nan::New("options_t").ToLocalChecked(), options_t);

        // set libtorrent::session::session_flags_t
        Local<Object> session_flags_t = Nan::New<Object>();

        session_flags_t->Set(Nan::New("start_default_features").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::add_default_plugins));
        session_flags_t->Set(Nan::New("start_default_features").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session::start_default_features));

        target->Set(Nan::New("session_flags_t").ToLocalChecked(), session_flags_t);
    };

    SessionWrap::SessionWrap(Local<Array> args) {
        int args_len = args->Length();

        if (args_len >= 1 && args_len <= 5) {
            libtorrent::fingerprint* print_ = FingerprintWrap::Unwrap(args->Get(0)->ToObject());

            if (args_len == 1) {
                obj_ = new libtorrent::session(*print_);
                return;
            }

            if (args->Get(1)->IsArray()) {
                Local<Array> pr = args->Get(0).As<Array>();
                std::pair<int, int> port_range = std::make_pair(pr->Get(0)->IntegerValue(), pr->Get(1)->IntegerValue());

                if (args_len == 2) {
                    obj_ = new libtorrent::session(*print_, port_range);
                    return;
                }

                std::string interface(*Nan::Utf8String(args->Get(2)->ToString()));

                if (args_len == 3)
                    obj_ = new libtorrent::session(*print_, port_range, interface.c_str());
                else if (args_len == 4)
                    obj_ = new libtorrent::session(*print_, port_range, interface.c_str(), args->Get(3)->Int32Value());
                else
                    obj_ = new libtorrent::session(*print_, port_range, interface.c_str(), args->Get(3)->Int32Value(), args->Get(4)->Int32Value());
            } else {
                if (args_len == 2)
                    obj_ = new libtorrent::session(*print_, args->Get(1)->Int32Value());
                else
                    obj_ = new libtorrent::session(*print_, args->Get(1)->Int32Value(), args->Get(2)->Int32Value());
            }
        } else {
            obj_ = new libtorrent::session();
        }
    };

    SessionWrap::~SessionWrap() {
        delete obj_;
    };

    void SessionWrap::Initialize(Local<Object> target) {
        Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
        tpl->SetClassName(Nan::New("session").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetPrototypeMethod(tpl, "save_state", save_state);
        Nan::SetPrototypeMethod(tpl, "load_state", load_state);

        Nan::SetPrototypeMethod(tpl, "post_torrent_updates", post_torrent_updates);

        Nan::SetPrototypeMethod(tpl, "get_torrents", get_torrents);

        Nan::SetPrototypeMethod(tpl, "find_torrent", find_torrent);

        Nan::SetPrototypeMethod(tpl, "add_torrent", add_torrent);
        Nan::SetPrototypeMethod(tpl, "async_add_torrent", async_add_torrent);

        Nan::SetPrototypeMethod(tpl, "pause", pause);
        Nan::SetPrototypeMethod(tpl, "resume", resume);
        Nan::SetPrototypeMethod(tpl, "is_paused", is_paused);

        Nan::SetPrototypeMethod(tpl, "status", status);
        Nan::SetPrototypeMethod(tpl, "get_cache_status", get_cache_status);

        Nan::SetPrototypeMethod(tpl, "get_cache_info", get_cache_info);

        Nan::SetPrototypeMethod(tpl, "add_feed", add_feed);
        Nan::SetPrototypeMethod(tpl, "remove_feed", remove_feed);
        Nan::SetPrototypeMethod(tpl, "get_feeds", get_feeds);

#ifndef TORRENT_DISABLE_DHT
        Nan::SetPrototypeMethod(tpl, "start_dht", start_dht);
        Nan::SetPrototypeMethod(tpl, "stop_dht", stop_dht);
        Nan::SetPrototypeMethod(tpl, "set_dht_settings", set_dht_settings);
        Nan::SetPrototypeMethod(tpl, "add_dht_node", add_dht_node);
        Nan::SetPrototypeMethod(tpl, "add_dht_router", add_dht_router);
        Nan::SetPrototypeMethod(tpl, "is_dht_running", is_dht_running);
#endif

#ifndef TORRENT_DISABLE_EXTENSIONS
        Nan::SetPrototypeMethod(tpl, "add_extension", add_extension);
#endif

#ifndef TORRENT_DISABLE_DHT
        Nan::SetPrototypeMethod(tpl, "load_asnum_db", load_asnum_db);
        Nan::SetPrototypeMethod(tpl, "load_country_db", load_country_db);
#endif

        Nan::SetPrototypeMethod(tpl, "set_ip_filter", set_ip_filter);
        Nan::SetPrototypeMethod(tpl, "get_ip_filter", get_ip_filter);

        Nan::SetPrototypeMethod(tpl, "set_peer_id", set_peer_id);
        Nan::SetPrototypeMethod(tpl, "id", id);

        Nan::SetPrototypeMethod(tpl, "is_listening", is_listening);
        Nan::SetPrototypeMethod(tpl, "listen_port", listen_port);
        Nan::SetPrototypeMethod(tpl, "listen_on", listen_on);

        Nan::SetPrototypeMethod(tpl, "remove_torrent", remove_torrent);

        Nan::SetPrototypeMethod(tpl, "set_settings", set_settings);
        Nan::SetPrototypeMethod(tpl, "settings", settings);

        Nan::SetPrototypeMethod(tpl, "set_proxy", set_proxy);
        Nan::SetPrototypeMethod(tpl, "proxy", proxy);

#if TORRENT_USE_I2P
        Nan::SetPrototypeMethod(tpl, "set_i2p_proxy", set_i2p_proxy);
        Nan::SetPrototypeMethod(tpl, "i2p_proxy", i2p_proxy);
#endif

        Nan::SetPrototypeMethod(tpl, "pop_alert", pop_alert);
        Nan::SetPrototypeMethod(tpl, "pop_alerts", pop_alerts);
        Nan::SetPrototypeMethod(tpl, "set_alert_mask", set_alert_mask);
        Nan::SetPrototypeMethod(tpl, "wait_for_alert", wait_for_alert);

        Nan::SetPrototypeMethod(tpl, "start_lsd", start_lsd);
        Nan::SetPrototypeMethod(tpl, "start_natpmp", start_natpmp);
        Nan::SetPrototypeMethod(tpl, "stop_upnp", stop_upnp);

        Nan::SetPrototypeMethod(tpl, "stop_lsd", stop_lsd);
        Nan::SetPrototypeMethod(tpl, "stop_natpmp", stop_natpmp);
        Nan::SetPrototypeMethod(tpl, "stop_upnp", stop_upnp);

        target->Set(Nan::New("session").ToLocalChecked(), tpl->GetFunction());
    };

    NAN_METHOD(SessionWrap::NewInstance) {
        Nan::HandleScope scope;

        if (!info.IsConstructCall()) {
            Nan::ThrowTypeError("Use the new operator to create instances of this object.");
            return;
        }

        Local<Array> data = Nan::New<Array>();

        for (int i = 0, e = info.Length(); i < e; ++i)
            data->Set(i, info[i]);

        SessionWrap* s = new SessionWrap(data);
        s->Wrap(info.This());

        info.GetReturnValue().Set(info.This());
    };

    NAN_METHOD(SessionWrap::save_state) {
        Nan::HandleScope scope;

        libtorrent::session* s = SessionWrap::Unwrap(info.This());
        libtorrent::entry e;

        if (info.Length() == 1)
            s->save_state(e, info[0]->IntegerValue());
        else
            s->save_state(e);

        info.GetReturnValue().Set(entry_to_object(e));
    };

    NAN_METHOD(SessionWrap::load_state) {
        Nan::HandleScope scope;

        // deprecated! need lazy_entry...
        libtorrent::entry e = entry_from_object(info[0]);
        SessionWrap::Unwrap(info.This())->load_state(e);

        info.GetReturnValue().SetUndefined();
    };


    NAN_METHOD(SessionWrap::post_torrent_updates) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->post_torrent_updates();

        info.GetReturnValue().SetUndefined();
    };


    NAN_METHOD(SessionWrap::get_torrents) {
        Nan::HandleScope scope;

        std::vector<libtorrent::torrent_handle> torrents;
        torrents = SessionWrap::Unwrap(info.This())->get_torrents();

        Local<Array> ret = Nan::New<Array>();

        for (std::vector<libtorrent::torrent_handle>::iterator i(torrents.begin()), e(torrents.end()); i != e; ++i)
            ret->Set(ret->Length(), TorrentHandleWrap::New(*i));

        info.GetReturnValue().Set(ret);
    };

    NAN_METHOD(SessionWrap::find_torrent) {
        Nan::HandleScope scope;

        libtorrent::sha1_hash info_hash;
        libtorrent::from_hex(*Nan::Utf8String(info[0]), 40, (char*)&info_hash[0]);
        libtorrent::torrent_handle th = SessionWrap::Unwrap(info.This())->find_torrent(info_hash);

        info.GetReturnValue().Set(TorrentHandleWrap::New(th));
    };

    NAN_METHOD(SessionWrap::add_torrent) {
        Nan::HandleScope scope;

        libtorrent::torrent_handle th;
        libtorrent::error_code ec;

        th = SessionWrap::Unwrap(info.This())->add_torrent(add_torrent_params_from_object(info[0]->ToObject()), ec);

        info.GetReturnValue().Set(TorrentHandleWrap::New(th));
    };

    NAN_METHOD(SessionWrap::async_add_torrent) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->async_add_torrent(add_torrent_params_from_object(info[0]->ToObject()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::pause) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->pause();

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::resume) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->resume();

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::is_paused) {
        Nan::HandleScope scope;

        libtorrent::session* s = SessionWrap::Unwrap(info.This());

        info.GetReturnValue().Set(Nan::New<Boolean>(s->is_paused()));
    };

    NAN_METHOD(SessionWrap::status) {
        Nan::HandleScope scope;

        libtorrent::session_status st = SessionWrap::Unwrap(info.This())->status();

        info.GetReturnValue().Set(session_status_to_object(st));
    };

    NAN_METHOD(SessionWrap::get_cache_status) {
        Nan::HandleScope scope;

        libtorrent::cache_status st = SessionWrap::Unwrap(info.This())->get_cache_status();
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("blocks_written").ToLocalChecked(), Nan::New<Number>(st.blocks_written));
        obj->Set(Nan::New("writes").ToLocalChecked(), Nan::New<Number>(st.writes));
        obj->Set(Nan::New("blocks_read").ToLocalChecked(), Nan::New<Number>(st.blocks_read));
        obj->Set(Nan::New("blocks_read_hit").ToLocalChecked(), Nan::New<Number>(st.blocks_read_hit));
        obj->Set(Nan::New("reads").ToLocalChecked(), Nan::New<Number>(st.reads));
        obj->Set(Nan::New("queued_bytes").ToLocalChecked(), Nan::New<Number>(st.queued_bytes));
        obj->Set(Nan::New("cache_size").ToLocalChecked(), Nan::New<Integer>(st.cache_size));
        obj->Set(Nan::New("read_cache_size").ToLocalChecked(), Nan::New<Integer>(st.read_cache_size));
        obj->Set(Nan::New("total_used_buffers").ToLocalChecked(), Nan::New<Integer>(st.total_used_buffers));

        obj->Set(Nan::New("average_queue_time").ToLocalChecked(), Nan::New<Integer>(st.average_queue_time));
        obj->Set(Nan::New("average_read_time").ToLocalChecked(), Nan::New<Integer>(st.average_read_time));
        obj->Set(Nan::New("average_write_time").ToLocalChecked(), Nan::New<Integer>(st.average_write_time));
        obj->Set(Nan::New("average_hash_time").ToLocalChecked(), Nan::New<Integer>(st.average_hash_time));
        obj->Set(Nan::New("average_job_time").ToLocalChecked(), Nan::New<Integer>(st.average_job_time));
        obj->Set(Nan::New("average_sort_time").ToLocalChecked(), Nan::New<Integer>(st.average_sort_time));
        obj->Set(Nan::New("job_queue_length").ToLocalChecked(), Nan::New<Integer>(st.job_queue_length));

        obj->Set(Nan::New("cumulative_job_time").ToLocalChecked(), Nan::New<Integer>(st.cumulative_job_time));
        obj->Set(Nan::New("cumulative_read_time").ToLocalChecked(), Nan::New<Integer>(st.cumulative_read_time));
        obj->Set(Nan::New("cumulative_write_time").ToLocalChecked(), Nan::New<Integer>(st.cumulative_write_time));
        obj->Set(Nan::New("cumulative_hash_time").ToLocalChecked(), Nan::New<Integer>(st.cumulative_hash_time));
        obj->Set(Nan::New("cumulative_sort_time").ToLocalChecked(), Nan::New<Integer>(st.cumulative_sort_time));
        obj->Set(Nan::New("total_read_back").ToLocalChecked(), Nan::New<Integer>(st.total_read_back));
        obj->Set(Nan::New("read_queue_size").ToLocalChecked(), Nan::New<Integer>(st.cache_size));

        info.GetReturnValue().Set(obj);
    };

    NAN_METHOD(SessionWrap::get_cache_info) {
        Nan::HandleScope scope;

        Local<Array> ret = Nan::New<Array>();

        libtorrent::sha1_hash info_hash;
        libtorrent::from_hex(*Nan::Utf8String(info[0]), 40, (char*)&info_hash[0]);
        std::vector<libtorrent::cached_piece_info> v;

        SessionWrap::Unwrap(info.This())->get_cache_info(info_hash, v);

        libtorrent::ptime now = libtorrent::time_now();

        for (std::vector<libtorrent::cached_piece_info>::iterator i(v.begin()), e(v.end()); i != e; ++i)
        {
            Local<Object> obj = Nan::New<Object>();
            Local<Array> blocks = Nan::New<Array>();

            obj->Set(Nan::New("piece").ToLocalChecked(), Nan::New<Integer>(i->piece));

            for (std::vector<bool>::const_iterator j(i->blocks.begin()), e(i->blocks.end()); j != e; ++j)
                blocks->Set(blocks->Length(), Nan::New<Boolean>(*j));

            obj->Set(Nan::New("blocks").ToLocalChecked(), blocks);
            obj->Set(Nan::New("last_use").ToLocalChecked(), Nan::New<Date>(libtorrent::total_milliseconds(now - i->last_use)).ToLocalChecked());
            obj->Set(Nan::New("next_to_hash").ToLocalChecked(), Nan::New<Integer>(i->next_to_hash));
            obj->Set(Nan::New("kind").ToLocalChecked(), Nan::New<Integer>(i->kind));

            ret->Set(ret->Length(), obj);
        }

        info.GetReturnValue().Set(ret);
    };

    NAN_METHOD(SessionWrap::add_feed) {
        Nan::HandleScope scope;

        libtorrent::feed_settings fs = feed_settings_from_object(info[0]->ToObject());
        libtorrent::feed_handle fh = SessionWrap::Unwrap(info.This())->add_feed(fs);

        info.GetReturnValue().Set(FeedHandleWrap::New(fh));
    };

    NAN_METHOD(SessionWrap::remove_feed) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->remove_feed(*FeedHandleWrap::Unwrap(info[0]->ToObject()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::get_feeds) {
        Nan::HandleScope scope;

        std::vector<libtorrent::feed_handle> feeds;

        SessionWrap::Unwrap(info.This())->get_feeds(feeds);

        Local<Array> ret = Nan::New<Array>();

        for (std::vector<libtorrent::feed_handle>::iterator i(feeds.begin()), e(feeds.end()); i != e; ++i)
            ret->Set(ret->Length(), FeedHandleWrap::New(*i));

        info.GetReturnValue().Set(ret);
    };

#ifndef TORRENT_DISABLE_DHT
    NAN_METHOD(SessionWrap::start_dht) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->start_dht();

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::stop_dht) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->stop_dht();

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::set_dht_settings) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->set_dht_settings(dht_settings_from_object(info[0]->ToObject()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::add_dht_node) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->add_dht_node(std::make_pair(
            std::string((*Nan::Utf8String(info[0]))), info[0]->IntegerValue()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::add_dht_router) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->add_dht_router(std::make_pair(
            std::string((*Nan::Utf8String(info[0]))), info[0]->IntegerValue()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::is_dht_running) {
        Nan::HandleScope scope;

        bool ret = SessionWrap::Unwrap(info.This())->is_dht_running();

        info.GetReturnValue().Set(Nan::New<Boolean>(ret));
    };
#endif

#ifndef TORRENT_DISABLE_EXTENSIONS
    NAN_METHOD(SessionWrap::add_extension) {
        Nan::HandleScope scope;

        libtorrent::session* s = SessionWrap::Unwrap(info.This());
        std::string name(*Nan::Utf8String(info[0]));

        if (name == "ut_metadata")
            s->add_extension(libtorrent::create_ut_metadata_plugin);
        else if (name == "ut_pex")
            s->add_extension(libtorrent::create_ut_pex_plugin);
        else if (name == "smart_ban")
            s->add_extension(libtorrent::create_smart_ban_plugin);
        else if (name == "lt_trackers")
            s->add_extension(libtorrent::create_lt_trackers_plugin);
        else if (name == "metadata_transfer")
            s->add_extension(libtorrent::create_metadata_plugin);

        info.GetReturnValue().SetUndefined();
    };
#endif

#ifndef TORRENT_DISABLE_GEO_IP
    NAN_METHOD(SessionWrap::load_asnum_db) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->load_asnum_db((std::string(*Nan::Utf8String(info[0]))).c_str());

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::load_country_db) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->load_country_db((std::string(*Nan::Utf8String(info[0]))).c_str());

        info.GetReturnValue().SetUndefined();
    };
#endif

    NAN_METHOD(SessionWrap::set_ip_filter) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->set_ip_filter(*IpFilterWrap::Unwrap(info[0]->ToObject()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::get_ip_filter) {
        Nan::HandleScope scope;

        libtorrent::ip_filter f = SessionWrap::Unwrap(info.This())->get_ip_filter();

        info.GetReturnValue().Set(IpFilterWrap::New(f));
    };

    NAN_METHOD(SessionWrap::set_peer_id) {
        Nan::HandleScope scope;

        libtorrent::peer_id pid;
        libtorrent::from_hex(*Nan::Utf8String(info[0]->ToString()), 40, (char*)&pid[0]);

        SessionWrap::Unwrap(info.This())->set_peer_id(pid);

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::id) {
        Nan::HandleScope scope;

        std::string ret(SessionWrap::Unwrap(info.This())->id().to_string());

        info.GetReturnValue().Set(Nan::New<String>(ret).ToLocalChecked());
    };

    NAN_METHOD(SessionWrap::is_listening) {
        Nan::HandleScope scope;

        libtorrent::session* s = SessionWrap::Unwrap(info.This());

        info.GetReturnValue().Set(Nan::New<Boolean>(s->is_listening()));
    };

    NAN_METHOD(SessionWrap::listen_port) {
        Nan::HandleScope scope;

        libtorrent::session* s = SessionWrap::Unwrap(info.This());

        info.GetReturnValue().Set(Nan::New<Integer>(s->listen_port()));
    };

    NAN_METHOD(SessionWrap::listen_on) {
        Nan::HandleScope scope;

        Local<Array> pr = info[0].As<Array>();

        std::pair<int, int> port_range = std::make_pair(pr->Get(0)->IntegerValue(), pr->Get(1)->IntegerValue());
        libtorrent::error_code ec;
        libtorrent::session* s = SessionWrap::Unwrap(info.This());

        if (info.Length() == 1) {
            s->listen_on(port_range, ec);
        } else {
            std::string interface(*Nan::Utf8String(info[1]->ToString()));

            if (info.Length() == 2) {
                s->listen_on(port_range, ec, interface.c_str());
            } else {
                s->listen_on(port_range, ec, interface.c_str(), info[2]->Int32Value());
            }
        }

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::remove_torrent) {
        Nan::HandleScope scope;

        libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info[0]->ToObject());

        if (info.Length() == 2)
            SessionWrap::Unwrap(info.This())->remove_torrent(*th, info[1]->IntegerValue());
        else
            SessionWrap::Unwrap(info.This())->remove_torrent(*th);

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::set_settings) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->set_settings(session_settings_from_object(info[0]->ToObject()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::settings) {
        Nan::HandleScope scope;

        libtorrent::session_settings s = SessionWrap::Unwrap(info.This())->settings();

        info.GetReturnValue().Set(session_settings_to_object(s));
    };

    NAN_METHOD(SessionWrap::set_proxy) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->set_proxy(proxy_settings_from_object(info[0]->ToObject()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::proxy) {
        Nan::HandleScope scope;

        libtorrent::proxy_settings s = SessionWrap::Unwrap(info.This())->proxy();

        info.GetReturnValue().Set(proxy_settings_to_object(s));
    };

#if TORRENT_USE_I2P
    NAN_METHOD(SessionWrap::set_i2p_proxy) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->set_i2p_proxy(proxy_settings_from_object(info[0]->ToObject()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::i2p_proxy) {
        Nan::HandleScope scope;

        libtorrent::proxy_settings s = SessionWrap::Unwrap(info.This())->i2p_proxy();

        info.GetReturnValue().Set(proxy_settings_to_object(s));
    };
#endif

    NAN_METHOD(SessionWrap::pop_alert) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(alert_to_object(*SessionWrap::Unwrap(info.This())->pop_alert()));
    };

    NAN_METHOD(SessionWrap::pop_alerts) {
        Nan::HandleScope scope;

        Local<Array> ret = Nan::New<Array>();

        std::deque<libtorrent::alert*> alerts;

        SessionWrap::Unwrap(info.This())->pop_alerts(&alerts);

        for (std::deque<libtorrent::alert*>::iterator i(alerts.begin()), e(alerts.end()); i != e; ++i)
            ret->Set(ret->Length(), alert_to_object(**i));

        info.GetReturnValue().Set(ret);
    };

    NAN_METHOD(SessionWrap::set_alert_mask) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->set_alert_mask(info[0]->Uint32Value());

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::wait_for_alert) {
        Nan::HandleScope scope;

        const libtorrent::alert* alert;

        alert = SessionWrap::Unwrap(info.This())->wait_for_alert(libtorrent::milliseconds(info[0]->IntegerValue()));

        info.GetReturnValue().Set(alert_to_object(*alert));
    };

    NAN_METHOD(SessionWrap::start_lsd) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->start_lsd();

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::start_natpmp) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->start_natpmp();

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::start_upnp) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->start_upnp();

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::stop_lsd) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->stop_lsd();

       info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::stop_natpmp) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->stop_natpmp();

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(SessionWrap::stop_upnp) {
        Nan::HandleScope scope;

        SessionWrap::Unwrap(info.This())->stop_upnp();

        info.GetReturnValue().SetUndefined();
    };
}; // namespace nodelt
