#include <v8.h>
#include <nan.h>
#include <node.h>

#include <string>

#include <libtorrent/create_torrent.hpp>
#include <libtorrent/file_storage.hpp>

#include "create_torrent.hpp"
#include "entry.hpp"
#include "file_storage.hpp"

using namespace v8;
using namespace node;


namespace nodelt {

    Nan::Persistent<Function> CreateTorrentWrap::constructor;

    CreateTorrentWrap::CreateTorrentWrap(libtorrent::file_storage& fs_) {
        obj_ = new libtorrent::create_torrent(fs_);
    };

    CreateTorrentWrap::~CreateTorrentWrap() {
        delete obj_;
    };

    void CreateTorrentWrap::Initialize(Local<Object> target) {
        Nan::HandleScope scope;

        Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);

        tpl->SetClassName(Nan::New("create_torrent").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetPrototypeMethod(tpl, "generate", generate);
        Nan::SetPrototypeMethod(tpl, "files", files);
        Nan::SetPrototypeMethod(tpl, "set_comment", set_comment);
        Nan::SetPrototypeMethod(tpl, "set_creator", set_creator);
        Nan::SetPrototypeMethod(tpl, "set_hash", set_hash);
        Nan::SetPrototypeMethod(tpl, "set_file_hash", set_file_hash);
        Nan::SetPrototypeMethod(tpl, "add_url_seed", add_url_seed);
        Nan::SetPrototypeMethod(tpl, "add_http_seed", add_http_seed);
        Nan::SetPrototypeMethod(tpl, "add_node", add_node);
        Nan::SetPrototypeMethod(tpl, "add_tracker", add_tracker);
        Nan::SetPrototypeMethod(tpl, "set_priv", set_priv);
        Nan::SetPrototypeMethod(tpl, "num_pieces", num_pieces);
        Nan::SetPrototypeMethod(tpl, "piece_length", piece_length);
        Nan::SetPrototypeMethod(tpl, "piece_size", piece_size);
        Nan::SetPrototypeMethod(tpl, "priv", priv);
        Nan::SetPrototypeMethod(tpl, "set_root_cert", set_root_cert);

        constructor.Reset(tpl->GetFunction());

        target->Set(Nan::New("create_torrent").ToLocalChecked(), tpl->GetFunction());
    };

