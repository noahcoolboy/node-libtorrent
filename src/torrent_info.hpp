#ifndef NODE_LIBTORRENT_TORRENT_INFO_HPP_INCLUDED
#define NODE_LIBTORRENT_TORRENT_INFO_HPP_INCLUDED

#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/torrent_info.hpp>


namespace nodelt {
    class TorrentInfoWrap: public node::ObjectWrap {
        public:
            static void Initialize(v8::Handle<v8::Object> target);
            static v8::Local<v8::Object> New(const libtorrent::torrent_info& ti);

            static libtorrent::torrent_info* Unwrap(const v8::Local<v8::Object>& obj) {
                return Nan::ObjectWrap::Unwrap<TorrentInfoWrap>(obj)->obj_;
            };

        private:
            libtorrent::torrent_info* obj_;

            TorrentInfoWrap();
            TorrentInfoWrap(const std::string& filename);
            ~TorrentInfoWrap();

            static v8::Persistent<v8::Function> constructor;
            static NAN_METHOD(NewInstance);

            static NAN_METHOD(remap_files);
            static NAN_METHOD(add_tracker);
            static NAN_METHOD(add_url_seed);
            static NAN_METHOD(add_http_seed);
            static NAN_METHOD(web_seeds);

            static NAN_METHOD(name);
            static NAN_METHOD(comment);
            static NAN_METHOD(creator);
            static NAN_METHOD(total_size);
            static NAN_METHOD(piece_length);
            static NAN_METHOD(num_pieces);
            static NAN_METHOD(info_hash);

            static NAN_METHOD(hash_for_piece);
            static NAN_METHOD(merkle_tree);
            static NAN_METHOD(set_merkle_tree);
            static NAN_METHOD(piece_size);

            static NAN_METHOD(num_files);
            static NAN_METHOD(file_at);
            static NAN_METHOD(files);
            static NAN_METHOD(orig_files);
            static NAN_METHOD(rename_file);

            static NAN_METHOD(priv);
            static NAN_METHOD(trackers);

            static NAN_METHOD(creation_date);

            static NAN_METHOD(add_node);
            static NAN_METHOD(nodes);
            static NAN_METHOD(metadata);
            static NAN_METHOD(metadata_size);
            static NAN_METHOD(map_block);
            static NAN_METHOD(map_file);
    };

    v8::Local<v8::Object> announce_entry_to_object(const libtorrent::announce_entry& ae);
    libtorrent::announce_entry announce_entry_from_object(v8::Local<v8::Object> obj);
};

#endif // NODE_LIBTORRENT_TORRENT_INFO_HPP_INCLUDED
