#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/bitfield.hpp>

#include "torrent_handle.hpp"
#include "torrent_status.hpp"


using namespace v8;
using namespace node;


namespace nodelt {
    void bind_torrent_status(Local<Object> target) {
        // set libtorrent::torrent_status::state_t
        Local<Object> state_t = Nan::New<Object>();

        state_t->Set(Nan::New("queued_for_checking").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::torrent_status::queued_for_checking));
        state_t->Set(Nan::New("checking_files").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::torrent_status::checking_files));
        state_t->Set(Nan::New("downloading_metadata").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::torrent_status::downloading_metadata));
        state_t->Set(Nan::New("downloading").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::torrent_status::downloading));
        state_t->Set(Nan::New("finished").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::torrent_status::finished));
        state_t->Set(Nan::New("seeding").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::torrent_status::seeding));
        state_t->Set(Nan::New("allocating").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::torrent_status::allocating));
        state_t->Set(Nan::New("checking_resume_data").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::torrent_status::checking_resume_data));
        target->Set(Nan::New("states").ToLocalChecked(), state_t);
    };

    Local<Array> bitfield_to_array(const libtorrent::bitfield& bf) {
        Nan::EscapableHandleScope scope;

        Local<Array> obj = Nan::New<Array>();

        for (libtorrent::bitfield::const_iterator i(bf.begin()), e(bf.end()); i != e; ++i)
            obj->Set(obj->Length(), Nan::New<Boolean>(*i));

        return scope.Escape(obj);
    };

    Local<Object> time_duration_to_object(const boost::posix_time::time_duration& td) {
        Nan::EscapableHandleScope scope;

        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("seconds").ToLocalChecked(),      Nan::New<Integer>(td.total_seconds()));
        obj->Set(Nan::New("milliseconds").ToLocalChecked(), Nan::New<Number>(td.total_milliseconds()));
        obj->Set(Nan::New("microseconds").ToLocalChecked(), Nan::New<Number>(td.total_microseconds()));

        return scope.Escape(obj);
    };

    Handle<Value> torrent_status_to_object(const libtorrent::torrent_status& st) {
        Nan::EscapableHandleScope scope;

        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("handle").ToLocalChecked(), TorrentHandleWrap::New(st.handle));

        obj->Set(Nan::New("state").ToLocalChecked(), Nan::New<Integer>(st.state));
        obj->Set(Nan::New("paused").ToLocalChecked(), Nan::New<Boolean>(st.paused));
        obj->Set(Nan::New("auto_managed").ToLocalChecked(), Nan::New<Boolean>(st.auto_managed));
        obj->Set(Nan::New("sequential_download").ToLocalChecked(), Nan::New<Boolean>(st.sequential_download));
        obj->Set(Nan::New("seeding").ToLocalChecked(), Nan::New<Boolean>(st.seeding));
        obj->Set(Nan::New("finished").ToLocalChecked(), Nan::New<Boolean>(st.finished));
        obj->Set(Nan::New("progress").ToLocalChecked(), Nan::New<Number>(st.progress));
        obj->Set(Nan::New("progressPpm").ToLocalChecked(), Nan::New<Integer>(st.progress_ppm));
        obj->Set(Nan::New("error").ToLocalChecked(), Nan::New<String>(st.error.c_str()).ToLocalChecked());

        obj->Set(Nan::New("next_announce").ToLocalChecked(), time_duration_to_object(st.next_announce));
        obj->Set(Nan::New("announce_interval").ToLocalChecked(), time_duration_to_object(st.announce_interval));

        obj->Set(Nan::New("current_tracker").ToLocalChecked(), Nan::New<String>(st.current_tracker.c_str()).ToLocalChecked());

        obj->Set(Nan::New("total_download").ToLocalChecked(), Nan::New<Number>(st.total_download));
        obj->Set(Nan::New("total_upload").ToLocalChecked(), Nan::New<Number>(st.total_upload));

        obj->Set(Nan::New("total_payload_download").ToLocalChecked(), Nan::New<Number>(st.total_payload_download));
        obj->Set(Nan::New("total_payload_upload").ToLocalChecked(), Nan::New<Number>(st.total_payload_upload));

        obj->Set(Nan::New("total_failed_bytes").ToLocalChecked(), Nan::New<Number>(st.total_failed_bytes));
        obj->Set(Nan::New("total_redundant_bytes").ToLocalChecked(), Nan::New<Number>(st.total_redundant_bytes));

        obj->Set(Nan::New("download_rate").ToLocalChecked(), Nan::New<Integer>(st.download_rate));
        obj->Set(Nan::New("upload_rate").ToLocalChecked(), Nan::New<Integer>(st.upload_rate));

        obj->Set(Nan::New("download_payload_rate").ToLocalChecked(), Nan::New<Integer>(st.download_payload_rate));
        obj->Set(Nan::New("upload_payload_rate").ToLocalChecked(), Nan::New<Integer>(st.upload_payload_rate));

        obj->Set(Nan::New("num_peers").ToLocalChecked(), Nan::New<Integer>(st.num_peers));

        obj->Set(Nan::New("num_complete").ToLocalChecked(), Nan::New<Integer>(st.num_complete));
        obj->Set(Nan::New("num_incomplete").ToLocalChecked(), Nan::New<Integer>(st.num_incomplete));

        obj->Set(Nan::New("list_seeds").ToLocalChecked(), Nan::New<Integer>(st.list_seeds));
        obj->Set(Nan::New("list_peers").ToLocalChecked(), Nan::New<Integer>(st.list_peers));

        obj->Set(Nan::New("connect_candidates").ToLocalChecked(), Nan::New<Integer>(st.connect_candidates));

        obj->Set(Nan::New("pieces").ToLocalChecked(), bitfield_to_array(st.pieces));
        obj->Set(Nan::New("verified_pieces").ToLocalChecked(), bitfield_to_array(st.verified_pieces));

        obj->Set(Nan::New("num_pieces").ToLocalChecked(), Nan::New<Integer>(st.num_pieces));

        obj->Set(Nan::New("total_done").ToLocalChecked(), Nan::New<Number>(st.total_done));
        obj->Set(Nan::New("total_wanted_done").ToLocalChecked(), Nan::New<Number>(st.total_wanted_done));
        obj->Set(Nan::New("total_wanted").ToLocalChecked(), Nan::New<Number>(st.total_wanted));

        obj->Set(Nan::New("num_seeds").ToLocalChecked(), Nan::New<Integer>(st.num_seeds));

        obj->Set(Nan::New("distributed_full_copies").ToLocalChecked(), Nan::New<Integer>(st.distributed_full_copies));
        obj->Set(Nan::New("distributed_fraction").ToLocalChecked(), Nan::New<Integer>(st.distributed_fraction));

        obj->Set(Nan::New("distributed_copies").ToLocalChecked(), Nan::New<Number>(st.distributed_copies));

        obj->Set(Nan::New("block_size").ToLocalChecked(), Nan::New<Integer>(st.block_size));

        obj->Set(Nan::New("num_uploads").ToLocalChecked(), Nan::New<Integer>(st.num_uploads));
        obj->Set(Nan::New("num_connections").ToLocalChecked(), Nan::New<Integer>(st.num_connections));
        obj->Set(Nan::New("uploads_limit").ToLocalChecked(), Nan::New<Integer>(st.uploads_limit));
        obj->Set(Nan::New("connections_limit").ToLocalChecked(), Nan::New<Integer>(st.connections_limit));

        obj->Set(Nan::New("storage_mode").ToLocalChecked(), Nan::New<Integer>(st.storage_mode));

        obj->Set(Nan::New("up_bandwidth_queue").ToLocalChecked(), Nan::New<Integer>(st.up_bandwidth_queue));
        obj->Set(Nan::New("down_bandwidth_queue").ToLocalChecked(), Nan::New<Integer>(st.down_bandwidth_queue));

        obj->Set(Nan::New("all_time_upload").ToLocalChecked(), Nan::New<Number>(st.all_time_upload));
        obj->Set(Nan::New("all_time_download").ToLocalChecked(), Nan::New<Number>(st.all_time_download));

        obj->Set(Nan::New("active_time").ToLocalChecked(), Nan::New<Integer>(st.active_time));
        obj->Set(Nan::New("finished_time").ToLocalChecked(), Nan::New<Integer>(st.finished_time));
        obj->Set(Nan::New("seeding_time").ToLocalChecked(), Nan::New<Integer>(st.seeding_time));

        obj->Set(Nan::New("seed_rank").ToLocalChecked(), Nan::New<Integer>(st.seed_rank));

        obj->Set(Nan::New("last_scrape").ToLocalChecked(), Nan::New<Integer>(st.last_scrape));

        obj->Set(Nan::New("has_incoming").ToLocalChecked(), Nan::New<Boolean>(st.has_incoming));

        obj->Set(Nan::New("sparse_regions").ToLocalChecked(), Nan::New<Integer>(st.sparse_regions));

        obj->Set(Nan::New("seed_mode").ToLocalChecked(), Nan::New<Boolean>(st.seed_mode));
        obj->Set(Nan::New("upload_mode").ToLocalChecked(), Nan::New<Boolean>(st.upload_mode));
        obj->Set(Nan::New("share_mode").ToLocalChecked(), Nan::New<Boolean>(st.share_mode));
        obj->Set(Nan::New("super_seeding").ToLocalChecked(), Nan::New<Boolean>(st.super_seeding));

        obj->Set(Nan::New("priority").ToLocalChecked(), Nan::New<Integer>(st.priority));

        obj->Set(Nan::New("added_time").ToLocalChecked(), Nan::New<Date>(((double) st.added_time)*1000).ToLocalChecked());
        obj->Set(Nan::New("completed_time").ToLocalChecked(), Nan::New<Date>(((double) st.completed_time)*1000).ToLocalChecked());
        obj->Set(Nan::New("last_seen_complete").ToLocalChecked(), Nan::New<Date>(((double) st.last_seen_complete)*1000).ToLocalChecked());

        obj->Set(Nan::New("time_since_upload").ToLocalChecked(), Nan::New<Integer>(st.time_since_upload));
        obj->Set(Nan::New("time_since_download").ToLocalChecked(), Nan::New<Integer>(st.time_since_download));

        obj->Set(Nan::New("queue_position").ToLocalChecked(), Nan::New<Integer>(st.queue_position));
        obj->Set(Nan::New("need_save_resume").ToLocalChecked(), Nan::New<Boolean>(st.need_save_resume));
        obj->Set(Nan::New("ip_filter_applies").ToLocalChecked(), Nan::New<Boolean>(st.ip_filter_applies));

        std::string info_hash = libtorrent::to_hex(st.info_hash.to_string());
        obj->Set(Nan::New("info_hash").ToLocalChecked(), Nan::New<String>(info_hash.c_str()).ToLocalChecked());

        //obj->Set(Nan::New("listen_port").ToLocalChecked(), Nan::New<Integer>(st.listen_port));

        return scope.Escape(obj);
    };
}; // namespace nodelt
