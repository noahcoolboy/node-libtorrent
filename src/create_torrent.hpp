#ifndef NODE_LIBTORRENT_CREATE_TORRENT_HPP_INCLUDED
#define NODE_LIBTORRENT_CREATE_TORRENT_HPP_INCLUDED

#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/create_torrent.hpp>

using namespace v8;

namespace nodelt {
    void bind_create_torrent(Local<Object> target);

    class CreateTorrentWrap: public Nan::ObjectWrap {
        public:
            static void Initialize(Local<Object> target);
            static libtorrent::create_torrent* Unwrap(const Local<Object>& obj) {
                return Nan::ObjectWrap::Unwrap<CreateTorrentWrap>(obj)->obj_;
            };

        private:
            libtorrent::create_torrent* obj_;
            CreateTorrentWrap(libtorrent::file_storage& fs);
            ~CreateTorrentWrap();

            static NAN_METHOD(NewInstance);
            static NAN_METHOD(generate);
            static NAN_METHOD(files);
            static NAN_METHOD(set_comment);
            static NAN_METHOD(set_creator);
            static NAN_METHOD(set_hash);
            static NAN_METHOD(set_file_hash);
            static NAN_METHOD(add_url_seed);
            static NAN_METHOD(add_http_seed);
            static NAN_METHOD(add_node);
            static NAN_METHOD(add_tracker);
            static NAN_METHOD(set_priv);
            static NAN_METHOD(num_pieces);
            static NAN_METHOD(piece_length);
            static NAN_METHOD(piece_size);
            static NAN_METHOD(priv);
            static NAN_METHOD(set_root_cert);
            static NAN_METHOD(set_piece_hashes);
            static NAN_METHOD(add_files);

            static Nan::Persistent<Function> constructor;
    };
};

#endif // NODE_LIBTORRENT_CREATE_TORRENT_HPP_INCLUDED
