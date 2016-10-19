#include <v8.h>
#include <nan.h>
#include <node.h>

#include <libtorrent/peer_info.hpp>

#include "peer_info.hpp"

using namespace v8;
using namespace node;

namespace nodelt {
    void bind_peer_info(Local<Object> target) {
        // set flags
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("interesting").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::interesting));
        obj->Set(Nan::New("choked").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::choked));
        obj->Set(Nan::New("remote_interested").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::remote_interested));
        obj->Set(Nan::New("remote_choked").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::remote_choked));
        obj->Set(Nan::New("supports_extensions").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::supports_extensions));
        obj->Set(Nan::New("local_connection").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::local_connection));
        obj->Set(Nan::New("handshake").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::handshake));
        obj->Set(Nan::New("connecting").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::connecting));
        obj->Set(Nan::New("on_parole").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::on_parole));
        obj->Set(Nan::New("seed").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::seed));
        obj->Set(Nan::New("optimistic_unchoke").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::optimistic_unchoke));
        obj->Set(Nan::New("snubbed").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::snubbed));
        obj->Set(Nan::New("upload_only").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::upload_only));
        obj->Set(Nan::New("endgame_mode").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::endgame_mode));
        obj->Set(Nan::New("holepunched").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::holepunched));

#ifndef TORRENT_DISABLE_ENCRYPTION
        obj->Set(Nan::New("rc4_encrypted").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::rc4_encrypted));
        obj->Set(Nan::New("plaintext_encrypted").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::plaintext_encrypted));
#endif

        obj->Set(Nan::New("standard_bittorrent").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::standard_bittorrent));
        obj->Set(Nan::New("web_seed").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::web_seed));

        obj->Set(Nan::New("tracker").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::tracker));
        obj->Set(Nan::New("dht").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::dht));
        obj->Set(Nan::New("pex").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::pex));
        obj->Set(Nan::New("lsd").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::lsd));
        obj->Set(Nan::New("resume_data").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::resume_data));
        obj->Set(Nan::New("incoming").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::incoming));

        obj->Set(Nan::New("bw_idle").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::bw_idle));
        obj->Set(Nan::New("bw_limit").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::bw_limit));
        obj->Set(Nan::New("bw_network").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::bw_network));
        obj->Set(Nan::New("bw_disk").ToLocalChecked(), Nan::New<Integer>(libtorrent::peer_info::bw_disk));

        target->Set(Nan::New("peer_info").ToLocalChecked(), obj);
    };

    Local<Array> get_tcp_endpoint(const libtorrent::tcp::endpoint& ep) {
        Nan::EscapableHandleScope scope;

        Local<Array> ret = Nan::New<Array>();

        ret->Set(0, Nan::New<String>(ep.address().to_string()).ToLocalChecked());
        ret->Set(1, Nan::New<Integer>(ep.port()));

        return scope.Escape(ret);
    };

    Local<Array> get_pieces(const libtorrent::peer_info& pi) {
        Nan::EscapableHandleScope scope;

        Local<Array> ret = Nan::New<Array>();

        for (libtorrent::bitfield::const_iterator i(pi.pieces.begin()), e(pi.pieces.end()); i != e; ++i)
            ret->Set(ret->Length(), Nan::New<Integer>(*i));

        return scope.Escape(ret);
    };

    Local<Object> peer_info_to_object(const libtorrent::peer_info& pi) {
        Nan::EscapableHandleScope scope;

        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("falgs").ToLocalChecked(), Nan::New<Uint32>(pi.flags));
        obj->Set(Nan::New("source").ToLocalChecked(), Nan::New<Integer>(pi.source));
        obj->Set(Nan::New("read_state").ToLocalChecked(), Nan::New<Integer>(pi.read_state));
        obj->Set(Nan::New("write_state").ToLocalChecked(), Nan::New<Integer>(pi.write_state));
        obj->Set(Nan::New("ip").ToLocalChecked(), get_tcp_endpoint(pi.ip));
        obj->Set(Nan::New("up_speed").ToLocalChecked(), Nan::New<Integer>(pi.up_speed));
        obj->Set(Nan::New("down_speed").ToLocalChecked(), Nan::New<Integer>(pi.down_speed));
        obj->Set(Nan::New("payload_up_speed").ToLocalChecked(), Nan::New<Integer>(pi.payload_up_speed));
        obj->Set(Nan::New("payload_down_speed").ToLocalChecked(), Nan::New<Integer>(pi.payload_down_speed));
        obj->Set(Nan::New("total_download").ToLocalChecked(), Nan::New<Number>((int32_t)pi.total_download));
        obj->Set(Nan::New("total_upload").ToLocalChecked(), Nan::New<Number>(pi.total_upload));
        obj->Set(Nan::New("pid").ToLocalChecked(), Nan::New<String>(libtorrent::to_hex(pi.pid.to_string())).ToLocalChecked());
        obj->Set(Nan::New("pieces").ToLocalChecked(), get_pieces(pi));
        obj->Set(Nan::New("upload_limit").ToLocalChecked(), Nan::New<Integer>(pi.upload_limit));
        obj->Set(Nan::New("download_limit").ToLocalChecked(), Nan::New<Integer>(pi.download_limit));
        obj->Set(Nan::New("last_request").ToLocalChecked(), Nan::New<Integer>(libtorrent::total_seconds(pi.last_request)));
        obj->Set(Nan::New("last_active").ToLocalChecked(), Nan::New<Integer>(libtorrent::total_seconds(pi.last_active)));
        obj->Set(Nan::New("download_queue_time").ToLocalChecked(), Nan::New<Integer>(libtorrent::total_seconds(pi.download_queue_time)));
        obj->Set(Nan::New("queue_bytes").ToLocalChecked(), Nan::New<Integer>(pi.queue_bytes));
        obj->Set(Nan::New("request_timeout").ToLocalChecked(), Nan::New<Integer>(pi.request_timeout));
        obj->Set(Nan::New("send_buffer_size").ToLocalChecked(), Nan::New<Integer>(pi.send_buffer_size));
        obj->Set(Nan::New("used_send_buffer").ToLocalChecked(), Nan::New<Integer>(pi.used_send_buffer));
        obj->Set(Nan::New("receive_buffer_size").ToLocalChecked(), Nan::New<Integer>(pi.receive_buffer_size));
        obj->Set(Nan::New("used_receive_buffer").ToLocalChecked(), Nan::New<Integer>(pi.used_receive_buffer));
        obj->Set(Nan::New("num_hashfails").ToLocalChecked(), Nan::New<Integer>(pi.num_hashfails));

