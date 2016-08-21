#include <v8.h>
#include <node.h>

#include <libtorrent/rss.hpp>

#include "add_torrent_params.hpp"
#include "rss.hpp"
#include "torrent_handle.hpp"


using namespace v8;
using namespace node;


namespace nodelt {
    Nan::Persistent<Function> FeedHandleWrap::constructor;

    FeedHandleWrap::FeedHandleWrap() {
        obj_ = NULL;
    };

    FeedHandleWrap::~FeedHandleWrap() {
        if (obj_ != NULL)
            delete obj_;
    };

    libtorrent::feed_settings feed_settings_from_object(Local<Object> obj) {
        libtorrent::feed_settings fs;

        if (Nan::Has(obj, Nan::New("url").ToLocalChecked()) == Nan::Just(true))
            fs.url = std::string(*Nan::Utf8String(Nan::Get(obj, Nan::New("url").ToLocalChecked()).ToLocalChecked()));
        if (Nan::Has(obj, Nan::New("auto_download").ToLocalChecked()) == Nan::Just(true))
            fs.auto_download = (Nan::Get(obj, Nan::New("auto_download").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("auto_map_handles").ToLocalChecked()) == Nan::Just(true))
            fs.auto_download = (Nan::Get(obj, Nan::New("auto_map_handles").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("default_ttl").ToLocalChecked()) == Nan::Just(true))
            fs.auto_download = (Nan::Get(obj, Nan::New("default_ttl").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("add_args").ToLocalChecked()) == Nan::Just(true))
            fs.add_args = add_torrent_params_from_object((Nan::Get(obj, Nan::New("add_args").ToLocalChecked()).ToLocalChecked()).As<Object>());

        return fs;
    };

    Local<Object> feed_settings_to_object(const libtorrent::feed_settings& fs) {
        Nan::EscapableHandleScope scope;

        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("url").ToLocalChecked(), Nan::New<String>(fs.url).ToLocalChecked());
        obj->Set(Nan::New("auto_download").ToLocalChecked(), Nan::New<Boolean>(fs.auto_download));
        obj->Set(Nan::New("default_ttl").ToLocalChecked(), Nan::New<Integer>(fs.default_ttl));

        return scope.Escape(obj);
    };

    void FeedHandleWrap::Initialize(Local<Object> target) {
        Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);

        tpl->SetClassName(Nan::New("FeedHandle").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetPrototypeMethod(tpl, "update_feed", update_feed);
        Nan::SetPrototypeMethod(tpl, "get_feed_status", get_feed_status);
        Nan::SetPrototypeMethod(tpl, "set_settings", set_settings);
        Nan::SetPrototypeMethod(tpl, "settings", settings);

        constructor.Reset(tpl->GetFunction());
    };

    Local<Object> FeedHandleWrap::New(const libtorrent::feed_handle& fh) {
        Nan::EscapableHandleScope scope;

        Local<Function> c = Nan::New<Function>(constructor);
        Nan::MaybeLocal<Object> obj = c->NewInstance(Nan::GetCurrentContext());

        Nan::ObjectWrap::Unwrap<FeedHandleWrap>(obj.ToLocalChecked())->obj_ = new libtorrent::feed_handle(fh);

        return scope.Escape(obj.ToLocalChecked());
    };

    NAN_METHOD(FeedHandleWrap::NewInstance) {
        Nan::HandleScope scope;

        if (!info.IsConstructCall()) {
            Nan::ThrowTypeError("Use the new operator to create instances of this object.");
            return;
        }

        FeedHandleWrap* fh = new FeedHandleWrap();
        fh->Wrap(info.This());

        info.GetReturnValue().Set(info.This());
    };

    NAN_METHOD(FeedHandleWrap::update_feed) {
        Nan::HandleScope scope;

        FeedHandleWrap::Unwrap(info.This())->update_feed();

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(FeedHandleWrap::get_feed_status) {
        Nan::HandleScope scope;

        libtorrent::feed_status s = FeedHandleWrap::Unwrap(info.This())->get_feed_status();
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("url").ToLocalChecked(), Nan::New<String>(s.url).ToLocalChecked());
        obj->Set(Nan::New("title").ToLocalChecked(), Nan::New<String>(s.url).ToLocalChecked());
        obj->Set(Nan::New("description").ToLocalChecked(), Nan::New<String>(s.url).ToLocalChecked());
        obj->Set(Nan::New("next_update").ToLocalChecked(), Nan::New<Integer>(s.next_update));
        obj->Set(Nan::New("updating").ToLocalChecked(), Nan::New<Boolean>(s.updating));

        Local<Array> items = Nan::New<Array>();

        for (std::vector<libtorrent::feed_item>::iterator i(s.items.begin()) , e(s.items.end()); i != e; ++i) {
            Local<Object> item = Nan::New<Object>();

            item->Set(Nan::New("url").ToLocalChecked(), Nan::New<String>(i->url).ToLocalChecked());
            item->Set(Nan::New("uuid").ToLocalChecked(), Nan::New<String>(i->uuid).ToLocalChecked());
            item->Set(Nan::New("title").ToLocalChecked(), Nan::New<String>(i->title).ToLocalChecked());
            item->Set(Nan::New("description").ToLocalChecked(), Nan::New<String>(i->description).ToLocalChecked());
            item->Set(Nan::New("comment").ToLocalChecked(), Nan::New<String>(i->comment).ToLocalChecked());
            item->Set(Nan::New("category").ToLocalChecked(), Nan::New<String>(i->category).ToLocalChecked());
            item->Set(Nan::New("size").ToLocalChecked(), Nan::New<Number>(i->size));
            item->Set(Nan::New("handle").ToLocalChecked(), TorrentHandleWrap::New(i->handle));
            item->Set(Nan::New("info_hash").ToLocalChecked(), Nan::New<String>(libtorrent::to_hex(i->info_hash.to_string())).ToLocalChecked());

            items->Set(items->Length(), item);
        }

        obj->Set(Nan::New("items").ToLocalChecked(), items);
        obj->Set(Nan::New("error").ToLocalChecked(), Nan::New<String>(s.error ? s.error.message() : "").ToLocalChecked());
        obj->Set(Nan::New("ttl").ToLocalChecked(), Nan::New<Integer>(s.ttl));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(FeedHandleWrap::set_settings) {
        Nan::HandleScope scope;

        FeedHandleWrap::Unwrap(info.This())->set_settings(feed_settings_from_object(info[0]->ToObject()));

        info.GetReturnValue().SetUndefined();
    };

    NAN_METHOD(FeedHandleWrap::settings) {
        Nan::HandleScope scope;

        libtorrent::feed_settings fs = FeedHandleWrap::Unwrap(info.This())->settings();

        info.GetReturnValue().Set(feed_settings_to_object(fs));
    };
}; //namespace nodelt
