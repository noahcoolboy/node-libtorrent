#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/storage.hpp>

#include "storage.hpp"

using namespace v8;
using namespace node;

namespace nodelt {
    void bind_storage(Local<Object> target) {
        // set libtorrent::storage_mode_t
        Local<Object> storage_mode_t = Nan::New<Object>();

        storage_mode_t->Set(Nan::New("storage_mode_allocate").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::storage_mode_allocate));
        storage_mode_t->Set(Nan::New("storage_mode_sparse").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::storage_mode_sparse));

        target->Set(Nan::New("storage_mode_t").ToLocalChecked(), storage_mode_t);
    };
}; // namespace nodelt
