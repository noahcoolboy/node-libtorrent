#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/ip_filter.hpp>

#include "ip_filter.hpp"

using namespace v8;
using namespace node;


namespace nodelt {
    Nan::Persistent<Function> IpFilterWrap::constructor;

    void bind_ip_filter(Local<Object> target) {
        IpFilterWrap::Initialize(target);
    };

    void IpFilterWrap::Initialize(Local<Object> target) {
        Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);

        tpl->SetClassName(Nan::New("ip_filter").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetPrototypeMethod(tpl, "add_rule", add_rule);
        Nan::SetPrototypeMethod(tpl, "access", access);
        Nan::SetPrototypeMethod(tpl, "export_filter", export_filter);

        constructor.Reset(tpl->GetFunction());
    };

    IpFilterWrap::IpFilterWrap() {
        obj_ = NULL;
    };

    IpFilterWrap::~IpFilterWrap() {
        if (obj_ != NULL)
            delete obj_;
    };

    Local<Object> IpFilterWrap::New(const libtorrent::ip_filter& f) {
        Nan::EscapableHandleScope scope;

        Local<Function> c = Nan::New<Function>(constructor);
        Nan::MaybeLocal<Object> obj = c->NewInstance(Nan::GetCurrentContext());

        Nan::ObjectWrap::Unwrap<IpFilterWrap>(obj.ToLocalChecked())->obj_ = new libtorrent::ip_filter(f);

        return scope.Escape(obj.ToLocalChecked());
    };

    template <class Addr> Local<Object> ip_range_to_object(const libtorrent::ip_range<Addr>& s) {
        Nan::EscapableHandleScope scope;

        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("first").ToLocalChecked(), Nan::New<String>(s.first.to_string()).ToLocalChecked());
        obj->Set(Nan::New("last").ToLocalChecked(), Nan::New<String>(s.first.to_string()).ToLocalChecked());
        obj->Set(Nan::New("flags").ToLocalChecked(), Nan::New<Integer>(s.flags));

        return scope.Escape(obj);
    };

    NAN_METHOD(IpFilterWrap::NewInstance) {
        Nan::HandleScope scope;

        if (!info.IsConstructCall()) {
            Nan::ThrowTypeError("Use the new operator to create instances of this object.");
            return;
        }

        IpFilterWrap* fs = new IpFilterWrap();
        fs->Wrap(info.This());

        info.GetReturnValue().Set(info.This());
    };

    NAN_METHOD(IpFilterWrap::add_rule) {
        Nan::HandleScope scope;

        libtorrent::address first, last;
        first = libtorrent::address::from_string(std::string(*Nan::Utf8String(info[0])));
        last = libtorrent::address::from_string(std::string(*Nan::Utf8String(info[1])));
        IpFilterWrap::Unwrap(info.This())->add_rule(first, last, (int)info[2]->IntegerValue());

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(IpFilterWrap::access) {
        Nan::HandleScope scope;

        int ret = IpFilterWrap::Unwrap(info.This())->access(
            libtorrent::address::from_string(std::string(*Nan::Utf8String(info[0]))));

        info.GetReturnValue().Set(Nan::New<Integer>(ret));
    };


    NAN_METHOD(IpFilterWrap::export_filter) {
        Nan::HandleScope scope;

        libtorrent::ip_filter::filter_tuple_t res = IpFilterWrap::Unwrap(info.This())->export_filter();
        Local<Array> ret = Nan::New<Array>();

#if TORRENT_USE_IPV6
        Local<Array> v4 = Nan::New<Array>(), v6 = Nan::New<Array>();

        for (std::vector<libtorrent::ip_range<libtorrent::address_v4> >::iterator
            i(res.get<0>().begin()), e(res.get<0>().end()); i != e; ++i)
            v4->Set(v4->Length(), ip_range_to_object<libtorrent::address_v4>(*i));

        for (std::vector<libtorrent::ip_range<libtorrent::address_v6> >::iterator
            i(res.get<1>().begin()), e(res.get<1>().end()); i != e; ++i)
            v6->Set(v6->Length(), ip_range_to_object<libtorrent::address_v6>(*i));

        ret->Set(0, v4);
        ret->Set(1, v6);
#else
        for (std::vector<libtorrent::ip_range<libtorrent::address_v4> >::iterator
            i(res.begin()), e(res.end()); i != e; ++i)
            ret->Set(ret->Length(), ip_range_to_object<libtorrent::address_v4>(*i));
#endif
        info.GetReturnValue().Set(ret);
    };
}; // namespace nodelt
