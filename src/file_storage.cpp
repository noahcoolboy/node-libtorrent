#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/file_storage.hpp>

#include "file_storage.hpp"

using namespace v8;
using namespace node;


namespace nodelt {

    Nan::Persistent<Function> FileStorageWrap::constructor;

    FileStorageWrap::FileStorageWrap() {
        obj_ = new libtorrent::file_storage();
    };

    FileStorageWrap::~FileStorageWrap() {
        delete obj_;
    };

    void bind_file_storage(Local<Object> target) {
        FileStorageWrap::Initialize(target);
    };

    Local<Object> file_entry_to_object(const libtorrent::file_entry& e) {
        Nan::EscapableHandleScope scope;

        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("path").ToLocalChecked(), Nan::New<String>(e.path).ToLocalChecked());
        obj->Set(Nan::New("offset").ToLocalChecked(), Nan::New<Integer>((uint32_t)e.offset));
        obj->Set(Nan::New("size").ToLocalChecked(), Nan::New<Integer>((uint32_t)e.size));
        obj->Set(Nan::New("file_base").ToLocalChecked(), Nan::New<Integer>((uint32_t)e.file_base));
        obj->Set(Nan::New("mtime").ToLocalChecked(), Nan::New<Integer>((uint32_t)e.mtime));
        obj->Set(Nan::New("filehash").ToLocalChecked(), Nan::New<String>(libtorrent::to_hex(e.filehash.to_string())).ToLocalChecked());
        obj->Set(Nan::New("pad_file").ToLocalChecked(), Nan::New<Boolean>(e.pad_file));
        obj->Set(Nan::New("hidden_attribute").ToLocalChecked(), Nan::New<Boolean>(e.hidden_attribute));
        obj->Set(Nan::New("executable_attribute").ToLocalChecked(), Nan::New<Boolean>(e.executable_attribute));
        obj->Set(Nan::New("symlink_attribute").ToLocalChecked(), Nan::New<Boolean>(e.symlink_attribute));
        obj->Set(Nan::New("symlink_path").ToLocalChecked(), Nan::New<String>(e.symlink_path).ToLocalChecked());