    void bind_create_torrent(Local<Object> target) {
        CreateTorrentWrap::Initialize(target);

        Local<Object> create_torrent_flags_t = Nan::New<Object>();

        create_torrent_flags_t->Set(Nan::New("optimize").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::create_torrent::optimize));
        create_torrent_flags_t->Set(Nan::New("merkle").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::create_torrent::merkle));
        create_torrent_flags_t->Set(Nan::New("modification_time").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::create_torrent::modification_time));
        create_torrent_flags_t->Set(Nan::New("symlinks").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::create_torrent::symlinks));
        create_torrent_flags_t->Set(Nan::New("calculate_file_hashes").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::create_torrent::calculate_file_hashes));

        target->Set(Nan::New("create_torrent_flags_t").ToLocalChecked(), create_torrent_flags_t);

        //Nan::SetMethod(target, "add_files", add_files);
        //Nan::SetMethod(target, "set_piece_hashes", set_piece_hashes);
    };

    NAN_METHOD(CreateTorrentWrap::NewInstance) {
        Nan::HandleScope scope;

        if (!info.IsConstructCall()) {
            Nan::ThrowTypeError("Use the new operator to create instances of this object.");
            return;
        }

        CreateTorrentWrap* ct = new CreateTorrentWrap(*FileStorageWrap::Unwrap(info[0]->ToObject()));

        ct->Wrap(info.This());

        info.GetReturnValue().Set(info.This());
    };

    NAN_METHOD(CreateTorrentWrap::generate) {
        Nan::HandleScope scope;

        libtorrent::entry e = CreateTorrentWrap::Unwrap(info.This())->generate();

        info.GetReturnValue().Set(entry_to_object(e));
    };

    NAN_METHOD(CreateTorrentWrap::files) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(FileStorageWrap::New(CreateTorrentWrap::Unwrap(info.This())->files()));
    };

    NAN_METHOD(CreateTorrentWrap::set_comment) {
        Nan::HandleScope scope;

        CreateTorrentWrap::Unwrap(info.This())->set_comment(*Nan::Utf8String(info[0]));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(CreateTorrentWrap::set_creator) {
        Nan::HandleScope scope;

        CreateTorrentWrap::Unwrap(info.This())->set_creator(*Nan::Utf8String(info[0]));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(CreateTorrentWrap::set_hash) {
        Nan::HandleScope scope;

        libtorrent::sha1_hash h;
        libtorrent::from_hex(*Nan::Utf8String(info[1]), 40, (char*)&h[0]);

        CreateTorrentWrap::Unwrap(info.This())->set_hash(info[0]->IntegerValue(), h);

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(CreateTorrentWrap::set_file_hash) {
        Nan::HandleScope scope;

        libtorrent::sha1_hash h;
        libtorrent::from_hex(*Nan::Utf8String(info[1]), 40, (char*)&h[0]);

        CreateTorrentWrap::Unwrap(info.This())->set_file_hash(info[0]->IntegerValue(), h);

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(CreateTorrentWrap::add_url_seed) {
        Nan::HandleScope scope;

        CreateTorrentWrap::Unwrap(info.This())->add_url_seed(std::string(*Nan::Utf8String(info[0])));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(CreateTorrentWrap::add_http_seed) {
        Nan::HandleScope scope;

        CreateTorrentWrap::Unwrap(info.This())->add_http_seed(std::string(*Nan::Utf8String(info[0])));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(CreateTorrentWrap::add_node) {
        Nan::HandleScope scope;

        CreateTorrentWrap::Unwrap(info.This())->add_node(std::make_pair(
            std::string(*Nan::Utf8String(info[0])),
            info[1]->IntegerValue()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(CreateTorrentWrap::add_tracker) {
        Nan::HandleScope scope;

        libtorrent::create_torrent* ct = CreateTorrentWrap::Unwrap(info.This());

        std::string url(*Nan::Utf8String(info[0]));

        if (info.Length() == 2)
            ct->add_tracker(url, info[1]->IntegerValue());
        else
            ct->add_tracker(url);

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(CreateTorrentWrap::set_priv) {
        Nan::HandleScope scope;

        CreateTorrentWrap::Unwrap(info.This())->set_priv(info[0]->BooleanValue());

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(CreateTorrentWrap::num_pieces) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Integer>(CreateTorrentWrap::Unwrap(info.This())->num_pieces()));
    };

    NAN_METHOD(CreateTorrentWrap::piece_length) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Integer>(CreateTorrentWrap::Unwrap(info.This())->piece_length()));
    };

    NAN_METHOD(CreateTorrentWrap::piece_size) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Integer>(CreateTorrentWrap::Unwrap(info.This())->piece_size(info[0]->IntegerValue())));
    };

    NAN_METHOD(CreateTorrentWrap::priv) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Boolean>(CreateTorrentWrap::Unwrap(info.This())->priv()));
    };

    NAN_METHOD(CreateTorrentWrap::set_root_cert) {
        Nan::HandleScope scope;

        CreateTorrentWrap::Unwrap(info.This())->set_root_cert(std::string(*Nan::Utf8String(info[0])));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(CreateTorrentWrap::add_files) {
        Nan::HandleScope scope;

        libtorrent::file_storage* fs = FileStorageWrap::Unwrap(info[0]->ToObject());
        std::string path(*Nan::Utf8String(info[1]->ToString()));

        if (info.Length() == 2)
            libtorrent::add_files(*fs, path);
        else
            libtorrent::add_files(*fs, path, Nan::To<uint32_t>(info[2]).FromJust());

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(CreateTorrentWrap::set_piece_hashes) {
        Nan::HandleScope scope;

        libtorrent::create_torrent* ct = CreateTorrentWrap::Unwrap(info[0]->ToObject());
        std::string path(*Nan::Utf8String(info[1]->ToString()));
        libtorrent::error_code ec;
        libtorrent::set_piece_hashes(*ct, path, ec);

        info.GetReturnValue().SetUndefined();
    };
}; // namespace nodelt
