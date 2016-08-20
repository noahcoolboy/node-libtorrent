#ifndef NODE_LIBTORRENT_FILE_STORAGE_HPP_INCLUDED
#define NODE_LIBTORRENT_FILE_STORAGE_HPP_INCLUDED

#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/file_storage.hpp>

using namespace v8;

namespace nodelt {
    Local<Object> file_entry_to_object(const libtorrent::file_entry& e);
    Local<Object> file_slice_to_object(const libtorrent::file_slice& fs);
    libtorrent::file_entry file_entry_from_object(Local<Object> obj);

    class FileStorageWrap: public Nan::ObjectWrap {
        public:
            static void Initialize(Local<Object> target);
            static Local<Object> New(const libtorrent::file_storage& fs);
            static libtorrent::file_storage* Unwrap(const Local<Object>& obj) {
                return Nan::ObjectWrap::Unwrap<FileStorageWrap>(obj)->obj_;
            };

        private:
            libtorrent::file_storage* obj_;
            FileStorageWrap();
            ~FileStorageWrap();

            static Nan::Persistent<Function> constructor;

            static NAN_METHOD(NewInstance);

            static NAN_METHOD(is_valid);
            static NAN_METHOD(add_file);
            static NAN_METHOD(num_files);
            static NAN_METHOD(at);
            static NAN_METHOD(total_size);
            static NAN_METHOD(set_num_pieces);
            static NAN_METHOD(num_pieces);
            static NAN_METHOD(set_piece_length);
            static NAN_METHOD(piece_length);
            static NAN_METHOD(piece_size);
            static NAN_METHOD(set_name);
            static NAN_METHOD(rename_file);
            static NAN_METHOD(name);
    };
};

#endif // NODE_LIBTORRENT_FILE_STORAGE_HPP_INCLUDED
