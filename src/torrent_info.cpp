#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/torrent_info.hpp>

#include "file_storage.hpp"
#include "torrent_info.hpp"

using namespace v8;
using namespace node;


namespace nodelt {
    Nan::Persistent<Function> TorrentInfoWrap::constructor;

    TorrentInfoWrap::TorrentInfoWrap() {
        obj_ = NULL;
    };

    TorrentInfoWrap::TorrentInfoWrap(const std::string& filename) {
        obj_ = new libtorrent::torrent_info(filename);
    };

    TorrentInfoWrap::~TorrentInfoWrap() {
        if (obj_ != NULL)
            delete obj_;
    };

    void bind_torrent_info(Local<Object> target) {
        TorrentInfoWrap::Initialize(target);

        Local<Object> tracker_source = Nan::New<Object>();
        tracker_source->Set(Nan::New("source_torrent").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::announce_entry::source_torrent));
        tracker_source->Set(Nan::New("source_client").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::announce_entry::source_client));
        tracker_source->Set(Nan::New("source_magnet_link").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::announce_entry::source_magnet_link));
        tracker_source->Set(Nan::New("source_tex").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::announce_entry::source_tex));
        target->Set(Nan::New("tracker_source").ToLocalChecked(), tracker_source);
    };

    Local<Object> web_seed_entry_headers_t_to_object(const libtorrent::web_seed_entry::headers_t& h) {
        Nan::EscapableHandleScope scope;

        Local<Array> ret = Nan::New<Array>();

        for (std::vector<std::pair<std::string, std::string> >::const_iterator i(h.begin()), e(h.end()); i != e; ++i) {
            Local<Array> obj = Nan::New<Array>();

            obj->Set(0, Nan::New<String>(i->first).ToLocalChecked());
            obj->Set(1, Nan::New<String>(i->second).ToLocalChecked());

            ret->Set(ret->Length(), obj);
        }

        return scope.Escape(ret);
    };

    libtorrent::web_seed_entry::headers_t web_seed_entry_headers_t_from_object(Local<Object> obj) {
        Nan::HandleScope scope;

        libtorrent::web_seed_entry::headers_t h;

        for (uint32_t i(0), e((obj.As<Array>())->Length()); i < e; ++i) {
            Local<Array> pair = (Nan::Get(obj, i).ToLocalChecked()).As<Array>();

            h.push_back(std::make_pair(
                std::string(*Nan::Utf8String(pair->Get(0))),
                std::string(*Nan::Utf8String(pair->Get(1)))
                        ));
        }

        return h;
    };

    Local<Object> announce_entry_to_object(const libtorrent::announce_entry& ae) {
        Nan::EscapableHandleScope scope;

        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("url").ToLocalChecked(), Nan::New<String>(ae.url.c_str()).ToLocalChecked());
        obj->Set(Nan::New("tier").ToLocalChecked(), Nan::New<Uint32>(ae.tier));
        obj->Set(Nan::New("fail_limit").ToLocalChecked(), Nan::New<Uint32>(ae.fail_limit));
        obj->Set(Nan::New("fails").ToLocalChecked(), Nan::New<Uint32>(ae.fails));
        obj->Set(Nan::New("source").ToLocalChecked(), Nan::New<Uint32>(ae.source));
        obj->Set(Nan::New("verified").ToLocalChecked(), Nan::New<Boolean>(ae.verified));
        obj->Set(Nan::New("updating").ToLocalChecked(), Nan::New<Boolean>(ae.updating));
        obj->Set(Nan::New("start_sent").ToLocalChecked(), Nan::New<Boolean>(ae.start_sent));
        obj->Set(Nan::New("complete_sent").ToLocalChecked(), Nan::New<Boolean>(ae.complete_sent));
        obj->Set(Nan::New("send_stats").ToLocalChecked(), Nan::New<Boolean>(ae.send_stats));

        return scope.Escape(obj);
    };

    libtorrent::announce_entry announce_entry_from_object(Local<Object> obj) {
        libtorrent::announce_entry ae(std::string(*Nan::Utf8String(
            Nan::Get(obj, Nan::New("url").ToLocalChecked()).ToLocalChecked())));

        if (Nan::Has(obj, Nan::New("tier").ToLocalChecked()) == Nan::Just(true))
            ae.tier = (Nan::Get(obj, Nan::New("tier").ToLocalChecked()).ToLocalChecked())->Uint32Value();
        if (Nan::Has(obj, Nan::New("fail_limit").ToLocalChecked()) == Nan::Just(true))
            ae.fail_limit = (Nan::Get(obj, Nan::New("fail_limit").ToLocalChecked()).ToLocalChecked())->Uint32Value();
        if (Nan::Has(obj, Nan::New("source").ToLocalChecked()) == Nan::Just(true))
            ae.source = (Nan::Get(obj, Nan::New("source").ToLocalChecked()).ToLocalChecked())->Uint32Value();
        if (Nan::Has(obj, Nan::New("verified").ToLocalChecked()) == Nan::Just(true))
            ae.verified = (Nan::Get(obj, Nan::New("verified").ToLocalChecked()).ToLocalChecked())->Uint32Value();
        if (Nan::Has(obj, Nan::New("send_stats").ToLocalChecked()) == Nan::Just(true))
            ae.send_stats = (Nan::Get(obj, Nan::New("send_stats").ToLocalChecked()).ToLocalChecked())->Uint32Value();

        return ae;
    };

    void TorrentInfoWrap::Initialize(Local<Object> target) {
        Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);

        tpl->SetClassName(Nan::New("torrent_info").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetPrototypeMethod(tpl, "remap_files", remap_files);
        Nan::SetPrototypeMethod(tpl, "add_tracker", add_tracker);
        Nan::SetPrototypeMethod(tpl, "add_url_seed", add_url_seed);
        Nan::SetPrototypeMethod(tpl, "add_http_seed", add_http_seed);
        Nan::SetPrototypeMethod(tpl, "web_seeds", web_seeds);

        Nan::SetPrototypeMethod(tpl, "name", name);
        Nan::SetPrototypeMethod(tpl, "comment", comment);
        Nan::SetPrototypeMethod(tpl, "creator", creator);
        Nan::SetPrototypeMethod(tpl, "total_size", total_size);
        Nan::SetPrototypeMethod(tpl, "piece_length", piece_length);
        Nan::SetPrototypeMethod(tpl, "num_pieces", num_pieces);
        Nan::SetPrototypeMethod(tpl, "info_hash", info_hash);

        Nan::SetPrototypeMethod(tpl, "hash_for_piece", hash_for_piece);
        Nan::SetPrototypeMethod(tpl, "merkle_tree", merkle_tree);
        Nan::SetPrototypeMethod(tpl, "set_merkle_tree", set_merkle_tree);
        Nan::SetPrototypeMethod(tpl, "piece_size", piece_size);

        Nan::SetPrototypeMethod(tpl, "num_files", num_files);
        Nan::SetPrototypeMethod(tpl, "file_at", file_at);
        Nan::SetPrototypeMethod(tpl, "files", files);
        Nan::SetPrototypeMethod(tpl, "orig_files", orig_files);
        Nan::SetPrototypeMethod(tpl, "rename_file", rename_file);

        Nan::SetPrototypeMethod(tpl, "priv", priv);
        Nan::SetPrototypeMethod(tpl, "trackers", trackers);

        Nan::SetPrototypeMethod(tpl, "creation_date", creation_date);

        Nan::SetPrototypeMethod(tpl, "add_node", add_node);
        Nan::SetPrototypeMethod(tpl, "nodes", nodes);
        Nan::SetPrototypeMethod(tpl, "metadata", metadata);
        Nan::SetPrototypeMethod(tpl, "metadata_size", metadata_size);
        Nan::SetPrototypeMethod(tpl, "map_block", map_block);
        Nan::SetPrototypeMethod(tpl, "map_file", map_file);

        constructor.Reset(tpl->GetFunction());

        target->Set(Nan::New("torrent_info").ToLocalChecked(), tpl->GetFunction());
    };

    Local<Object> TorrentInfoWrap::New(const libtorrent::torrent_info& ti) {
        Nan::EscapableHandleScope scope;

        Local<Function> c = Nan::New<Function>(constructor);
        Nan::MaybeLocal<Object> obj = c->NewInstance(Nan::GetCurrentContext());

        Nan::ObjectWrap::Unwrap<TorrentInfoWrap>(obj.ToLocalChecked())->obj_ = new libtorrent::torrent_info(ti);

        return scope.Escape(obj.ToLocalChecked());
    };

    NAN_METHOD(TorrentInfoWrap::NewInstance) {
        Nan::HandleScope scope;

        if (!info.IsConstructCall()) {
            Nan::ThrowTypeError("Use the new operator to create instances of this object.");
            return;
        }

        TorrentInfoWrap* ti;

        try {

            if (info.Length() == 1 && info[0]->IsString()) {
                std::string filename(*Nan::Utf8String(info[0]->ToString()));

                ti = new TorrentInfoWrap(filename);
            } else {
                ti = new TorrentInfoWrap();
            }

            ti->Wrap(info.This());
            info.GetReturnValue().Set(info.This());

        } catch(libtorrent::libtorrent_exception e) {
            Nan::ThrowError(e.what());
            info.GetReturnValue().SetUndefined();
        }
    };

    NAN_METHOD(TorrentInfoWrap::remap_files) {
        Nan::HandleScope scope;

        TorrentInfoWrap::Unwrap(info.This())->remap_files(*FileStorageWrap::Unwrap(info[0].As<Object>()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(TorrentInfoWrap::add_tracker) {
        Nan::HandleScope scope;

        TorrentInfoWrap::Unwrap(info.This())->add_tracker(std::string(*Nan::Utf8String(info[0])), info[1]->IntegerValue());

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(TorrentInfoWrap::add_url_seed) {
        Nan::HandleScope scope;
    
        libtorrent::torrent_info* ti = TorrentInfoWrap::Unwrap(info.This());
        std::string url(*Nan::Utf8String(info[0]));

        if (info.Length() > 1) {
            std::string extern_auth(*Nan::Utf8String(info[1]));

            if (info.Length() == 2)
                ti->add_url_seed(url, extern_auth);
            else
                ti->add_url_seed(url, extern_auth, web_seed_entry_headers_t_from_object(info[2].As<Object>()));
        } else {
            ti->add_url_seed(url);
        }

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(TorrentInfoWrap::add_http_seed) {
        Nan::HandleScope scope;
    
        libtorrent::torrent_info* ti = TorrentInfoWrap::Unwrap(info.This());
        std::string url(*Nan::Utf8String(info[0]));

        if (info.Length() > 1) {
            std::string extern_auth(*Nan::Utf8String(info[1]));

            if (info.Length() == 2)
                ti->add_http_seed(url, extern_auth);
            else
                ti->add_http_seed(url, extern_auth, web_seed_entry_headers_t_from_object(info[2].As<Object>()));
        } else {
            ti->add_http_seed(url);
        }

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(TorrentInfoWrap::web_seeds) {
        Nan::HandleScope scope;

        libtorrent::torrent_info* ti = TorrentInfoWrap::Unwrap(info.This());
        const std::vector<libtorrent::web_seed_entry> ws = ti->web_seeds();
        Local<Array> ret = Nan::New<Array>();

        for (std::vector<libtorrent::web_seed_entry>::const_iterator i(ws.begin()), e(ws.end()); i != e; ++i) {
            Local<Object> obj = Nan::New<Object>();

            obj->Set(Nan::New("url").ToLocalChecked(), Nan::New<String>(i->url.c_str()).ToLocalChecked());
            obj->Set(Nan::New("type").ToLocalChecked(), Nan::New<Integer>(i->type));
            obj->Set(Nan::New("auth").ToLocalChecked(), Nan::New<String>(i->auth.c_str()).ToLocalChecked());
            obj->Set(Nan::New("extra_headers").ToLocalChecked(), web_seed_entry_headers_t_to_object(i->extra_headers));

            int retry = libtorrent::total_seconds(i->retry - libtorrent::min_time());

            obj->Set(Nan::New("retry").ToLocalChecked(), Nan::New<Integer>(retry));
            obj->Set(Nan::New("resolving").ToLocalChecked(), Nan::New<Boolean>(i->resolving));
            obj->Set(Nan::New("removed").ToLocalChecked(), Nan::New<Boolean>(i->removed));

            Local<Array> endpoint = Nan::New<Array>();
            endpoint->Set(0, Nan::New<String>(i->endpoint.address().to_string().c_str()).ToLocalChecked());
            endpoint->Set(1, Nan::New<Integer>(i->endpoint.port()));

            obj->Set(Nan::New("endpoint").ToLocalChecked(), endpoint);
            ret->Set(ret->Length(), obj);
        }

        info.GetReturnValue().Set(ret);
    };

    NAN_METHOD(TorrentInfoWrap::name) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<String>(TorrentInfoWrap::Unwrap(info.This())->name().c_str()).ToLocalChecked());
    };

    NAN_METHOD(TorrentInfoWrap::comment) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<String>(TorrentInfoWrap::Unwrap(info.This())->comment().c_str()).ToLocalChecked());
    };

    NAN_METHOD(TorrentInfoWrap::creator) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<String>(TorrentInfoWrap::Unwrap(info.This())->creator().c_str()).ToLocalChecked());
    };

    NAN_METHOD(TorrentInfoWrap::total_size) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Number>(TorrentInfoWrap::Unwrap(info.This())->total_size()));
    };

    NAN_METHOD(TorrentInfoWrap::piece_length) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Integer>(TorrentInfoWrap::Unwrap(info.This())->piece_length()));
    };

    NAN_METHOD(TorrentInfoWrap::num_pieces) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Integer>(TorrentInfoWrap::Unwrap(info.This())->num_pieces()));
    };

    NAN_METHOD(TorrentInfoWrap::info_hash) {
        Nan::HandleScope scope;

        libtorrent::sha1_hash h(TorrentInfoWrap::Unwrap(info.This())->info_hash());

        info.GetReturnValue().Set(Nan::New<String>(libtorrent::to_hex(h.to_string()).c_str()).ToLocalChecked());
    };

    NAN_METHOD(TorrentInfoWrap::hash_for_piece) {
        Nan::HandleScope scope;

        libtorrent::sha1_hash hash = TorrentInfoWrap::Unwrap(info.This())->hash_for_piece(info[0]->IntegerValue());

        info.GetReturnValue().Set(Nan::New<String>(libtorrent::to_hex(hash.to_string()).c_str()).ToLocalChecked());
    };

    NAN_METHOD(TorrentInfoWrap::merkle_tree) {
        Nan::HandleScope scope;

        std::vector<libtorrent::sha1_hash> t = TorrentInfoWrap::Unwrap(info.This())->merkle_tree();
        Local<Array> ret = Nan::New<Array>();

        for (std::vector<libtorrent::sha1_hash>::iterator i(t.begin()), e(t.end()); i != e; ++i)
            ret->Set(ret->Length(), Nan::New<String>(libtorrent::to_hex(i->to_string()).c_str()).ToLocalChecked());

        info.GetReturnValue().Set(ret);
    };

    NAN_METHOD(TorrentInfoWrap::set_merkle_tree) {
        Nan::HandleScope scope;

        std::vector<libtorrent::sha1_hash> arg;
        Local<Array> t = Nan::New<Array>();

        for (uint32_t i(0), e(t->Length()); i < e; ++i) {
            libtorrent::sha1_hash h;
            libtorrent::from_hex(*Nan::Utf8String(t->Get(0)), 40, (char*)&h[0]);
            arg.push_back(h);
        }

        TorrentInfoWrap::Unwrap(info.This())->set_merkle_tree(arg);

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(TorrentInfoWrap::piece_size) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Integer>(TorrentInfoWrap::Unwrap(info.This())->piece_size(info[0]->IntegerValue())));
    };


    NAN_METHOD(TorrentInfoWrap::num_files) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Integer>(TorrentInfoWrap::Unwrap(info.This())->num_files()));
    };

    NAN_METHOD(TorrentInfoWrap::file_at) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(file_entry_to_object(TorrentInfoWrap::Unwrap(info.This())->file_at(info[0]->IntegerValue())));
    };

    NAN_METHOD(TorrentInfoWrap::files) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(FileStorageWrap::New(TorrentInfoWrap::Unwrap(info.This())->files()));
    };

    NAN_METHOD(TorrentInfoWrap::orig_files) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(FileStorageWrap::New(TorrentInfoWrap::Unwrap(info.This())->orig_files()));
    };

    NAN_METHOD(TorrentInfoWrap::rename_file) {
        Nan::HandleScope scope;

        TorrentInfoWrap::Unwrap(info.This())->rename_file(info[0]->IntegerValue(), std::string(*Nan::Utf8String(info[1])));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(TorrentInfoWrap::priv) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Boolean>(TorrentInfoWrap::Unwrap(info.This())->priv()));
    };

    NAN_METHOD(TorrentInfoWrap::trackers) {
        Nan::HandleScope scope;

        std::vector<libtorrent::announce_entry> t = TorrentInfoWrap::Unwrap(info.This())->trackers();
        Local<Array> ret = Nan::New<Array>();

        for (std::vector<libtorrent::announce_entry>::iterator i(t.begin()), e(t.end()); i != e; ++i)
            ret->Set(ret->Length(), announce_entry_to_object(*i));

        info.GetReturnValue().Set(ret);
    };

    NAN_METHOD(TorrentInfoWrap::creation_date) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Date>(((double) *TorrentInfoWrap::Unwrap(info.This())->creation_date())*1000).ToLocalChecked());
    };

    NAN_METHOD(TorrentInfoWrap::add_node) {
        Nan::HandleScope scope;

        TorrentInfoWrap::Unwrap(info.This())->add_node(std::make_pair(
            std::string(*Nan::Utf8String(info[0])),
            info[1]->IntegerValue()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(TorrentInfoWrap::nodes) {
        Nan::HandleScope scope;

        std::vector<std::pair<std::string, int> > ns = TorrentInfoWrap::Unwrap(info.This())->nodes();
        Local<Array> ret = Nan::New<Array>();

        for (std::vector<std::pair<std::string, int> >::iterator i(ns.begin()), e(ns.end()); i != e; ++i) {
            Local<Array> obj = Nan::New<Array>();

            obj->Set(0, Nan::New<String>(i->first.c_str()).ToLocalChecked());
            obj->Set(1, Nan::New<Integer>(i->second));
            ret->Set(ret->Length(), obj);
        }

        info.GetReturnValue().Set(ret);
    };

    NAN_METHOD(TorrentInfoWrap::metadata) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<String>(&TorrentInfoWrap::Unwrap(info.This())->metadata()[0]).ToLocalChecked());
    };

    NAN_METHOD(TorrentInfoWrap::metadata_size) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Integer>(TorrentInfoWrap::Unwrap(info.This())->metadata_size()));
    };

    NAN_METHOD(TorrentInfoWrap::map_block) {
        Nan::HandleScope scope;

        std::vector<libtorrent::file_slice> res = TorrentInfoWrap::Unwrap(info.This())->map_block(info[0]->IntegerValue(),
            info[1]->IntegerValue(), info[2]->IntegerValue());

        Local<Array> ret = Nan::New<Array>();

        for (std::vector<libtorrent::file_slice>::iterator i(res.begin()), e(res.end()); i != e; ++i)
            ret->Set(ret->Length(), file_slice_to_object(*i));

        info.GetReturnValue().Set(ret);
    };

    NAN_METHOD(TorrentInfoWrap::map_file) {
        Nan::HandleScope scope;
    
        libtorrent::peer_request res = TorrentInfoWrap::Unwrap(info.This())->map_file(info[0]->IntegerValue(),
            info[1]->IntegerValue(), info[2]->IntegerValue());

        Local<Object> ret = Nan::New<Object>();

        ret->Set(Nan::New("piece").ToLocalChecked(), Nan::New<Integer>(res.piece));
        ret->Set(Nan::New("start").ToLocalChecked(), Nan::New<Integer>(res.start));
        ret->Set(Nan::New("length").ToLocalChecked(), Nan::New<Integer>(res.length));

        info.GetReturnValue().Set(ret);
    };
}; // namespace nodelt
