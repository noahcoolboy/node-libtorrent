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

    if (Nan::Has(obj, Nan::New("ti").ToLocalChecked()) == Nan::Just(true)) {
      Nan::MaybeLocal<Object> ti = Nan::Get(obj, Nan::New("ti").ToLocalChecked()); // to object
      p.ti = new libtorrent::torrent_info(*TorrentInfoWrap::Unwrap(ti));
    }
    if (Nan::Has(obj, Nan::New("trackers").ToLocalChecked()) == Nan::Just(true)) {
      Local<Array> trackers = Array::Cast(*obj->Get(Nan::New("trackers")));
      for (uint32_t i = 0, e = trackers->Length(); i < e; ++i)
        p.trackers.push_back(*String::Utf8Value(trackers->Get(i)));
    }
    if (Nan::Has(obj, Nan::New("dht_nodes").ToLocalChecked()) == Nan::Just(true)) {
      Local<Array> dht_nodes = Array::Cast(*obj->Get(Nan::New("dht_nodes")));
      for (uint32_t i = 0, e = dht_nodes->Length(); i < e; ++i) {
        Local<Array> node = Array::Cast(*dht_nodes->Get(i));
        p.dht_nodes.push_back(std::make_pair(
          *String::Utf8Value(node->Get(0)),
          node->Get(1)->Int32Value()));
      }
    }/*
    if (obj->Has(Nan::New("info_hash"))) {
      String::AsciiValue info_hash(obj->Get(Nan::New("info_hash"))->ToString());
      libtorrent::from_hex(*info_hash, 40, (char*)&p.info_hash[0]);
    }
    if (obj->Has(Nan::New("name")))
      p.name = std::string(*String::Utf8Value(obj->Get(Nan::New("name"))));
    p.save_path = std::string(*String::Utf8Value(obj->Get(Nan::New("save_path"))));
    if (obj->Has(Nan::New("resume_data"))) {
      String::AsciiValue data(obj->Get(Nan::New("resume_data"))->ToString());
      std::vector<char> rd(data.length());
      std::memcpy(&rd[0], *data, rd.size());
      p.resume_data = rd;
    }
    if (obj->Has(Nan::New("storage_mode")))
      p.storage_mode = (libtorrent::storage_mode_t) obj->Get(Nan::New("storage_mode"))->Int32Value();
    if (obj->Has(Nan::New("file_priorities"))) {
      Local<Array> data = Array::Cast(*obj->Get(Nan::New("file_priorities")));
      std::vector<boost::uint8_t> fp;
      for (uint32_t i = 0, e = data->Length(); i < e; ++i)
        fp.push_back(data->Get(i)->Int32Value());
      p.file_priorities = fp;
    }
    if (obj->Has(Nan::New("trackerid")))
      p.trackerid = std::string(*String::Utf8Value(obj->Get(Nan::New("trackerid"))));
    if (obj->Has(Nan::New("url")))
      p.url = std::string(*String::Utf8Value(obj->Get(Nan::New("url"))));
    if (obj->Has(Nan::New("uuid")))
      p.uuid = std::string(*String::Utf8Value(obj->Get(Nan::New("uuid"))));
    if (obj->Has(Nan::New("source_feed_url")))
      p.source_feed_url = std::string(*String::Utf8Value(obj->Get(Nan::New("source_feed_url"))));
    if (obj->Has(Nan::New("flags")))
      p.flags = obj->Get(Nan::New("flags"))->IntegerValue();
*/
    return p;
  }

  void bind_add_torrent_params(Handle<Object> target) {
    // add_torrent_params::flags_t
    /*
    Local<Object> flags_t = Object::New();
    flags_t->Set(Nan::New("flag_seed_mode"),
      Nan::New<Integer>(libtorrent::add_torrent_params::flag_seed_mode));
    flags_t->Set(Nan::New("flag_override_resume_data"),
      Nan::New<Integer>(libtorrent::add_torrent_params::flag_override_resume_data));
    flags_t->Set(Nan::New("flag_upload_mode"),
      Nan::New<Integer>(libtorrent::add_torrent_params::flag_upload_mode));
    flags_t->Set(Nan::New("flag_share_mode"),
      Nan::New<Integer>(libtorrent::add_torrent_params::flag_share_mode));
    flags_t->Set(Nan::New("flag_apply_ip_filter"),
      Nan::New<Integer>(libtorrent::add_torrent_params::flag_apply_ip_filter));
    flags_t->Set(Nan::New("flag_paused"),
      Nan::New<Integer>(libtorrent::add_torrent_params::flag_paused));
    flags_t->Set(Nan::New("flag_auto_managed"),
      Nan::New<Integer>(libtorrent::add_torrent_params::flag_auto_managed));
    flags_t->Set(Nan::New("flag_duplicate_is_error"),
      Nan::New<Integer>(libtorrent::add_torrent_params::flag_duplicate_is_error));
    flags_t->Set(Nan::New("flag_merge_resume_trackers"),
      Nan::New<Integer>(libtorrent::add_torrent_params::flag_merge_resume_trackers));
    flags_t->Set(Nan::New("flag_update_subscribe"),
      Nan::New<Integer>(libtorrent::add_torrent_params::flag_update_subscribe));
    flags_t->Set(Nan::New("default_flags"),
      Nan::New<Integer>(libtorrent::add_torrent_params::default_flags));
    target->Set(Nan::New("add_torrent_params_flags_t"), flags_t);
  */};
}; // namespace nodelt