#ifndef TORRENT_DISABLE_RESOLVE_COUNTRIES
        obj->Set(Nan::New("country").ToLocalChecked(), Nan::New<String>(pi.country).ToLocalChecked());
#endif
#ifndef TORRENT_DISABLE_GEO_IP
        obj->Set(Nan::New("inet_as_name").ToLocalChecked(), Nan::New<String>(pi.inet_as_name).ToLocalChecked());
        obj->Set(Nan::New("inet_as").ToLocalChecked(), Nan::New<Integer>(pi.inet_as));
#endif
        // obj->Set(Nan::New("load_balancing").ToLocalChecked(), Nan::New<Integer>(pi.load_balancing));
        obj->Set(Nan::New("download_queue_length").ToLocalChecked(), Nan::New<Integer>(pi.download_queue_length));
        obj->Set(Nan::New("timed_out_requests").ToLocalChecked(), Nan::New<Integer>(pi.timed_out_requests));
        obj->Set(Nan::New("busy_requests").ToLocalChecked(), Nan::New<Integer>(pi.busy_requests));
        obj->Set(Nan::New("requests_in_buffer").ToLocalChecked(), Nan::New<Integer>(pi.requests_in_buffer));
        obj->Set(Nan::New("target_dl_queue_length").ToLocalChecked(), Nan::New<Integer>(pi.target_dl_queue_length));
        obj->Set(Nan::New("upload_queue_length").ToLocalChecked(), Nan::New<Integer>(pi.upload_queue_length));
        obj->Set(Nan::New("failcount").ToLocalChecked(), Nan::New<Integer>(pi.failcount));
        obj->Set(Nan::New("downloading_piece_index").ToLocalChecked(), Nan::New<Integer>(pi.downloading_piece_index));
        obj->Set(Nan::New("downloading_block_index").ToLocalChecked(), Nan::New<Integer>(pi.downloading_block_index));
        obj->Set(Nan::New("downloading_progress").ToLocalChecked(), Nan::New<Integer>(pi.downloading_progress));
        obj->Set(Nan::New("downloading_total").ToLocalChecked(), Nan::New<Integer>(pi.downloading_total));
        obj->Set(Nan::New("client").ToLocalChecked(), Nan::New<String>(pi.client).ToLocalChecked());
        obj->Set(Nan::New("connection_type").ToLocalChecked(), Nan::New<Integer>(pi.connection_type));
        obj->Set(Nan::New("remote_dl_rate").ToLocalChecked(), Nan::New<Integer>(pi.remote_dl_rate));
        obj->Set(Nan::New("pending_disk_bytes").ToLocalChecked(), Nan::New<Integer>(pi.pending_disk_bytes));
        obj->Set(Nan::New("send_quota").ToLocalChecked(), Nan::New<Integer>(pi.send_quota));
        obj->Set(Nan::New("receive_quota").ToLocalChecked(), Nan::New<Integer>(pi.receive_quota));
        obj->Set(Nan::New("rtt").ToLocalChecked(), Nan::New<Integer>(pi.rtt));
        obj->Set(Nan::New("num_pieces").ToLocalChecked(), Nan::New<Integer>(pi.num_pieces));
        obj->Set(Nan::New("download_rate_peak").ToLocalChecked(), Nan::New<Integer>(pi.download_rate_peak));
        obj->Set(Nan::New("upload_rate_peak").ToLocalChecked(), Nan::New<Integer>(pi.upload_rate_peak));
        obj->Set(Nan::New("progress").ToLocalChecked(), Nan::New<Number>(pi.progress));
        obj->Set(Nan::New("progress_ppm").ToLocalChecked(), Nan::New<Integer>(pi.progress_ppm));
        obj->Set(Nan::New("estimated_reciprocation_rate").ToLocalChecked(), Nan::New<Integer>(pi.estimated_reciprocation_rate));
        obj->Set(Nan::New("local_endpoint").ToLocalChecked(), get_tcp_endpoint(pi.local_endpoint));

        return scope.Escape(obj);
    }
}; // namespace nodelt
