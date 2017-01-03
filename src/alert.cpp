#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>

#include "alert.hpp"
#include "torrent_handle.hpp"

using namespace v8;
using namespace node;


namespace nodelt {
    Nan::Persistent<Function> AlertWrap::constructor;

    AlertWrap::AlertWrap() {
        obj_ = NULL;
    };

    AlertWrap::~AlertWrap() {
        if (obj_ != NULL)
            delete obj_;
    };

    void AlertWrap::Initialize(Local<Object> target) {
        Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);

        tpl->SetClassName(Nan::New("TorrentAlert").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetPrototypeMethod(tpl, "what", what);
        Nan::SetPrototypeMethod(tpl, "type", type);
        Nan::SetPrototypeMethod(tpl, "handle", handle);
        Nan::SetPrototypeMethod(tpl, "message", message);
        Nan::SetPrototypeMethod(tpl, "category", category);
        Nan::SetPrototypeMethod(tpl, "buffer", buffer);
        Nan::SetPrototypeMethod(tpl, "size", size);

        constructor.Reset(tpl->GetFunction());
    };

    Local<Object> AlertWrap::New(const libtorrent::alert* a) {
        Nan::EscapableHandleScope scope;

        Local<Function> c = Nan::New<Function>(constructor);
        Nan::MaybeLocal<Object> obj = c->NewInstance();//Nan::GetCurrentContext());

        if(a) {
            try {
                auto casted = getObject <libtorrent::add_torrent_alert,
                                 libtorrent::torrent_removed_alert,
                                 libtorrent::save_resume_data_alert,
                                 libtorrent::save_resume_data_failed_alert,
                                 libtorrent::metadata_received_alert,
                                 libtorrent::torrent_finished_alert> (*a);

                Nan::ObjectWrap::Unwrap<AlertWrap>(obj.ToLocalChecked())->obj_ = casted;
                return scope.Escape(obj.ToLocalChecked());
            } catch(std::exception e) {
                /* casting failed */
                Nan::ObjectWrap::Unwrap<AlertWrap>(obj.ToLocalChecked())->obj_ = a;
                return scope.Escape(obj.ToLocalChecked());
            }
        } else {
            return scope.Escape(Nan::New<Object>());
        }
    };

    NAN_METHOD(AlertWrap::NewInstance) {
        Nan::HandleScope scope;

        if (!info.IsConstructCall()) {
            Nan::ThrowTypeError("Use the new operator to create instances of this object.");
            return;
        }

        AlertWrap* a = new AlertWrap();

        a->Wrap(info.This());

        info.GetReturnValue().Set(info.This());
    };

    NAN_METHOD(AlertWrap::what) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<String>(AlertWrap::Unwrap(info.This())->what()).ToLocalChecked());
    };

    NAN_METHOD(AlertWrap::type) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Integer>(AlertWrap::Unwrap(info.This())->type()));
    };

    NAN_METHOD(AlertWrap::message) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<String>(AlertWrap::Unwrap(info.This())->message()).ToLocalChecked());
    };

    NAN_METHOD(AlertWrap::category) {
        Nan::HandleScope scope;

        info.GetReturnValue().Set(Nan::New<Integer>(AlertWrap::Unwrap(info.This())->category()));
    };

    NAN_METHOD(AlertWrap::buffer) {
        Nan::HandleScope scope;
        info.GetReturnValue().Set(
            Nan::Encode(
                getBuffer  <libtorrent::read_piece_alert> (*AlertWrap::Unwrap(info.This())),
                getSize    <libtorrent::read_piece_alert> (*AlertWrap::Unwrap(info.This()))
            )
        );
    };

    NAN_METHOD(AlertWrap::size) {
        Nan::HandleScope scope;
        info.GetReturnValue().Set(
            Nan::New<Number>(
                getSize    <libtorrent::read_piece_alert> (*AlertWrap::Unwrap(info.This()))
            )
        );
    };

    NAN_METHOD(AlertWrap::handle) {
        Nan::HandleScope scope;

        try {
            auto casted = getHandle <libtorrent::add_torrent_alert,
                                 libtorrent::torrent_removed_alert,
                                 libtorrent::save_resume_data_alert,
                                 libtorrent::save_resume_data_failed_alert,
                                 libtorrent::metadata_received_alert,
                                 libtorrent::torrent_finished_alert,
                                 libtorrent::read_piece_alert> (*AlertWrap::Unwrap(info.This()));

            info.GetReturnValue().Set(TorrentHandleWrap::FromExisting(casted));
        } catch (std::exception e) {
            /* casting failed */
            info.GetReturnValue().SetUndefined();
        }
    };

    void bind_alert(Local<Object> target) {
        AlertWrap::Initialize(target);

        Local<Object> alert_t = Nan::New<Object>();
        Local<Object> category_t = Nan::New<Object>();

        // add categories
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

        // define & add few alert types
        alert_t->Set(Nan::New("add_torrent_alert").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::add_torrent_alert::alert_type));
        alert_t->Set(Nan::New("torrent_removed_alert").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::torrent_removed_alert::alert_type));
        alert_t->Set(Nan::New("save_resume_data_alert").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::save_resume_data_alert::alert_type));
        alert_t->Set(Nan::New("save_resume_data_failed_alert").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::save_resume_data_failed_alert::alert_type));
        alert_t->Set(Nan::New("metadata_received_alert").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::metadata_received_alert::alert_type));
        alert_t->Set(Nan::New("torrent_finished_alert").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::torrent_finished_alert::alert_type));

        target->Set(Nan::New("alert_t").ToLocalChecked(), alert_t);
  };
}; // namespace nodelt
