#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/alert.hpp>

#include "alert.hpp"

using namespace v8;
using namespace node;


namespace nodelt {
    v8::Local<v8::Object> alert_to_object(const libtorrent::alert& alert) {
        Nan::EscapableHandleScope scope;
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("message").ToLocalChecked(), Nan::New(alert.message()).ToLocalChecked());
        obj->Set(Nan::New("what").ToLocalChecked(), Nan::New(alert.what()).ToLocalChecked());
        obj->Set(Nan::New("category").ToLocalChecked(), Nan::New<Integer>(alert.category()));

        return scope.Escape(obj);
    };

    void bind_alert(Local<Object> target) {
        Local<Object> category_t = Nan::New<Object>();

        category_t->Set(Nan::New("error_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::error_notification));
        category_t->Set(Nan::New("peer_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::peer_notification));
        category_t->Set(Nan::New("port_mapping_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::port_mapping_notification));
        category_t->Set(Nan::New("storage_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::storage_notification));
        category_t->Set(Nan::New("tracker_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::tracker_notification));
        category_t->Set(Nan::New("debug_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::debug_notification));
        category_t->Set(Nan::New("status_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::status_notification));
        category_t->Set(Nan::New("progress_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::progress_notification));
        category_t->Set(Nan::New("ip_block_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::ip_block_notification));
        category_t->Set(Nan::New("performance_warning").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::performance_warning));
        category_t->Set(Nan::New("dht_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::dht_notification));
        category_t->Set(Nan::New("stats_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::stats_notification));
        category_t->Set(Nan::New("rss_notification").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::rss_notification));
        category_t->Set(Nan::New("all_categories").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::alert::all_categories));

        target->Set(Nan::New("category_t").ToLocalChecked(), category_t);
  };
}; // namespace nodelt
