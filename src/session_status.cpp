#include <v8.h>
#include <nan.h>
#include <libtorrent/session_status.hpp>

#include "session_status.hpp"

using namespace v8;

namespace nodelt {
#ifndef TORRENT_DISABLE_DHT
    Local<Object> dht_lookup_to_object(const libtorrent::dht_lookup& dht) {
        Nan::EscapableHandleScope scope;
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("type").ToLocalChecked(), Nan::New<String>(dht.type).ToLocalChecked());
        obj->Set(Nan::New("outstanding_requests").ToLocalChecked(), Nan::New<Integer>(dht.outstanding_requests));
        obj->Set(Nan::New("timeouts").ToLocalChecked(), Nan::New<Integer>(dht.timeouts));
        obj->Set(Nan::New("responses").ToLocalChecked(), Nan::New<Integer>(dht.responses));
        obj->Set(Nan::New("branch_factor").ToLocalChecked(), Nan::New<Integer>(dht.branch_factor));
        obj->Set(Nan::New("nodes_left").ToLocalChecked(), Nan::New<Integer>(dht.nodes_left));
        obj->Set(Nan::New("last_sent").ToLocalChecked(), Nan::New<Integer>(dht.last_sent));
        obj->Set(Nan::New("first_timeout").ToLocalChecked(), Nan::New<Integer>(dht.first_timeout));

        return scope.Escape(obj);
    };

    Local<Object> dht_routing_bucket_to_object(const libtorrent::dht_routing_bucket& dht) {
        Nan::EscapableHandleScope scope;
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("num_nodes").ToLocalChecked(), Nan::New<Integer>(dht.num_nodes));
        obj->Set(Nan::New("num_replacements").ToLocalChecked(), Nan::New<Integer>(dht.num_replacements));
        obj->Set(Nan::New("last_active").ToLocalChecked(), Nan::New<Integer>(dht.last_active));

        return scope.Escape(obj);
    };
