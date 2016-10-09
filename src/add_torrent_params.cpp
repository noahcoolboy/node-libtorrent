#include <node.h>
#include <nan.h>
#include <v8.h>

#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_info.hpp>

#include "add_torrent_params.hpp"
#include "torrent_info.hpp"

using namespace v8;
using namespace node;


namespace nodelt {
    libtorrent::add_torrent_params add_torrent_params_from_object(Local<Object> obj) {
        Nan::HandleScope scope;
        libtorrent::add_torrent_params p;

        try {
            if (Nan::Has(obj, Nan::New("ti").ToLocalChecked()) == Nan::Just(true)) {
                Nan::MaybeLocal<Value> ti = Nan::Get(obj, Nan::New("ti").ToLocalChecked());
                p.ti = new libtorrent::torrent_info(*TorrentInfoWrap::Unwrap(ti.ToLocalChecked().As<Object>()));
            }

            if (Nan::Has(obj, Nan::New("trackers").ToLocalChecked()) == Nan::Just(true)) {
                Local<Array> trackers = (Nan::Get(obj, Nan::New("trackers") .ToLocalChecked()).ToLocalChecked()).As<Array>();

                for (uint32_t i = 0, e = trackers->Length(); i < e; ++i)
                    p.trackers.push_back(*String::Utf8Value(trackers->Get(i)));
            }

            if (Nan::Has(obj, Nan::New("dht_nodes").ToLocalChecked()) == Nan::Just(true)) {
                Local<Array> dht_nodes = (Nan::Get(obj, Nan::New("dht_nodes").ToLocalChecked()).ToLocalChecked()).As<Array>();

                for (uint32_t i = 0, e = dht_nodes->Length(); i < e; ++i) {
                    Local<Array> node = dht_nodes->Get(i).As<Array>();
                    p.dht_nodes.push_back(std::make_pair(
                        *Nan::Utf8String(node->Get(0)),
                        node->Get(1)->Int32Value() ));
                }
            }

            if (Nan::Has(obj, Nan::New("info_hash").ToLocalChecked()) == Nan::Just(true)) {
                Nan::Utf8String info_hash (Nan::Get(obj, Nan::New("info_hash").ToLocalChecked()).ToLocalChecked());
                libtorrent::from_hex(*info_hash, 40, (char*)&p.info_hash[0]);
            }

            if (Nan::Has(obj, Nan::New("name").ToLocalChecked()) == Nan::Just(true))
                p.name = std::string(*Nan::Utf8String(Nan::Get(obj, Nan::New("name").ToLocalChecked()).ToLocalChecked()));

            p.save_path = std::string(*Nan::Utf8String(Nan::Get(obj, Nan::New("save_path").ToLocalChecked()).ToLocalChecked()));

            if (Nan::Has(obj, Nan::New("resume_data").ToLocalChecked()) == Nan::Just(true)) {
                Nan::Utf8String data(Nan::Get(obj, Nan::New("resume_data").ToLocalChecked()).ToLocalChecked());
                std::vector<char> rd(data.length());
                std::memcpy(&rd[0], *data, rd.size());
                p.resume_data = rd;
            }

            if (Nan::Has(obj, Nan::New("storage_mode").ToLocalChecked()) == Nan::Just(true))
                p.storage_mode = (libtorrent::storage_mode_t)
                    (Nan::Get(obj, Nan::New("storage_mode").ToLocalChecked()).ToLocalChecked())->Int32Value();

            if (Nan::Has(obj, Nan::New("file_priorities").ToLocalChecked()) == Nan::Just(true)) {
                Local<Array> data = (Nan::Get(obj, Nan::New("file_priorities").ToLocalChecked()).ToLocalChecked()).As<Array>();
                std::vector<boost::uint8_t> fp;

                for (uint32_t i = 0, e = data->Length(); i < e; ++i)
                    fp.push_back(data->Get(i)->Int32Value());

                p.file_priorities = fp;
            }

            if (Nan::Has(obj, Nan::New("trackerid").ToLocalChecked()) == Nan::Just(true))
                p.trackerid = std::string(*Nan::Utf8String(Nan::Get(obj, Nan::New("trackerid").ToLocalChecked()).ToLocalChecked()));

            if (Nan::Has(obj, Nan::New("url").ToLocalChecked()) == Nan::Just(true))
                p.url = std::string(*Nan::Utf8String(Nan::Get(obj, Nan::New("url").ToLocalChecked()).ToLocalChecked()));

            if (Nan::Has(obj, Nan::New("uuid").ToLocalChecked()) == Nan::Just(true))
                p.uuid = std::string(*Nan::Utf8String(Nan::Get(obj, Nan::New("uuid").ToLocalChecked()).ToLocalChecked()));

            if (Nan::Has(obj, Nan::New("source_feed_url").ToLocalChecked()) == Nan::Just(true))
                p.source_feed_url = std::string(*Nan::Utf8String(Nan::Get(obj, Nan::New("source_feed_url").ToLocalChecked()).ToLocalChecked()));

            if (Nan::Has(obj, Nan::New("flags").ToLocalChecked()) == Nan::Just(true))
                p.flags = (Nan::Get(obj, Nan::New("flags").ToLocalChecked()).ToLocalChecked())->IntegerValue();

        } catch(libtorrent::libtorrent_exception e) {
            Nan::ThrowError(e.what());
        }

        return p;
    }

    void bind_add_torrent_params(Local<Object> target) {
        // add_torrent_params::flags_t
        Local<Object> flags_t = Nan::New<Object>();

        flags_t->Set(Nan::New("flag_seed_mode").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_params::flag_seed_mode));
        flags_t->Set(Nan::New("flag_override_resume_data").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_params::flag_override_resume_data));
        flags_t->Set(Nan::New("flag_upload_mode").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_params::flag_upload_mode));
        flags_t->Set(Nan::New("flag_share_mode").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_params::flag_share_mode));
        flags_t->Set(Nan::New("flag_apply_ip_filter").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_params::flag_apply_ip_filter));
        flags_t->Set(Nan::New("flag_paused").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_params::flag_paused));
        flags_t->Set(Nan::New("flag_auto_managed").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_params::flag_auto_managed));
        flags_t->Set(Nan::New("flag_duplicate_is_error").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_params::flag_duplicate_is_error));
        flags_t->Set(Nan::New("flag_merge_resume_trackers").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_params::flag_merge_resume_trackers));
        flags_t->Set(Nan::New("flag_update_subscribe").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_params::flag_update_subscribe));
        flags_t->Set(Nan::New("default_flags").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_params::default_flags));
        target->Set(Nan::New("add_torrent_params_flags_t").ToLocalChecked(), flags_t);
    };
}; // namespace nodelt