        return scope.Escape(obj);
    };

    libtorrent::file_entry file_entry_from_object(Local<Object> obj) {
        libtorrent::file_entry e;

        if (Nan::Has(obj, Nan::New("path").ToLocalChecked()) == Nan::Just(true))
            e.path = std::string(*Nan::Utf8String(Nan::Get(obj, Nan::New("path").ToLocalChecked()).ToLocalChecked()));
        if (Nan::Has(obj, Nan::New("offset").ToLocalChecked()) == Nan::Just(true))
            e.offset = (Nan::Get(obj, Nan::New("offset").ToLocalChecked())).ToLocalChecked()->IntegerValue();
        if (Nan::Has(obj, Nan::New("size").ToLocalChecked()) == Nan::Just(true))
            e.size = (Nan::Get(obj, Nan::New("size").ToLocalChecked())).ToLocalChecked()->IntegerValue();
        if (Nan::Has(obj, Nan::New("file_base").ToLocalChecked()) == Nan::Just(true))
            e.file_base = (Nan::Get(obj, Nan::New("file_base").ToLocalChecked())).ToLocalChecked()->IntegerValue();
        if (Nan::Has(obj, Nan::New("mtime").ToLocalChecked()) == Nan::Just(true))
            e.mtime = (Nan::Get(obj, Nan::New("mtime").ToLocalChecked())).ToLocalChecked()->IntegerValue();
        if (Nan::Has(obj, Nan::New("filehash").ToLocalChecked()) == Nan::Just(true)) {
            Nan::Utf8String filehash((Nan::Get(obj, Nan::New("filehash").ToLocalChecked())).ToLocalChecked());
            libtorrent::from_hex(*filehash, 40, (char*)&e.filehash[0]);
        }
        if (Nan::Has(obj, Nan::New("pad_file").ToLocalChecked()) == Nan::Just(true))
            e.pad_file = (Nan::Get(obj, Nan::New("pad_file").ToLocalChecked())).ToLocalChecked()->BooleanValue();
        if (Nan::Has(obj, Nan::New("hidden_attribute").ToLocalChecked()) == Nan::Just(true))
            e.hidden_attribute = (Nan::Get(obj, Nan::New("offset").ToLocalChecked())).ToLocalChecked()->BooleanValue();
        if (Nan::Has(obj, Nan::New("executable_attribute").ToLocalChecked()) == Nan::Just(true))
            e.executable_attribute = (Nan::Get(obj, Nan::New("executable_attribute").ToLocalChecked())).ToLocalChecked()->BooleanValue();
        if (Nan::Has(obj, Nan::New("symlink_attribute").ToLocalChecked()) == Nan::Just(true))
            e.symlink_attribute = (Nan::Get(obj, Nan::New("symlink_attribute").ToLocalChecked())).ToLocalChecked()->BooleanValue();
        if (Nan::Has(obj, Nan::New("symlink_path").ToLocalChecked()) == Nan::Just(true))
            e.symlink_path = std::string(*Nan::Utf8String(Nan::Get(obj, Nan::New("symlink_path").ToLocalChecked()).ToLocalChecked()));

        return e;
    };

    Local<Object> file_slice_to_object(const libtorrent::file_slice& fs) {
        Nan::EscapableHandleScope scope;

        Local<Object> ret = Nan::New<Object>();

        ret->Set(Nan::New("file_index").ToLocalChecked(), Nan::New<Number>(fs.file_index));
        ret->Set(Nan::New("offset").ToLocalChecked(), Nan::New<Number>(fs.offset));
        ret->Set(Nan::New("size").ToLocalChecked(), Nan::New<Number>(fs.size));

        return scope.Escape(ret);
    };

    void FileStorageWrap::Initialize(Local<Object> target) {
        Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);

        tpl->SetClassName(Nan::New("file_storage").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetPrototypeMethod(tpl, "is_valid", is_valid);
        Nan::SetPrototypeMethod(tpl, "add_file", add_file);
        Nan::SetPrototypeMethod(tpl, "num_files", num_files);
        Nan::SetPrototypeMethod(tpl, "at", at);
        Nan::SetPrototypeMethod(tpl, "total_size", total_size);
        Nan::SetPrototypeMethod(tpl, "set_num_pieces", set_num_pieces);
        Nan::SetPrototypeMethod(tpl, "num_pieces", num_pieces);
        Nan::SetPrototypeMethod(tpl, "set_piece_length", set_piece_length);
        Nan::SetPrototypeMethod(tpl, "piece_length", piece_length);
        Nan::SetPrototypeMethod(tpl, "piece_size", piece_size);
        Nan::SetPrototypeMethod(tpl, "set_name", set_name);
        Nan::SetPrototypeMethod(tpl, "rename_file", rename_file);
        Nan::SetPrototypeMethod(tpl, "name", name);

        constructor.Reset(tpl->GetFunction());

        target->Set(Nan::New("file_storage").ToLocalChecked(), tpl->GetFunction());
    };

    Local<Object> FileStorageWrap::New(const libtorrent::file_storage& fs) {
        Nan::EscapableHandleScope scope;

        Local<Function> c = Nan::New<Function>(constructor);
        Nan::MaybeLocal<Object> obj = c->NewInstance(Nan::GetCurrentContext());

        // what's the purpose of this?
        delete FileStorageWrap::Unwrap(obj.ToLocalChecked());
        Nan::ObjectWrap::Unwrap<FileStorageWrap>(obj.ToLocalChecked())->obj_ = new libtorrent::file_storage(fs);

        return scope.Escape(obj.ToLocalChecked());
    };

    NAN_METHOD(FileStorageWrap::NewInstance) {
        Nan::HandleScope scope;

        if (!info.IsConstructCall()) {
            Nan::ThrowTypeError("Use the new operator to create instances of this object.");
            return;
        }

        FileStorageWrap* fs = new FileStorageWrap();
        fs->Wrap(info.This());

        info.GetReturnValue().Set(info.This());
    };


    NAN_METHOD(FileStorageWrap::add_file) {
        Nan::HandleScope scope;
    
        libtorrent::file_storage* fs = FileStorageWrap::Unwrap(info.This());

        if (info[0]->IsObject()) {
            fs->add_file(file_entry_from_object(info[0]->ToObject()));
        } else {
            std::string p(*Nan::Utf8String(info[0]));
            libtorrent::size_type size(info[1]->IntegerValue());

            if (info.Length() == 3)
                fs->add_file(p, size, info[2]->IntegerValue());
            else
                fs->add_file(p, size);
        }

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(FileStorageWrap::is_valid) {
        Nan::HandleScope scope;

        libtorrent::file_storage* fs = FileStorageWrap::Unwrap(info.This());

        info.GetReturnValue().Set(Nan::New<Boolean>(fs->is_valid()));
    };

    NAN_METHOD(FileStorageWrap::num_files) {
        Nan::HandleScope scope;

        libtorrent::file_storage* fs = FileStorageWrap::Unwrap(info.This());

        info.GetReturnValue().Set(Nan::New<Number>(fs->num_files()));
    };

    NAN_METHOD(FileStorageWrap::at) {
        Nan::HandleScope scope;

        libtorrent::file_storage* fs = FileStorageWrap::Unwrap(info.This());

        info.GetReturnValue().Set(file_entry_to_object(fs->at(info[0]->IntegerValue())));
    };

    NAN_METHOD(FileStorageWrap::total_size) {
        Nan::HandleScope scope;

        libtorrent::file_storage* fs = FileStorageWrap::Unwrap(info.This());

        info.GetReturnValue().Set(Nan::New<Number>(fs->total_size()));
    };

    NAN_METHOD(FileStorageWrap::set_num_pieces) {
        Nan::HandleScope scope;

        libtorrent::file_storage* fs = FileStorageWrap::Unwrap(info.This());

        fs->set_num_pieces(info[0]->IntegerValue());

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(FileStorageWrap::num_pieces) {
        Nan::HandleScope scope;

        libtorrent::file_storage* fs = FileStorageWrap::Unwrap(info.This());

        info.GetReturnValue().Set(Nan::New<Integer>(fs->num_pieces()));
    };

    NAN_METHOD(FileStorageWrap::set_piece_length) {
        Nan::HandleScope scope;

        FileStorageWrap::Unwrap(info.This())->set_piece_length(info[0]->IntegerValue());

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(FileStorageWrap::piece_length) {
        Nan::HandleScope scope;

        libtorrent::file_storage* fs = FileStorageWrap::Unwrap(info.This());

        info.GetReturnValue().Set(Nan::New<Integer>(fs->piece_length()));
    };

    NAN_METHOD(FileStorageWrap::piece_size) {
        Nan::HandleScope scope;

        libtorrent::file_storage* fs = FileStorageWrap::Unwrap(info.This());

        info.GetReturnValue().Set(Nan::New<Integer>(fs->piece_size(info[0]->IntegerValue())));
    };

    NAN_METHOD(FileStorageWrap::set_name) {
        Nan::HandleScope scope;

        FileStorageWrap::Unwrap(info.This())->set_name(std::string(*Nan::Utf8String(info[0])));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(FileStorageWrap::rename_file) {
        Nan::HandleScope scope;

        FileStorageWrap::Unwrap(info.This())->rename_file(info[0]->IntegerValue(), std::string(*Nan::Utf8String(info[1])));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(FileStorageWrap::name) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<String>(FileStorageWrap::Unwrap(info.This())->name()).ToLocalChecked());
    };
}; // namespace nodelt
