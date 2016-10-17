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
    Local<Object> alert_to_object(const libtorrent::alert& alert) {
        Nan::EscapableHandleScope scope;
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("message").ToLocalChecked(), Nan::New(alert.message()).ToLocalChecked());
        obj->Set(Nan::New("what").ToLocalChecked(), Nan::New(alert.what()).ToLocalChecked());
        obj->Set(Nan::New("type").ToLocalChecked(), Nan::New<Integer>(alert.type()));
        obj->Set(Nan::New("category").ToLocalChecked(), Nan::New<Integer>(alert.category()));
        obj->Set(Nan::New("handle").ToLocalChecked(), alert_to_handle(alert));

        return scope.Escape(obj);
    };

    Local<Object> alert_to_handle(const libtorrent::alert &alert) {
        Nan::EscapableHandleScope scope;
        Local<Object> obj = Nan::New<Object>();

        /* cast to some particular alert type */
        auto castedHandle = getHandle <libtorrent::add_torrent_alert,
                                 libtorrent::torrent_removed_alert,
                                 libtorrent::save_resume_data_alert,
                                 libtorrent::save_resume_data_failed_alert,
                                 libtorrent::metadata_received_alert,
                                 libtorrent::torrent_finished_alert> (alert);

        /*if(castedHandle == nullptr) {
             obj->Set(Nan::New("handle").ToLocalChecked(), Nan::Undefined());
        } else {*/
            obj = TorrentHandleWrap::FromExisting(castedHandle);
        //}

        return scope.Escape(obj);
    }

    void bind_alert(Local<Object> target) {
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
