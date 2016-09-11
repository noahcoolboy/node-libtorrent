#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/fingerprint.hpp>

#include "fingerprint.hpp"

using namespace v8;
using namespace node;


namespace nodelt {
    Nan::Persistent<Function> FingerprintWrap::constructor;

    FingerprintWrap::FingerprintWrap(const char* id_string, int major, int minor, int revision, int tag) {
        obj_ = new libtorrent::fingerprint(id_string, major, minor, revision, tag);
    };

    FingerprintWrap::~FingerprintWrap() {
        delete obj_;
    };

    void bind_fingerprint(Local<Object> target) {
        FingerprintWrap::Initialize(target);
    };

    void FingerprintWrap::Initialize(Local<Object> target) {
        Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);

        tpl->SetClassName(Nan::New("fingerprint").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetPrototypeMethod(tpl, "to_string", to_string);

        constructor.Reset(tpl->GetFunction());

        target->Set(Nan::New("fingerprint").ToLocalChecked(), tpl->GetFunction());
    };


    NAN_METHOD(FingerprintWrap::NewInstance) {
        Nan::HandleScope scope;

        if (!info.IsConstructCall()) {
            Nan::ThrowTypeError("Use the new operator to create instances of this object.");
            return;
        }

        FingerprintWrap* fp = new FingerprintWrap(
            std::string(*Nan::Utf8String(info[0]->ToString())).c_str(),
            info[1]->Int32Value(),
            info[2]->Int32Value(),
            info[3]->Int32Value(),
            info[4]->Int32Value());

        fp->Wrap(info.This());

        libtorrent::fingerprint* fp_ = FingerprintWrap::Unwrap(info.This());

        info.This()->Set(Nan::New("name").ToLocalChecked(), Nan::New<String>(fp_->name).ToLocalChecked());
        info.This()->Set(Nan::New("major_version").ToLocalChecked(), Nan::New<Integer>(fp_->major_version));
        info.This()->Set(Nan::New("minor_version").ToLocalChecked(), Nan::New<Integer>(fp_->minor_version));
        info.This()->Set(Nan::New("revision_version").ToLocalChecked(), Nan::New<Integer>(fp_->revision_version));
        info.This()->Set(Nan::New("tag_version").ToLocalChecked(), Nan::New<Integer>(fp_->tag_version));

        info.GetReturnValue().Set(info.This());
    };

    NAN_METHOD(FingerprintWrap::to_string) {
        Nan::HandleScope scope;

        libtorrent::fingerprint* fp = FingerprintWrap::Unwrap(info.This());

        info.GetReturnValue().Set(Nan::New<String>(fp->to_string()).ToLocalChecked());
    };
}; // namespace nodelt
