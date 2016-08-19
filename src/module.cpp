#include <node.h>
#include <nan.h>
#include <v8.h>

#include <libtorrent/version.hpp>

using namespace v8;

namespace nodelt {
    void bind_add_torrent_params(Local<Object> target);
    void bind_alert(Local<Object> target);
    void bind_bencode(Local<Object> target);
    void bind_create_torrent(Local<Object> target);
    void bind_file_storage(Local<Object> target);
    void bind_fingerprint(Local<Object> target);
    void bind_ip_filter(Local<Object> target);
    void bind_peer_info(Local<Object> target);
    void bind_session(Local<Object> target);
    void bind_session_settings(Local<Object> target);
    void bind_storage(Local<Object> target);
    void bind_torrent_handle(Local<Object> target);
    void bind_torrent_info(Local<Object> target);
    void bind_torrent_status(Local<Object> target);

    static NAN_MODULE_INIT(Initialize) {
        bind_add_torrent_params(target);
        bind_alert(target);
        bind_bencode(target);
        bind_create_torrent(target);
        bind_file_storage(target);
        bind_fingerprint(target);
        bind_ip_filter(target);
        bind_peer_info(target);
        bind_session(target);
        bind_session_settings(target);
        bind_storage(target);
        bind_torrent_handle(target);
        bind_torrent_info(target);
        bind_torrent_status(target);

        target->Set(Nan::New("version").ToLocalChecked(),       Nan::New(LIBTORRENT_VERSION).ToLocalChecked());
        target->Set(Nan::New("version_major").ToLocalChecked(), Nan::New<Integer>(LIBTORRENT_VERSION_MAJOR));
        target->Set(Nan::New("version_minor").ToLocalChecked(), Nan::New<Integer>(LIBTORRENT_VERSION_MINOR));
    };
}; // namespace nodelt

NODE_MODULE(libtorrent, nodelt::Initialize);