#endif

    Local<Object> utp_status_to_object(const libtorrent::utp_status& st) {
        Nan::EscapableHandleScope scope;
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("num_idle").ToLocalChecked(), Nan::New<Integer>(st.num_idle));
        obj->Set(Nan::New("num_syn_sent").ToLocalChecked(), Nan::New<Integer>(st.num_syn_sent));
        obj->Set(Nan::New("num_connected").ToLocalChecked(), Nan::New<Integer>(st.num_connected));
        obj->Set(Nan::New("num_fin_sent").ToLocalChecked(), Nan::New<Integer>(st.num_fin_sent));
        obj->Set(Nan::New("num_close_wait").ToLocalChecked(), Nan::New<Integer>(st.num_close_wait));

        return scope.Escape(obj);
    };

    Local<Object> session_status_to_object(const libtorrent::session_status& st) {
        Nan::EscapableHandleScope scope;
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("has_incoming_connections").ToLocalChecked(), Nan::New<Boolean>(st.has_incoming_connections));

        obj->Set(Nan::New("upload_rate").ToLocalChecked(), Nan::New<Integer>(st.upload_rate));
        obj->Set(Nan::New("download_rate").ToLocalChecked(), Nan::New<Integer>(st.upload_rate));
        obj->Set(Nan::New("total_download").ToLocalChecked(), Nan::New<Number>(st.total_download));
        obj->Set(Nan::New("total_upload").ToLocalChecked(), Nan::New<Number>(st.total_upload));

        obj->Set(Nan::New("payload_upload_rate").ToLocalChecked(), Nan::New<Integer>(st.payload_upload_rate));
        obj->Set(Nan::New("payload_download_rate").ToLocalChecked(), Nan::New<Integer>(st.payload_download_rate));
        obj->Set(Nan::New("total_payload_download").ToLocalChecked(), Nan::New<Number>(st.total_payload_download));
        obj->Set(Nan::New("total_payload_upload").ToLocalChecked(), Nan::New<Number>(st.total_payload_upload));

        obj->Set(Nan::New("ip_overhead_upload_rate").ToLocalChecked(), Nan::New<Integer>(st.ip_overhead_upload_rate));
        obj->Set(Nan::New("ip_overhead_download_rate").ToLocalChecked(), Nan::New<Integer>(st.ip_overhead_download_rate));
        obj->Set(Nan::New("total_ip_overhead_download").ToLocalChecked(), Nan::New<Number>(st.total_ip_overhead_download));
        obj->Set(Nan::New("total_ip_overhead_upload").ToLocalChecked(), Nan::New<Number>(st.total_ip_overhead_upload));

        obj->Set(Nan::New("dht_upload_rate").ToLocalChecked(), Nan::New<Integer>(st.dht_upload_rate));
        obj->Set(Nan::New("dht_download_rate").ToLocalChecked(), Nan::New<Integer>(st.dht_download_rate));
        obj->Set(Nan::New("total_dht_download").ToLocalChecked(), Nan::New<Number>(st.total_dht_download));
        obj->Set(Nan::New("total_dht_upload").ToLocalChecked(), Nan::New<Number>(st.total_dht_upload));

        obj->Set(Nan::New("tracker_upload_rate").ToLocalChecked(), Nan::New<Integer>(st.tracker_upload_rate));
        obj->Set(Nan::New("tracker_download_rate").ToLocalChecked(), Nan::New<Integer>(st.tracker_download_rate));
        obj->Set(Nan::New("total_tracker_download").ToLocalChecked(), Nan::New<Number>(st.total_tracker_download));
        obj->Set(Nan::New("total_tracker_upload").ToLocalChecked(), Nan::New<Number>(st.total_tracker_upload));

        obj->Set(Nan::New("total_redundant_bytes").ToLocalChecked(), Nan::New<Number>(st.total_redundant_bytes));
        obj->Set(Nan::New("total_failed_bytes").ToLocalChecked(), Nan::New<Number>(st.total_failed_bytes));

        obj->Set(Nan::New("num_peers").ToLocalChecked(), Nan::New<Integer>(st.num_peers));
        obj->Set(Nan::New("num_unchoked").ToLocalChecked(), Nan::New<Integer>(st.num_unchoked));
        obj->Set(Nan::New("allowed_upload_slots").ToLocalChecked(), Nan::New<Integer>(st.allowed_upload_slots));

        obj->Set(Nan::New("up_bandwidth_queue").ToLocalChecked(), Nan::New<Integer>(st.up_bandwidth_queue));
        obj->Set(Nan::New("down_bandwidth_queue").ToLocalChecked(), Nan::New<Integer>(st.down_bandwidth_queue));

        obj->Set(Nan::New("up_bandwidth_bytes_queue").ToLocalChecked(), Nan::New<Integer>(st.up_bandwidth_bytes_queue));
        obj->Set(Nan::New("down_bandwidth_bytes_queue").ToLocalChecked(), Nan::New<Integer>(st.down_bandwidth_bytes_queue));

        obj->Set(Nan::New("optimistic_unchoke_counter").ToLocalChecked(), Nan::New<Integer>(st.optimistic_unchoke_counter));
        obj->Set(Nan::New("unchoke_counter").ToLocalChecked(), Nan::New<Integer>(st.unchoke_counter));

        obj->Set(Nan::New("disk_write_queue").ToLocalChecked(), Nan::New<Integer>(st.disk_write_queue));
        obj->Set(Nan::New("disk_read_queue").ToLocalChecked(), Nan::New<Integer>(st.disk_read_queue));

#ifndef TORRENT_DISABLE_DHT
        obj->Set(Nan::New("dht_nodes").ToLocalChecked(), Nan::New<Integer>(st.dht_nodes));
        obj->Set(Nan::New("dht_node_cache").ToLocalChecked(), Nan::New<Integer>(st.dht_node_cache));
        obj->Set(Nan::New("dht_torrents").ToLocalChecked(), Nan::New<Integer>(st.dht_torrents));
        obj->Set(Nan::New("dht_global_nodes").ToLocalChecked(), Nan::New<Number>(st.dht_global_nodes));

        Local<Array> active_requests = Nan::New<Array>();

        for (std::vector<libtorrent::dht_lookup>::const_iterator i(st.active_requests.begin()), e(st.active_requests.end()); i != e; ++i)
            active_requests->Set(active_requests->Length(), dht_lookup_to_object(*i));

        obj->Set(Nan::New("active_requests").ToLocalChecked(), active_requests);

        Local<Array> dht_routing_table = Nan::New<Array>();

        for (std::vector<libtorrent::dht_routing_bucket>::const_iterator i(st.dht_routing_table.begin()), e(st.dht_routing_table.end()); i != e; ++i)
            dht_routing_table->Set(active_requests->Length(), dht_routing_bucket_to_object(*i));

        obj->Set(Nan::New("dht_routing_table").ToLocalChecked(), dht_routing_table);
        obj->Set(Nan::New("dht_total_allocations").ToLocalChecked(), Nan::New<Integer>(st.dht_total_allocations));
#endif

        obj->Set(Nan::New("utp_stats").ToLocalChecked(), utp_status_to_object(st.utp_stats));
        obj->Set(Nan::New("peerlist_size").ToLocalChecked(), Nan::New<Integer>(st.peerlist_size));

        return scope.Escape(obj);
    };
}; // namespace nodelt
