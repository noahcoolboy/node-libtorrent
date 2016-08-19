#include <v8.h>
#include <nan.h>

#include <libtorrent/entry.hpp>

#include "entry.hpp"

using namespace v8;


namespace nodelt {
    Local<Value> entry_to_object(const libtorrent::entry& e_) {
        Nan::EscapableHandleScope scope;

        if (e_.type() == libtorrent::entry::int_t) {
            return scope.Escape(Nan::New<Integer>(e_.integer()));
        } else if (e_.type() == libtorrent::entry::string_t) {
            return scope.Escape(Nan::New<String>(e_.string()).ToLocalChecked());
        } else if (e_.type() == libtorrent::entry::list_t) {
            Local<Array> obj = Nan::New<Array>();

            for (libtorrent::entry::list_type::const_iterator i(e_.list().begin()), e(e_.list().end()); i != e; ++i)
                obj->Set(obj->Length(), entry_to_object(*i));

            return scope.Escape(obj);
        } else if (e_.type() == libtorrent::entry::dictionary_t) {
            Local<Object> obj = Nan::New<Object>();

            for (libtorrent::entry::dictionary_type::const_iterator i(e_.dict().begin()), e(e_.dict().end()); i != e; ++i)
                obj->Set(Nan::New<String>(i->first), entry_to_object(i->second));

            return scope.Escape(obj);
        }

        return scope.Escape(Nan::New<Object>());
    };

    libtorrent::entry entry_from_object(Local<Value> obj) {
        libtorrent::entry e_;

        if (obj->IsNumber()) {
            e_ = (libtorrent::entry::integer_type) obj->IntegerValue();
        } else if (obj->IsString()) {
            e_ = std::string(*Nan::Utf8String(obj).ToLocalChecked());
        } else if (obj->IsArray()) {
            libtorrent::entry::list_type res;
            Local<Array> src = Array::Cast(*obj);

            for (uint32_t i = 0, e = src->Length(); i < e; ++i)
                res.push_back(entry_from_object(src->Get(i)));

            e_ = res;
        } else if (obj->IsObject()) {
            libtorrent::entry::dictionary_type res;
            Local<Object> src = obj.As<Object>();
            Local<Array> keys = src->GetOwnPropertyNames();

            for (uint32_t i = 0, e = keys->Length(); i < e; ++i)
                res.insert(std::make_pair(
                    std::string(Nan::New<String>(keys->Get(i))),
                    entry_from_object(src->Get(i))));

            e_ = res;
        }

        return e_;
    };
}; // namespace nodelt
