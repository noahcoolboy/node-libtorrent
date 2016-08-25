#include <v8.h>
#include <nan.h>

#include <libtorrent/session_settings.hpp>

#include "session_settings.hpp"

using namespace v8;

namespace nodelt {
    void bind_session_settings(Local<Object> target) {
        // set libtorrent::session_settings::choking_algorithm_t
        Local<Object> choking_algorithm_t = Nan::New<Object>();

        choking_algorithm_t->Set(Nan::New("fixed_slots_choker").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::fixed_slots_choker));
        choking_algorithm_t->Set(Nan::New("auto_expand_choker").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::auto_expand_choker));
        choking_algorithm_t->Set(Nan::New("rate_based_choker").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::rate_based_choker));
        choking_algorithm_t->Set(Nan::New("bittyrant_choker").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::bittyrant_choker));
        target->Set(Nan::New("choking_algorithm_t").ToLocalChecked(), choking_algorithm_t);

        // set libtorrent::session_settings::seed_choking_algorithm_t
        Local<Object> seed_choking_algorithm_t = Nan::New<Object>();

        seed_choking_algorithm_t->Set(Nan::New("round_robin").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::round_robin));
        seed_choking_algorithm_t->Set(Nan::New("fastest_upload").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::fastest_upload));
        seed_choking_algorithm_t->Set(Nan::New("anti_leech").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::anti_leech));
        target->Set(Nan::New("seed_choking_algorithm_t").ToLocalChecked(), seed_choking_algorithm_t);

        // set libtorrent::session_settings::io_buffer_mode_t
        Local<Object> io_buffer_mode_t = Nan::New<Object>();

        io_buffer_mode_t->Set(Nan::New("enable_os_cache").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::enable_os_cache));
        io_buffer_mode_t->Set(Nan::New("disable_os_cache_for_aligned_files").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::disable_os_cache_for_aligned_files));
        io_buffer_mode_t->Set(Nan::New("disable_os_cache").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::disable_os_cache));
        target->Set(Nan::New("io_buffer_mode_t").ToLocalChecked(), io_buffer_mode_t);

        // set libtorrent::session_settings::disk_cache_algo_t
        Local<Object> disk_cache_algo_t = Nan::New<Object>();

        disk_cache_algo_t->Set(Nan::New("lru").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::lru));
        disk_cache_algo_t->Set(Nan::New("largest_contiguous").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::largest_contiguous));
        disk_cache_algo_t->Set(Nan::New("avoid_readback").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::avoid_readback));
        target->Set(Nan::New("disk_cache_algo_t").ToLocalChecked(), disk_cache_algo_t);

        // set libtorrent::session_settings::suggest_mode_t
        Local<Object> suggest_mode_t = Nan::New<Object>();

        suggest_mode_t->Set(Nan::New("no_piece_suggestions").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::no_piece_suggestions));
        suggest_mode_t->Set(Nan::New("suggest_read_cache").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::suggest_read_cache));
        target->Set(Nan::New("suggest_mode_t").ToLocalChecked(), suggest_mode_t);

        // set libtorrent::session_settings::suggest_mode_t
        Local<Object> bandwidth_mixed_algo_t = Nan::New<Object>();

        bandwidth_mixed_algo_t->Set(Nan::New("prefer_tcp").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::prefer_tcp));
        bandwidth_mixed_algo_t->Set(Nan::New("peer_proportional").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::session_settings::peer_proportional));
        target->Set(Nan::New("bandwidth_mixed_algo_t").ToLocalChecked(), bandwidth_mixed_algo_t);

        // set libtorrent::proxy_settings::proxy_type
        Local<Object> proxy_type = Nan::New<Object>();

        proxy_type->Set(Nan::New("none").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::proxy_settings::none));
        proxy_type->Set(Nan::New("socks4").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::proxy_settings::socks4));
        proxy_type->Set(Nan::New("socks5").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::proxy_settings::socks5));
        proxy_type->Set(Nan::New("socks5_pw").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::proxy_settings::socks5_pw));
        proxy_type->Set(Nan::New("http").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::proxy_settings::http));
        proxy_type->Set(Nan::New("http_pw").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::proxy_settings::http_pw));
        proxy_type->Set(Nan::New("i2p_proxy").ToLocalChecked(),
            Nan::New<Integer>(libtorrent::proxy_settings::i2p_proxy));
        target->Set(Nan::New("proxy_type").ToLocalChecked(), proxy_type);
    };

#ifndef TORRENT_DISABLE_DHT
    libtorrent::dht_settings const dht_settings_from_object(Local<Object> obj) {
        libtorrent::dht_settings dht;

        if (Nan::Has(obj, Nan::New("max_peers_reply").ToLocalChecked()) == Nan::Just(true))
            dht.max_peers_reply = (Nan::Get(obj, Nan::New("max_peers_reply").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("search_branching").ToLocalChecked()) == Nan::Just(true))
            dht.search_branching = (Nan::Get(obj, Nan::New("search_branching").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_fail_count").ToLocalChecked()) == Nan::Just(true))
            dht.max_fail_count = (Nan::Get(obj, Nan::New("max_fail_count").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_torrents").ToLocalChecked()) == Nan::Just(true))
            dht.max_torrents = (Nan::Get(obj, Nan::New("max_torrents").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_dht_items").ToLocalChecked()) == Nan::Just(true))
            dht.max_dht_items = (Nan::Get(obj, Nan::New("max_dht_items").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_torrent_search_reply").ToLocalChecked()) == Nan::Just(true))
            dht.max_torrent_search_reply = (Nan::Get(obj, Nan::New("max_torrent_search_reply").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("restrict_routing_ips").ToLocalChecked()) == Nan::Just(true))
            dht.restrict_routing_ips = (Nan::Get(obj, Nan::New("restrict_routing_ips").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("restrict_search_ips").ToLocalChecked()) == Nan::Just(true))
            dht.restrict_search_ips = (Nan::Get(obj, Nan::New("restrict_search_ips").ToLocalChecked()).ToLocalChecked())->BooleanValue();

        return dht;
    };
#endif

    Local<Object> proxy_settings_to_object(const libtorrent::proxy_settings& s) {
        Nan::EscapableHandleScope scope;
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("hostname").ToLocalChecked(), Nan::New<String>(s.hostname).ToLocalChecked());
        obj->Set(Nan::New("port").ToLocalChecked(), Nan::New<Integer>(s.port));

        obj->Set(Nan::New("username").ToLocalChecked(), Nan::New<String>(s.username).ToLocalChecked());
        obj->Set(Nan::New("password").ToLocalChecked(), Nan::New<String>(s.password).ToLocalChecked());

        obj->Set(Nan::New("type").ToLocalChecked(), Nan::New<Integer>(s.type));

        obj->Set(Nan::New("proxy_hostnames").ToLocalChecked(), Nan::New<Integer>(s.proxy_hostnames));
        obj->Set(Nan::New("proxy_peer_connections").ToLocalChecked(), Nan::New<Integer>(s.proxy_peer_connections));

        return scope.Escape(obj);
    };

    libtorrent::proxy_settings proxy_settings_from_object(Local<Object> obj) {
        libtorrent::proxy_settings s;

        if (Nan::Has(obj, Nan::New("hostname").ToLocalChecked()) == Nan::Just(true)) {
            Local<Object> hostname = (Nan::Get(obj, Nan::New("hostname").ToLocalChecked()).ToLocalChecked()).As<Object>();
            s.hostname = std::string(*Nan::Utf8String(hostname));
        }
        if (Nan::Has(obj, Nan::New("port").ToLocalChecked()) == Nan::Just(true))
            s.port = (Nan::Get(obj, Nan::New("port").ToLocalChecked()).ToLocalChecked())->IntegerValue();

        if (Nan::Has(obj, Nan::New("username").ToLocalChecked()) == Nan::Just(true)) {
            Local<Object> username = (Nan::Get(obj, Nan::New("username").ToLocalChecked()).ToLocalChecked()).As<Object>();
            s.username = std::string(*Nan::Utf8String(username));
        }
        if (Nan::Has(obj, Nan::New("password").ToLocalChecked()) == Nan::Just(true)) {
            Local<Object> password = (Nan::Get(obj, Nan::New("password").ToLocalChecked()).ToLocalChecked()).As<Object>();
            s.password = std::string(*Nan::Utf8String(password));
        }

        if (Nan::Has(obj, Nan::New("type").ToLocalChecked()) == Nan::Just(true))
            s.type = (libtorrent::proxy_settings::proxy_type) (Nan::Get(obj, Nan::New("type").ToLocalChecked()).ToLocalChecked())->IntegerValue();

        if (Nan::Has(obj, Nan::New("proxy_hostnames").ToLocalChecked()) == Nan::Just(true))
            s.proxy_hostnames = (Nan::Get(obj, Nan::New("proxy_hostnames").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("proxy_peer_connections").ToLocalChecked()) == Nan::Just(true))
            s.proxy_peer_connections = (Nan::Get(obj, Nan::New("proxy_peer_connections").ToLocalChecked()).ToLocalChecked())->BooleanValue();

        return s;
    };

    Local<Object> session_settings_to_object(const libtorrent::session_settings& s) {
        Nan::EscapableHandleScope scope;
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("version").ToLocalChecked(), Nan::New<Integer>(s.version));
        obj->Set(Nan::New("user_agent").ToLocalChecked(), Nan::New<String>(s.user_agent).ToLocalChecked());
        obj->Set(Nan::New("tracker_completion_timeout").ToLocalChecked(), Nan::New<Integer>(s.tracker_completion_timeout));
        obj->Set(Nan::New("tracker_receive_timeout").ToLocalChecked(), Nan::New<Integer>(s.tracker_receive_timeout));
        obj->Set(Nan::New("stop_tracker_timeout").ToLocalChecked(), Nan::New<Integer>(s.stop_tracker_timeout));
        obj->Set(Nan::New("tracker_maximum_response_length").ToLocalChecked(), Nan::New<Integer>(s.tracker_maximum_response_length));
        obj->Set(Nan::New("piece_timeout").ToLocalChecked(), Nan::New<Integer>(s.piece_timeout));
        obj->Set(Nan::New("request_timeout").ToLocalChecked(), Nan::New<Integer>(s.request_timeout));
        obj->Set(Nan::New("request_queue_time").ToLocalChecked(), Nan::New<Integer>(s.request_queue_time));
        obj->Set(Nan::New("max_allowed_in_request_queue").ToLocalChecked(), Nan::New<Integer>(s.max_allowed_in_request_queue));
        obj->Set(Nan::New("max_out_request_queue").ToLocalChecked(), Nan::New<Integer>(s.max_out_request_queue));
        obj->Set(Nan::New("whole_pieces_threshold").ToLocalChecked(), Nan::New<Integer>(s.whole_pieces_threshold));
        obj->Set(Nan::New("peer_timeout").ToLocalChecked(), Nan::New<Integer>(s.peer_timeout));
        obj->Set(Nan::New("urlseed_timeout").ToLocalChecked(), Nan::New<Integer>(s.urlseed_timeout));
        obj->Set(Nan::New("urlseed_pipeline_size").ToLocalChecked(), Nan::New<Integer>(s.urlseed_pipeline_size));
        obj->Set(Nan::New("urlseed_wait_retry").ToLocalChecked(), Nan::New<Integer>(s.urlseed_wait_retry));
        obj->Set(Nan::New("file_pool_size").ToLocalChecked(), Nan::New<Integer>(s.file_pool_size));
        obj->Set(Nan::New("allow_multiple_connections_per_ip").ToLocalChecked(), Nan::New<Boolean>(s.allow_multiple_connections_per_ip));
        obj->Set(Nan::New("max_failcount").ToLocalChecked(), Nan::New<Integer>(s.max_failcount));
        obj->Set(Nan::New("min_reconnect_time").ToLocalChecked(), Nan::New<Integer>(s.min_reconnect_time));
        obj->Set(Nan::New("peer_connect_timeout").ToLocalChecked(), Nan::New<Integer>(s.peer_connect_timeout));
        obj->Set(Nan::New("ignore_limits_on_local_network").ToLocalChecked(), Nan::New<Boolean>(s.ignore_limits_on_local_network));
        obj->Set(Nan::New("connection_speed").ToLocalChecked(), Nan::New<Integer>(s.connection_speed));
        obj->Set(Nan::New("send_redundant_have").ToLocalChecked(), Nan::New<Boolean>(s.send_redundant_have));
        obj->Set(Nan::New("lazy_bitfields").ToLocalChecked(), Nan::New<Boolean>(s.lazy_bitfields));
        obj->Set(Nan::New("inactivity_timeout").ToLocalChecked(), Nan::New<Integer>(s.inactivity_timeout));
        obj->Set(Nan::New("unchoke_interval").ToLocalChecked(), Nan::New<Integer>(s.unchoke_interval));
        obj->Set(Nan::New("optimistic_unchoke_interval").ToLocalChecked(), Nan::New<Integer>(s.optimistic_unchoke_interval));
        obj->Set(Nan::New("announce_ip").ToLocalChecked(), Nan::New<String>(s.announce_ip).ToLocalChecked());
        obj->Set(Nan::New("num_want").ToLocalChecked(), Nan::New<Integer>(s.num_want));
        obj->Set(Nan::New("initial_picker_threshold").ToLocalChecked(), Nan::New<Integer>(s.initial_picker_threshold));
        obj->Set(Nan::New("allowed_fast_set_size").ToLocalChecked(), Nan::New<Integer>(s.allowed_fast_set_size));
        obj->Set(Nan::New("suggest_mode").ToLocalChecked(), Nan::New<Integer>(s.suggest_mode));
        obj->Set(Nan::New("max_queued_disk_bytes").ToLocalChecked(), Nan::New<Integer>(s.max_queued_disk_bytes));
        obj->Set(Nan::New("max_queued_disk_bytes_low_watermark").ToLocalChecked(), Nan::New<Integer>(s.max_queued_disk_bytes_low_watermark));
        obj->Set(Nan::New("handshake_timeout").ToLocalChecked(), Nan::New<Integer>(s.handshake_timeout));

#ifndef TORRENT_DISABLE_DHT
        obj->Set(Nan::New("use_dht_as_fallback").ToLocalChecked(), Nan::New<Boolean>(s.use_dht_as_fallback));
#endif

        obj->Set(Nan::New("free_torrent_hashes").ToLocalChecked(), Nan::New<Boolean>(s.free_torrent_hashes));
        obj->Set(Nan::New("upnp_ignore_nonrouters").ToLocalChecked(), Nan::New<Boolean>(s.upnp_ignore_nonrouters));
        obj->Set(Nan::New("send_buffer_low_watermark").ToLocalChecked(), Nan::New<Integer>(s.send_buffer_low_watermark));
        obj->Set(Nan::New("send_buffer_watermark").ToLocalChecked(), Nan::New<Integer>(s.send_buffer_watermark));
        obj->Set(Nan::New("send_buffer_watermark_factor").ToLocalChecked(), Nan::New<Integer>(s.send_buffer_watermark_factor));
        obj->Set(Nan::New("choking_algorithm").ToLocalChecked(), Nan::New<Integer>(s.choking_algorithm));
        obj->Set(Nan::New("seed_choking_algorithm").ToLocalChecked(), Nan::New<Integer>(s.seed_choking_algorithm));
        obj->Set(Nan::New("use_parole_mode").ToLocalChecked(), Nan::New<Boolean>(s.use_parole_mode));
        obj->Set(Nan::New("cache_size").ToLocalChecked(), Nan::New<Integer>(s.cache_size));
        obj->Set(Nan::New("cache_buffer_chunk_size").ToLocalChecked(), Nan::New<Integer>(s.cache_buffer_chunk_size));
        obj->Set(Nan::New("cache_expiry").ToLocalChecked(), Nan::New<Integer>(s.cache_expiry));
        obj->Set(Nan::New("use_read_cache").ToLocalChecked(), Nan::New<Boolean>(s.use_read_cache));
        obj->Set(Nan::New("explicit_read_cache").ToLocalChecked(), Nan::New<Boolean>(s.explicit_read_cache));
        obj->Set(Nan::New("explicit_cache_interval").ToLocalChecked(), Nan::New<Integer>(s.explicit_cache_interval));
        obj->Set(Nan::New("disk_io_write_mode").ToLocalChecked(), Nan::New<Integer>(s.disk_io_write_mode));
        obj->Set(Nan::New("disk_io_read_mode").ToLocalChecked(), Nan::New<Integer>(s.disk_io_read_mode));
        obj->Set(Nan::New("coalesce_reads").ToLocalChecked(), Nan::New<Boolean>(s.coalesce_reads));
        obj->Set(Nan::New("coalesce_writes").ToLocalChecked(), Nan::New<Boolean>(s.coalesce_writes));

        Local<Array> outgoing_ports = Nan::New<Array>();
        outgoing_ports->Set(0, Nan::New<Integer>(s.outgoing_ports.first));
        outgoing_ports->Set(1, Nan::New<Integer>(s.outgoing_ports.second));
        obj->Set(Nan::New("outgoing_ports").ToLocalChecked(), outgoing_ports);

        obj->Set(Nan::New("peer_tos").ToLocalChecked(), Nan::New<String>(&s.peer_tos).ToLocalChecked());
        obj->Set(Nan::New("active_downloads").ToLocalChecked(), Nan::New<Integer>(s.active_downloads));
        obj->Set(Nan::New("active_seeds").ToLocalChecked(), Nan::New<Integer>(s.active_seeds));
        obj->Set(Nan::New("active_dht_limit").ToLocalChecked(), Nan::New<Integer>(s.active_dht_limit));
        obj->Set(Nan::New("active_tracker_limit").ToLocalChecked(), Nan::New<Integer>(s.active_tracker_limit));
        obj->Set(Nan::New("active_lsd_limit").ToLocalChecked(), Nan::New<Integer>(s.active_lsd_limit));
        obj->Set(Nan::New("active_limit").ToLocalChecked(), Nan::New<Integer>(s.active_limit));
        obj->Set(Nan::New("auto_manage_prefer_seeds").ToLocalChecked(), Nan::New<Boolean>(s.auto_manage_prefer_seeds));
        obj->Set(Nan::New("dont_count_slow_torrents").ToLocalChecked(), Nan::New<Boolean>(s.dont_count_slow_torrents));
        obj->Set(Nan::New("auto_manage_interval").ToLocalChecked(), Nan::New<Integer>(s.auto_manage_interval));
        obj->Set(Nan::New("share_ratio_limit").ToLocalChecked(), Nan::New<Number>(s.share_ratio_limit));
        obj->Set(Nan::New("seed_time_ratio_limit").ToLocalChecked(), Nan::New<Number>(s.seed_time_ratio_limit));
        obj->Set(Nan::New("seed_time_limit").ToLocalChecked(), Nan::New<Integer>(s.seed_time_limit));
        obj->Set(Nan::New("peer_turnover_interval").ToLocalChecked(), Nan::New<Integer>(s.peer_turnover_interval));
        obj->Set(Nan::New("peer_turnover").ToLocalChecked(), Nan::New<Number>(s.peer_turnover));
        obj->Set(Nan::New("peer_turnover_cutoff").ToLocalChecked(), Nan::New<Number>(s.peer_turnover_cutoff));
        obj->Set(Nan::New("close_redundant_connections").ToLocalChecked(), Nan::New<Boolean>(s.close_redundant_connections));
        obj->Set(Nan::New("auto_scrape_interval").ToLocalChecked(), Nan::New<Integer>(s.auto_scrape_interval));
        obj->Set(Nan::New("auto_scrape_min_interval").ToLocalChecked(), Nan::New<Integer>(s.auto_scrape_min_interval));
        obj->Set(Nan::New("max_peerlist_size").ToLocalChecked(), Nan::New<Integer>(s.max_peerlist_size));
        obj->Set(Nan::New("max_paused_peerlist_size").ToLocalChecked(), Nan::New<Integer>(s.max_paused_peerlist_size));
        obj->Set(Nan::New("min_announce_interval").ToLocalChecked(), Nan::New<Integer>(s.min_announce_interval));
        obj->Set(Nan::New("prioritize_partial_pieces").ToLocalChecked(), Nan::New<Boolean>(s.prioritize_partial_pieces));
        obj->Set(Nan::New("auto_manage_startup").ToLocalChecked(), Nan::New<Integer>(s.auto_manage_startup));
        obj->Set(Nan::New("rate_limit_ip_overhead").ToLocalChecked(), Nan::New<Boolean>(s.rate_limit_ip_overhead));
        obj->Set(Nan::New("announce_to_all_trackers").ToLocalChecked(), Nan::New<Boolean>(s.announce_to_all_trackers));
        obj->Set(Nan::New("announce_to_all_tiers").ToLocalChecked(), Nan::New<Boolean>(s.announce_to_all_tiers));
        obj->Set(Nan::New("prefer_udp_trackers").ToLocalChecked(), Nan::New<Boolean>(s.prefer_udp_trackers));
        obj->Set(Nan::New("strict_super_seeding").ToLocalChecked(), Nan::New<Boolean>(s.strict_super_seeding));
        obj->Set(Nan::New("seeding_piece_quota").ToLocalChecked(), Nan::New<Integer>(s.seeding_piece_quota));
        obj->Set(Nan::New("max_sparse_regions").ToLocalChecked(), Nan::New<Integer>(s.max_sparse_regions));

#ifndef TORRENT_DISABLE_MLOCK
        obj->Set(Nan::New("lock_disk_cache").ToLocalChecked(), Nan::New<Boolean>(s.lock_disk_cache));
#endif

        obj->Set(Nan::New("max_rejects").ToLocalChecked(), Nan::New<Integer>(s.max_rejects));
        obj->Set(Nan::New("recv_socket_buffer_size").ToLocalChecked(), Nan::New<Integer>(s.recv_socket_buffer_size));
        obj->Set(Nan::New("send_socket_buffer_size").ToLocalChecked(), Nan::New<Integer>(s.send_socket_buffer_size));
        obj->Set(Nan::New("optimize_hashing_for_speed").ToLocalChecked(), Nan::New<Boolean>(s.optimize_hashing_for_speed));
        obj->Set(Nan::New("file_checks_delay_per_block").ToLocalChecked(), Nan::New<Integer>(s.file_checks_delay_per_block));
        obj->Set(Nan::New("disk_cache_algorithm").ToLocalChecked(), Nan::New<Integer>(s.disk_cache_algorithm));
        obj->Set(Nan::New("read_cache_line_size").ToLocalChecked(), Nan::New<Integer>(s.read_cache_line_size));
        obj->Set(Nan::New("write_cache_line_size").ToLocalChecked(), Nan::New<Integer>(s.write_cache_line_size));
        obj->Set(Nan::New("optimistic_disk_retry").ToLocalChecked(), Nan::New<Integer>(s.optimistic_disk_retry));
        obj->Set(Nan::New("disable_hash_checks").ToLocalChecked(), Nan::New<Boolean>(s.disable_hash_checks));
        obj->Set(Nan::New("allow_reordered_disk_operations").ToLocalChecked(), Nan::New<Boolean>(s.allow_reordered_disk_operations));
        obj->Set(Nan::New("allow_i2p_mixed").ToLocalChecked(), Nan::New<Boolean>(s.allow_i2p_mixed));
        obj->Set(Nan::New("max_suggest_pieces").ToLocalChecked(), Nan::New<Integer>(s.max_suggest_pieces));
        obj->Set(Nan::New("drop_skipped_requests").ToLocalChecked(), Nan::New<Boolean>(s.drop_skipped_requests));
        obj->Set(Nan::New("low_prio_disk").ToLocalChecked(), Nan::New<Boolean>(s.low_prio_disk));
        obj->Set(Nan::New("local_service_announce_interval").ToLocalChecked(), Nan::New<Integer>(s.local_service_announce_interval));
        obj->Set(Nan::New("dht_announce_interval").ToLocalChecked(), Nan::New<Integer>(s.dht_announce_interval));
        obj->Set(Nan::New("udp_tracker_token_expiry").ToLocalChecked(), Nan::New<Integer>(s.udp_tracker_token_expiry));
        obj->Set(Nan::New("volatile_read_cache").ToLocalChecked(), Nan::New<Boolean>(s.volatile_read_cache));
        obj->Set(Nan::New("guided_read_cache").ToLocalChecked(), Nan::New<Boolean>(s.guided_read_cache));
        obj->Set(Nan::New("default_cache_min_age").ToLocalChecked(), Nan::New<Integer>(s.default_cache_min_age));
        obj->Set(Nan::New("num_optimistic_unchoke_slots").ToLocalChecked(), Nan::New<Integer>(s.num_optimistic_unchoke_slots));
        obj->Set(Nan::New("no_atime_storage").ToLocalChecked(), Nan::New<Boolean>(s.no_atime_storage));
        obj->Set(Nan::New("default_est_reciprocation_rate").ToLocalChecked(), Nan::New<Integer>(s.default_est_reciprocation_rate));
        obj->Set(Nan::New("increase_est_reciprocation_rate").ToLocalChecked(), Nan::New<Integer>(s.increase_est_reciprocation_rate));
        obj->Set(Nan::New("decrease_est_reciprocation_rate").ToLocalChecked(), Nan::New<Integer>(s.decrease_est_reciprocation_rate));
        obj->Set(Nan::New("incoming_starts_queued_torrents").ToLocalChecked(), Nan::New<Boolean>(s.incoming_starts_queued_torrents));
        obj->Set(Nan::New("report_true_downloaded").ToLocalChecked(), Nan::New<Boolean>(s.report_true_downloaded));
        obj->Set(Nan::New("strict_end_game_mode").ToLocalChecked(), Nan::New<Boolean>(s.strict_end_game_mode));
        obj->Set(Nan::New("broadcast_lsd").ToLocalChecked(), Nan::New<Boolean>(s.broadcast_lsd));
        obj->Set(Nan::New("enable_outgoing_utp").ToLocalChecked(), Nan::New<Boolean>(s.enable_outgoing_utp));
        obj->Set(Nan::New("enable_incoming_utp").ToLocalChecked(), Nan::New<Boolean>(s.enable_incoming_utp));
        obj->Set(Nan::New("enable_outgoing_tcp").ToLocalChecked(), Nan::New<Boolean>(s.enable_outgoing_tcp));
        obj->Set(Nan::New("enable_incoming_tcp").ToLocalChecked(), Nan::New<Boolean>(s.enable_incoming_tcp));
        obj->Set(Nan::New("max_pex_peers").ToLocalChecked(), Nan::New<Integer>(s.max_pex_peers));
        obj->Set(Nan::New("ignore_resume_timestamps").ToLocalChecked(), Nan::New<Boolean>(s.ignore_resume_timestamps));
        obj->Set(Nan::New("no_recheck_incomplete_resume").ToLocalChecked(), Nan::New<Boolean>(s.no_recheck_incomplete_resume));
        obj->Set(Nan::New("anonymous_mode").ToLocalChecked(), Nan::New<Boolean>(s.anonymous_mode));
        obj->Set(Nan::New("tick_interval").ToLocalChecked(), Nan::New<Integer>(s.tick_interval));
        obj->Set(Nan::New("report_web_seed_downloads").ToLocalChecked(), Nan::New<Boolean>(s.report_web_seed_downloads));
        obj->Set(Nan::New("share_mode_target").ToLocalChecked(), Nan::New<Integer>(s.share_mode_target));
        obj->Set(Nan::New("upload_rate_limit").ToLocalChecked(), Nan::New<Integer>(s.upload_rate_limit));
        obj->Set(Nan::New("download_rate_limit").ToLocalChecked(), Nan::New<Integer>(s.download_rate_limit));
        obj->Set(Nan::New("local_upload_rate_limit").ToLocalChecked(), Nan::New<Integer>(s.local_upload_rate_limit));
        obj->Set(Nan::New("local_download_rate_limit").ToLocalChecked(), Nan::New<Integer>(s.local_download_rate_limit));
        obj->Set(Nan::New("dht_upload_rate_limit").ToLocalChecked(), Nan::New<Integer>(s.dht_upload_rate_limit));
        obj->Set(Nan::New("unchoke_slots_limit").ToLocalChecked(), Nan::New<Integer>(s.unchoke_slots_limit));
        obj->Set(Nan::New("half_open_limit").ToLocalChecked(), Nan::New<Integer>(s.half_open_limit));
        obj->Set(Nan::New("connections_limit").ToLocalChecked(), Nan::New<Integer>(s.connections_limit));
        obj->Set(Nan::New("utp_target_delay").ToLocalChecked(), Nan::New<Integer>(s.utp_target_delay));
        obj->Set(Nan::New("utp_gain_factor").ToLocalChecked(), Nan::New<Integer>(s.utp_gain_factor));
        obj->Set(Nan::New("utp_min_timeout").ToLocalChecked(), Nan::New<Integer>(s.utp_min_timeout));
        obj->Set(Nan::New("utp_syn_resends").ToLocalChecked(), Nan::New<Integer>(s.utp_syn_resends));
        obj->Set(Nan::New("utp_fin_resends").ToLocalChecked(), Nan::New<Integer>(s.utp_fin_resends));
        obj->Set(Nan::New("utp_num_resends").ToLocalChecked(), Nan::New<Integer>(s.utp_num_resends));
        obj->Set(Nan::New("utp_connect_timeout").ToLocalChecked(), Nan::New<Integer>(s.utp_connect_timeout));
        obj->Set(Nan::New("utp_delayed_ack").ToLocalChecked(), Nan::New<Integer>(s.utp_delayed_ack));
        obj->Set(Nan::New("utp_dynamic_sock_buf").ToLocalChecked(), Nan::New<Boolean>(s.utp_dynamic_sock_buf));
        obj->Set(Nan::New("utp_loss_multiplier").ToLocalChecked(), Nan::New<Integer>(s.utp_loss_multiplier));
        obj->Set(Nan::New("mixed_mode_algorithm").ToLocalChecked(), Nan::New<Integer>(s.mixed_mode_algorithm));
        obj->Set(Nan::New("rate_limit_utp").ToLocalChecked(), Nan::New<Boolean>(s.rate_limit_utp));
        obj->Set(Nan::New("listen_queue_size").ToLocalChecked(), Nan::New<Integer>(s.listen_queue_size));
        obj->Set(Nan::New("announce_double_nat").ToLocalChecked(), Nan::New<Boolean>(s.announce_double_nat));
        obj->Set(Nan::New("torrent_connect_boost").ToLocalChecked(), Nan::New<Integer>(s.torrent_connect_boost));
        obj->Set(Nan::New("seeding_outgoing_connections").ToLocalChecked(), Nan::New<Boolean>(s.seeding_outgoing_connections));
        obj->Set(Nan::New("no_connect_privileged_ports").ToLocalChecked(), Nan::New<Boolean>(s.no_connect_privileged_ports));
        obj->Set(Nan::New("alert_queue_size").ToLocalChecked(), Nan::New<Integer>(s.alert_queue_size));
        obj->Set(Nan::New("max_metadata_size").ToLocalChecked(), Nan::New<Integer>(s.max_metadata_size));
        obj->Set(Nan::New("smooth_connects").ToLocalChecked(), Nan::New<Boolean>(s.smooth_connects));
        obj->Set(Nan::New("always_send_user_agent").ToLocalChecked(), Nan::New<Boolean>(s.always_send_user_agent));
        obj->Set(Nan::New("apply_ip_filter_to_trackers").ToLocalChecked(), Nan::New<Boolean>(s.apply_ip_filter_to_trackers));
        obj->Set(Nan::New("read_job_every").ToLocalChecked(), Nan::New<Integer>(s.read_job_every));
        obj->Set(Nan::New("use_disk_read_ahead").ToLocalChecked(), Nan::New<Boolean>(s.use_disk_read_ahead));
        obj->Set(Nan::New("lock_files").ToLocalChecked(), Nan::New<Boolean>(s.lock_files));
        obj->Set(Nan::New("ssl_listen").ToLocalChecked(), Nan::New<Integer>(s.ssl_listen));
        obj->Set(Nan::New("tracker_backoff").ToLocalChecked(), Nan::New<Integer>(s.tracker_backoff));
        obj->Set(Nan::New("ban_web_seeds").ToLocalChecked(), Nan::New<Boolean>(s.ban_web_seeds));

        return scope.Escape(obj);
    };

    libtorrent::session_settings session_settings_from_object(Local<Object> obj) {
        libtorrent::session_settings s;

        if (Nan::Has(obj, Nan::New("version").ToLocalChecked()) == Nan::Just(true))
            s.version = (Nan::Get(obj, Nan::New("version").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("user_agent").ToLocalChecked()) == Nan::Just(true)) {
            Local<Object> user_agent = (Nan::Get(obj, Nan::New("user_agent").ToLocalChecked()).ToLocalChecked()).As<Object>();
            s.user_agent = std::string(*Nan::Utf8String(user_agent));
        }
        if (Nan::Has(obj, Nan::New("tracker_completion_timeout").ToLocalChecked()) == Nan::Just(true))
            s.tracker_completion_timeout = (Nan::Get(obj, Nan::New("tracker_completion_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("tracker_receive_timeout").ToLocalChecked()) == Nan::Just(true))
            s.tracker_receive_timeout = (Nan::Get(obj, Nan::New("tracker_receive_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("stop_tracker_timeout").ToLocalChecked()) == Nan::Just(true))
            s.stop_tracker_timeout = (Nan::Get(obj, Nan::New("stop_tracker_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("tracker_maximum_response_length").ToLocalChecked()) == Nan::Just(true))
            s.tracker_maximum_response_length = (Nan::Get(obj, Nan::New("tracker_maximum_response_length").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("piece_timeout").ToLocalChecked()) == Nan::Just(true))
            s.piece_timeout = (Nan::Get(obj, Nan::New("piece_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("request_timeout").ToLocalChecked()) == Nan::Just(true))
            s.request_timeout = (Nan::Get(obj, Nan::New("request_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("request_queue_time").ToLocalChecked()) == Nan::Just(true))
            s.request_queue_time = (Nan::Get(obj, Nan::New("request_queue_time").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_allowed_in_request_queue").ToLocalChecked()) == Nan::Just(true))
            s.max_allowed_in_request_queue = (Nan::Get(obj, Nan::New("max_allowed_in_request_queue").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_out_request_queue").ToLocalChecked()) == Nan::Just(true))
            s.max_out_request_queue = (Nan::Get(obj, Nan::New("max_out_request_queue").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("whole_pieces_threshold").ToLocalChecked()) == Nan::Just(true))
            s.whole_pieces_threshold = (Nan::Get(obj, Nan::New("whole_pieces_threshold").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("peer_timeout").ToLocalChecked()) == Nan::Just(true))
            s.peer_timeout = (Nan::Get(obj, Nan::New("peer_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("urlseed_timeout").ToLocalChecked()) == Nan::Just(true))
            s.urlseed_timeout = (Nan::Get(obj, Nan::New("urlseed_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("urlseed_pipeline_size").ToLocalChecked()) == Nan::Just(true))
            s.urlseed_pipeline_size = (Nan::Get(obj, Nan::New("urlseed_pipeline_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("urlseed_wait_retry").ToLocalChecked()) == Nan::Just(true))
            s.urlseed_wait_retry = (Nan::Get(obj, Nan::New("urlseed_wait_retry").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("file_pool_size").ToLocalChecked()) == Nan::Just(true))
            s.file_pool_size = (Nan::Get(obj, Nan::New("file_pool_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("allow_multiple_connections_per_ip").ToLocalChecked()) == Nan::Just(true))
            s.allow_multiple_connections_per_ip = (Nan::Get(obj, Nan::New("allow_multiple_connections_per_ip").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("max_failcount").ToLocalChecked()) == Nan::Just(true))
            s.max_failcount = (Nan::Get(obj, Nan::New("max_failcount").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("min_reconnect_time").ToLocalChecked()) == Nan::Just(true))
            s.min_reconnect_time = (Nan::Get(obj, Nan::New("min_reconnect_time").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("peer_connect_timeout").ToLocalChecked()) == Nan::Just(true))
            s.peer_connect_timeout = (Nan::Get(obj, Nan::New("peer_connect_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("ignore_limits_on_local_network").ToLocalChecked()) == Nan::Just(true))
            s.ignore_limits_on_local_network = (Nan::Get(obj, Nan::New("ignore_limits_on_local_network").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("connection_speed").ToLocalChecked()) == Nan::Just(true))
            s.connection_speed = (Nan::Get(obj, Nan::New("connection_speed").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("send_redundant_have").ToLocalChecked()) == Nan::Just(true))
            s.send_redundant_have = (Nan::Get(obj, Nan::New("send_redundant_have").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("lazy_bitfields").ToLocalChecked()) == Nan::Just(true))
            s.lazy_bitfields = (Nan::Get(obj, Nan::New("lazy_bitfields").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("inactivity_timeout").ToLocalChecked()) == Nan::Just(true))
            s.inactivity_timeout = (Nan::Get(obj, Nan::New("inactivity_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("unchoke_interval").ToLocalChecked()) == Nan::Just(true))
            s.unchoke_interval = (Nan::Get(obj, Nan::New("unchoke_interval").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("optimistic_unchoke_interval").ToLocalChecked()) == Nan::Just(true))
            s.optimistic_unchoke_interval = (Nan::Get(obj, Nan::New("optimistic_unchoke_interval").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("announce_ip").ToLocalChecked()) == Nan::Just(true)) {
            Local<Object> announce_ip = (Nan::Get(obj, Nan::New("announce_ip").ToLocalChecked()).ToLocalChecked()).As<Object>();
            s.announce_ip = std::string(*Nan::Utf8String(announce_ip));
        }
        if (Nan::Has(obj, Nan::New("num_want").ToLocalChecked()) == Nan::Just(true))
            s.num_want = (Nan::Get(obj, Nan::New("num_want").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("initial_picker_threshold").ToLocalChecked()) == Nan::Just(true))
            s.initial_picker_threshold = (Nan::Get(obj, Nan::New("initial_picker_threshold").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("allowed_fast_set_size").ToLocalChecked()) == Nan::Just(true))
            s.allowed_fast_set_size = (Nan::Get(obj, Nan::New("allowed_fast_set_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("suggest_mode").ToLocalChecked()) == Nan::Just(true))
            s.suggest_mode = (Nan::Get(obj, Nan::New("suggest_mode").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_queued_disk_bytes").ToLocalChecked()) == Nan::Just(true))
            s.max_queued_disk_bytes = (Nan::Get(obj, Nan::New("max_queued_disk_bytes").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_queued_disk_bytes_low_watermark").ToLocalChecked()) == Nan::Just(true))
            s.max_queued_disk_bytes_low_watermark = (Nan::Get(obj, Nan::New("max_queued_disk_bytes_low_watermark").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("handshake_timeout").ToLocalChecked()) == Nan::Just(true))
            s.handshake_timeout = (Nan::Get(obj, Nan::New("handshake_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();

#ifndef TORRENT_DISABLE_DHT
        if (Nan::Has(obj, Nan::New("use_dht_as_fallback").ToLocalChecked()) == Nan::Just(true))
            s.use_dht_as_fallback = (Nan::Get(obj, Nan::New("use_dht_as_fallback").ToLocalChecked()).ToLocalChecked())->BooleanValue();
#endif

        if (Nan::Has(obj, Nan::New("free_torrent_hashes").ToLocalChecked()) == Nan::Just(true))
            s.free_torrent_hashes = (Nan::Get(obj, Nan::New("free_torrent_hashes").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("upnp_ignore_nonrouters").ToLocalChecked()) == Nan::Just(true))
            s.upnp_ignore_nonrouters = (Nan::Get(obj, Nan::New("upnp_ignore_nonrouters").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("send_buffer_low_watermark").ToLocalChecked()) == Nan::Just(true))
            s.send_buffer_low_watermark = (Nan::Get(obj, Nan::New("send_buffer_low_watermark").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("send_buffer_watermark").ToLocalChecked()) == Nan::Just(true))
            s.send_buffer_watermark = (Nan::Get(obj, Nan::New("send_buffer_watermark").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("send_buffer_watermark_factor").ToLocalChecked()) == Nan::Just(true))
            s.send_buffer_watermark_factor = (Nan::Get(obj, Nan::New("send_buffer_watermark_factor").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("choking_algorithm").ToLocalChecked()) == Nan::Just(true))
            s.choking_algorithm = (Nan::Get(obj, Nan::New("choking_algorithm").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("seed_choking_algorithm").ToLocalChecked()) == Nan::Just(true))
            s.seed_choking_algorithm = (Nan::Get(obj, Nan::New("seed_choking_algorithm").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("use_parole_mode").ToLocalChecked()) == Nan::Just(true))
            s.use_parole_mode = (Nan::Get(obj, Nan::New("use_parole_mode").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("cache_size").ToLocalChecked()) == Nan::Just(true))
            s.cache_size = (Nan::Get(obj, Nan::New("cache_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("cache_buffer_chunk_size").ToLocalChecked()) == Nan::Just(true))
            s.cache_buffer_chunk_size = (Nan::Get(obj, Nan::New("cache_buffer_chunk_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("cache_expiry").ToLocalChecked()) == Nan::Just(true))
            s.cache_expiry = (Nan::Get(obj, Nan::New("cache_expiry").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("use_read_cache").ToLocalChecked()) == Nan::Just(true))
            s.use_read_cache = (Nan::Get(obj, Nan::New("use_read_cache").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("explicit_read_cache").ToLocalChecked()) == Nan::Just(true))
            s.explicit_read_cache = (Nan::Get(obj, Nan::New("explicit_read_cache").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("explicit_cache_interval").ToLocalChecked()) == Nan::Just(true))
            s.explicit_cache_interval = (Nan::Get(obj, Nan::New("explicit_cache_interval").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("disk_io_write_mode").ToLocalChecked()) == Nan::Just(true))
            s.disk_io_write_mode = (Nan::Get(obj, Nan::New("disk_io_write_mode").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("disk_io_read_mode").ToLocalChecked()) == Nan::Just(true))
            s.disk_io_read_mode = (Nan::Get(obj, Nan::New("disk_io_read_mode").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("coalesce_reads").ToLocalChecked()) == Nan::Just(true))
            s.coalesce_reads = (Nan::Get(obj, Nan::New("coalesce_reads").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("coalesce_writes").ToLocalChecked()) == Nan::Just(true))
            s.coalesce_writes = (Nan::Get(obj, Nan::New("coalesce_writes").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("outgoing_ports").ToLocalChecked()) == Nan::Just(true)) {
            Local<Object> outgoing_ports = (Nan::Get(obj, Nan::New("outgoing_ports").ToLocalChecked()).ToLocalChecked()).As<Object>();
            s.outgoing_ports = std::make_pair(outgoing_ports->Get(0)->IntegerValue(), outgoing_ports->Get(1)->IntegerValue());
        }
        if (Nan::Has(obj, Nan::New("peer_tos").ToLocalChecked()) == Nan::Just(true)) {
            Local<Object> announce_ip = (Nan::Get(obj, Nan::New("peer_tos").ToLocalChecked()).ToLocalChecked()).As<Object>();
            s.peer_tos = std::string(*Nan::Utf8String(announce_ip))[0];
        }
        if (Nan::Has(obj, Nan::New("active_downloads").ToLocalChecked()) == Nan::Just(true))
            s.active_downloads = (Nan::Get(obj, Nan::New("active_downloads").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("active_seeds").ToLocalChecked()) == Nan::Just(true))
            s.active_seeds = (Nan::Get(obj, Nan::New("active_seeds").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("active_dht_limit").ToLocalChecked()) == Nan::Just(true))
            s.active_dht_limit = (Nan::Get(obj, Nan::New("active_dht_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("active_tracker_limit").ToLocalChecked()) == Nan::Just(true))
            s.active_tracker_limit = (Nan::Get(obj, Nan::New("active_tracker_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("active_lsd_limit").ToLocalChecked()) == Nan::Just(true))
            s.active_lsd_limit = (Nan::Get(obj, Nan::New("active_lsd_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("active_limit").ToLocalChecked()) == Nan::Just(true))
            s.active_limit = (Nan::Get(obj, Nan::New("active_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("auto_manage_prefer_seeds").ToLocalChecked()) == Nan::Just(true))
            s.auto_manage_prefer_seeds = (Nan::Get(obj, Nan::New("auto_manage_prefer_seeds").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("dont_count_slow_torrents").ToLocalChecked()) == Nan::Just(true))
            s.dont_count_slow_torrents = (Nan::Get(obj, Nan::New("dont_count_slow_torrents").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("auto_manage_interval").ToLocalChecked()) == Nan::Just(true))
            s.auto_manage_interval = (Nan::Get(obj, Nan::New("auto_manage_interval").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("share_ratio_limit").ToLocalChecked()) == Nan::Just(true))
            s.share_ratio_limit = (Nan::Get(obj, Nan::New("share_ratio_limit").ToLocalChecked()).ToLocalChecked())->NumberValue();
        if (Nan::Has(obj, Nan::New("seed_time_ratio_limit").ToLocalChecked()) == Nan::Just(true))
            s.seed_time_ratio_limit = (Nan::Get(obj, Nan::New("seed_time_ratio_limit").ToLocalChecked()).ToLocalChecked())->NumberValue();
        if (Nan::Has(obj, Nan::New("seed_time_limit").ToLocalChecked()) == Nan::Just(true))
            s.seed_time_limit = (Nan::Get(obj, Nan::New("seed_time_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("peer_turnover_interval").ToLocalChecked()) == Nan::Just(true))
            s.peer_turnover_interval = (Nan::Get(obj, Nan::New("peer_turnover_interval").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("peer_turnover").ToLocalChecked()) == Nan::Just(true))
            s.peer_turnover = (Nan::Get(obj, Nan::New("peer_turnover").ToLocalChecked()).ToLocalChecked())->NumberValue();
        if (Nan::Has(obj, Nan::New("peer_turnover_cutoff").ToLocalChecked()) == Nan::Just(true))
            s.peer_turnover_cutoff = (Nan::Get(obj, Nan::New("peer_turnover_cutoff").ToLocalChecked()).ToLocalChecked())->NumberValue();
        if (Nan::Has(obj, Nan::New("close_redundant_connections").ToLocalChecked()) == Nan::Just(true))
            s.close_redundant_connections = (Nan::Get(obj, Nan::New("close_redundant_connections").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("auto_scrape_interval").ToLocalChecked()) == Nan::Just(true))
            s.auto_scrape_interval = (Nan::Get(obj, Nan::New("auto_scrape_interval").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("auto_scrape_min_interval").ToLocalChecked()) == Nan::Just(true))
            s.auto_scrape_min_interval = (Nan::Get(obj, Nan::New("auto_scrape_min_interval").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_peerlist_size").ToLocalChecked()) == Nan::Just(true))
            s.max_peerlist_size = (Nan::Get(obj, Nan::New("max_peerlist_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_paused_peerlist_size").ToLocalChecked()) == Nan::Just(true))
            s.max_paused_peerlist_size = (Nan::Get(obj, Nan::New("max_paused_peerlist_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("min_announce_interval").ToLocalChecked()) == Nan::Just(true))
            s.min_announce_interval = (Nan::Get(obj, Nan::New("min_announce_interval").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("prioritize_partial_pieces").ToLocalChecked()) == Nan::Just(true))
            s.prioritize_partial_pieces = (Nan::Get(obj, Nan::New("prioritize_partial_pieces").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("auto_manage_startup").ToLocalChecked()) == Nan::Just(true))
            s.auto_manage_startup = (Nan::Get(obj, Nan::New("auto_manage_startup").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("rate_limit_ip_overhead").ToLocalChecked()) == Nan::Just(true))
            s.rate_limit_ip_overhead = (Nan::Get(obj, Nan::New("rate_limit_ip_overhead").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("announce_to_all_trackers").ToLocalChecked()) == Nan::Just(true))
            s.announce_to_all_trackers = (Nan::Get(obj, Nan::New("announce_to_all_trackers").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("announce_to_all_tiers").ToLocalChecked()) == Nan::Just(true))
            s.announce_to_all_tiers = (Nan::Get(obj, Nan::New("announce_to_all_tiers").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("prefer_udp_trackers").ToLocalChecked()) == Nan::Just(true))
            s.prefer_udp_trackers = (Nan::Get(obj, Nan::New("prefer_udp_trackers").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("strict_super_seeding").ToLocalChecked()) == Nan::Just(true))
            s.strict_super_seeding = (Nan::Get(obj, Nan::New("strict_super_seeding").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("seeding_piece_quota").ToLocalChecked()) == Nan::Just(true))
            s.seeding_piece_quota = (Nan::Get(obj, Nan::New("seeding_piece_quota").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_sparse_regions").ToLocalChecked()) == Nan::Just(true))
            s.max_sparse_regions = (Nan::Get(obj, Nan::New("max_sparse_regions").ToLocalChecked()).ToLocalChecked())->IntegerValue();

#ifndef TORRENT_DISABLE_MLOCK
        if (Nan::Has(obj, Nan::New("lock_disk_cache").ToLocalChecked()) == Nan::Just(true))
            s.lock_disk_cache = (Nan::Get(obj, Nan::New("lock_disk_cache").ToLocalChecked()).ToLocalChecked())->BooleanValue();
#endif

        if (Nan::Has(obj, Nan::New("max_rejects").ToLocalChecked()) == Nan::Just(true))
            s.max_rejects = (Nan::Get(obj, Nan::New("max_rejects").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("recv_socket_buffer_size").ToLocalChecked()) == Nan::Just(true))
            s.recv_socket_buffer_size = (Nan::Get(obj, Nan::New("recv_socket_buffer_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("send_socket_buffer_size").ToLocalChecked()) == Nan::Just(true))
            s.send_socket_buffer_size = (Nan::Get(obj, Nan::New("send_socket_buffer_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("optimize_hashing_for_speed").ToLocalChecked()) == Nan::Just(true))
            s.optimize_hashing_for_speed = (Nan::Get(obj, Nan::New("optimize_hashing_for_speed").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("file_checks_delay_per_block").ToLocalChecked()) == Nan::Just(true))
            s.file_checks_delay_per_block = (Nan::Get(obj, Nan::New("file_checks_delay_per_block").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("disk_cache_algorithm").ToLocalChecked()) == Nan::Just(true))
            s.disk_cache_algorithm = (libtorrent::session_settings::disk_cache_algo_t) (Nan::Get(obj, Nan::New("disk_cache_algorithm").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("read_cache_line_size").ToLocalChecked()) == Nan::Just(true))
            s.read_cache_line_size = (Nan::Get(obj, Nan::New("read_cache_line_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("write_cache_line_size").ToLocalChecked()) == Nan::Just(true))
            s.write_cache_line_size = (Nan::Get(obj, Nan::New("write_cache_line_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("optimistic_disk_retry").ToLocalChecked()) == Nan::Just(true))
            s.optimistic_disk_retry = (Nan::Get(obj, Nan::New("optimistic_disk_retry").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("disable_hash_checks").ToLocalChecked()) == Nan::Just(true))
            s.disable_hash_checks = (Nan::Get(obj, Nan::New("disable_hash_checks").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("allow_reordered_disk_operations").ToLocalChecked()) == Nan::Just(true))
            s.allow_reordered_disk_operations = (Nan::Get(obj, Nan::New("allow_reordered_disk_operations").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("allow_i2p_mixed").ToLocalChecked()) == Nan::Just(true))
            s.allow_i2p_mixed = (Nan::Get(obj, Nan::New("allow_i2p_mixed").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("max_suggest_pieces").ToLocalChecked()) == Nan::Just(true))
            s.max_suggest_pieces = (Nan::Get(obj, Nan::New("max_suggest_pieces").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("drop_skipped_requests").ToLocalChecked()) == Nan::Just(true))
            s.drop_skipped_requests = (Nan::Get(obj, Nan::New("drop_skipped_requests").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("low_prio_disk").ToLocalChecked()) == Nan::Just(true))
            s.low_prio_disk = (Nan::Get(obj, Nan::New("low_prio_disk").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("local_service_announce_interval").ToLocalChecked()) == Nan::Just(true))
            s.local_service_announce_interval = (Nan::Get(obj, Nan::New("local_service_announce_interval").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("dht_announce_interval").ToLocalChecked()) == Nan::Just(true))
            s.dht_announce_interval = (Nan::Get(obj, Nan::New("dht_announce_interval").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("udp_tracker_token_expiry").ToLocalChecked()) == Nan::Just(true))
            s.udp_tracker_token_expiry = (Nan::Get(obj, Nan::New("udp_tracker_token_expiry").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("volatile_read_cache").ToLocalChecked()) == Nan::Just(true))
            s.volatile_read_cache = (Nan::Get(obj, Nan::New("volatile_read_cache").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("guided_read_cache").ToLocalChecked()) == Nan::Just(true))
            s.guided_read_cache = (Nan::Get(obj, Nan::New("guided_read_cache").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("default_cache_min_age").ToLocalChecked()) == Nan::Just(true))
            s.default_cache_min_age = (Nan::Get(obj, Nan::New("default_cache_min_age").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("num_optimistic_unchoke_slots").ToLocalChecked()) == Nan::Just(true))
            s.num_optimistic_unchoke_slots = (Nan::Get(obj, Nan::New("num_optimistic_unchoke_slots").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("no_atime_storage").ToLocalChecked()) == Nan::Just(true))
            s.no_atime_storage = (Nan::Get(obj, Nan::New("no_atime_storage").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("default_est_reciprocation_rate").ToLocalChecked()) == Nan::Just(true))
            s.default_est_reciprocation_rate = (Nan::Get(obj, Nan::New("default_est_reciprocation_rate").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("increase_est_reciprocation_rate").ToLocalChecked()) == Nan::Just(true))
            s.increase_est_reciprocation_rate = (Nan::Get(obj, Nan::New("increase_est_reciprocation_rate").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("decrease_est_reciprocation_rate").ToLocalChecked()) == Nan::Just(true))
            s.decrease_est_reciprocation_rate = (Nan::Get(obj, Nan::New("decrease_est_reciprocation_rate").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("incoming_starts_queued_torrents").ToLocalChecked()) == Nan::Just(true))
            s.incoming_starts_queued_torrents = (Nan::Get(obj, Nan::New("incoming_starts_queued_torrents").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("report_true_downloaded").ToLocalChecked()) == Nan::Just(true))
            s.report_true_downloaded = (Nan::Get(obj, Nan::New("report_true_downloaded").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("strict_end_game_mode").ToLocalChecked()) == Nan::Just(true))
            s.strict_end_game_mode = (Nan::Get(obj, Nan::New("strict_end_game_mode").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("broadcast_lsd").ToLocalChecked()) == Nan::Just(true))
            s.broadcast_lsd = (Nan::Get(obj, Nan::New("broadcast_lsd").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("enable_outgoing_utp").ToLocalChecked()) == Nan::Just(true))
            s.enable_outgoing_utp = (Nan::Get(obj, Nan::New("enable_outgoing_utp").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("enable_incoming_utp").ToLocalChecked()) == Nan::Just(true))
            s.enable_incoming_utp = (Nan::Get(obj, Nan::New("enable_incoming_utp").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("enable_outgoing_tcp").ToLocalChecked()) == Nan::Just(true))
            s.enable_outgoing_tcp = (Nan::Get(obj, Nan::New("enable_outgoing_tcp").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("enable_incoming_tcp").ToLocalChecked()) == Nan::Just(true))
            s.enable_incoming_tcp = (Nan::Get(obj, Nan::New("enable_incoming_tcp").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("max_pex_peers").ToLocalChecked()) == Nan::Just(true))
            s.max_pex_peers = (Nan::Get(obj, Nan::New("max_pex_peers").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("ignore_resume_timestamps").ToLocalChecked()) == Nan::Just(true))
            s.ignore_resume_timestamps = (Nan::Get(obj, Nan::New("ignore_resume_timestamps").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("no_recheck_incomplete_resume").ToLocalChecked()) == Nan::Just(true))
            s.no_recheck_incomplete_resume = (Nan::Get(obj, Nan::New("no_recheck_incomplete_resume").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("anonymous_mode").ToLocalChecked()) == Nan::Just(true))
            s.anonymous_mode = (Nan::Get(obj, Nan::New("anonymous_mode").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("tick_interval").ToLocalChecked()) == Nan::Just(true))
            s.tick_interval = (Nan::Get(obj, Nan::New("tick_interval").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("report_web_seed_downloads").ToLocalChecked()) == Nan::Just(true))
            s.report_web_seed_downloads = (Nan::Get(obj, Nan::New("report_web_seed_downloads").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("share_mode_target").ToLocalChecked()) == Nan::Just(true))
            s.share_mode_target = (Nan::Get(obj, Nan::New("share_mode_target").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("upload_rate_limit").ToLocalChecked()) == Nan::Just(true))
            s.upload_rate_limit = (Nan::Get(obj, Nan::New("upload_rate_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("download_rate_limit").ToLocalChecked()) == Nan::Just(true))
            s.download_rate_limit = (Nan::Get(obj, Nan::New("download_rate_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("local_upload_rate_limit").ToLocalChecked()) == Nan::Just(true))
            s.local_upload_rate_limit = (Nan::Get(obj, Nan::New("local_upload_rate_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("local_download_rate_limit").ToLocalChecked()) == Nan::Just(true))
            s.local_download_rate_limit = (Nan::Get(obj, Nan::New("local_download_rate_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("dht_upload_rate_limit").ToLocalChecked()) == Nan::Just(true))
            s.dht_upload_rate_limit = (Nan::Get(obj, Nan::New("dht_upload_rate_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("unchoke_slots_limit").ToLocalChecked()) == Nan::Just(true))
            s.unchoke_slots_limit = (Nan::Get(obj, Nan::New("unchoke_slots_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("half_open_limit").ToLocalChecked()) == Nan::Just(true))
            s.half_open_limit = (Nan::Get(obj, Nan::New("half_open_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("connections_limit").ToLocalChecked()) == Nan::Just(true))
            s.connections_limit = (Nan::Get(obj, Nan::New("connections_limit").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("utp_target_delay").ToLocalChecked()) == Nan::Just(true))
            s.utp_target_delay = (Nan::Get(obj, Nan::New("utp_target_delay").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("utp_gain_factor").ToLocalChecked()) == Nan::Just(true))
            s.utp_gain_factor = (Nan::Get(obj, Nan::New("utp_gain_factor").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("utp_min_timeout").ToLocalChecked()) == Nan::Just(true))
            s.utp_min_timeout = (Nan::Get(obj, Nan::New("utp_min_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("utp_syn_resends").ToLocalChecked()) == Nan::Just(true))
            s.utp_syn_resends = (Nan::Get(obj, Nan::New("utp_syn_resends").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("utp_fin_resends").ToLocalChecked()) == Nan::Just(true))
            s.utp_fin_resends = (Nan::Get(obj, Nan::New("utp_fin_resends").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("utp_num_resends").ToLocalChecked()) == Nan::Just(true))
            s.utp_num_resends = (Nan::Get(obj, Nan::New("utp_num_resends").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("utp_connect_timeout").ToLocalChecked()) == Nan::Just(true))
            s.utp_connect_timeout = (Nan::Get(obj, Nan::New("utp_connect_timeout").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("utp_delayed_ack").ToLocalChecked()) == Nan::Just(true))
            s.utp_delayed_ack = (Nan::Get(obj, Nan::New("utp_delayed_ack").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("utp_dynamic_sock_buf").ToLocalChecked()) == Nan::Just(true))
            s.utp_dynamic_sock_buf = (Nan::Get(obj, Nan::New("utp_dynamic_sock_buf").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("utp_loss_multiplier").ToLocalChecked()) == Nan::Just(true))
            s.utp_loss_multiplier = (Nan::Get(obj, Nan::New("utp_loss_multiplier").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("mixed_mode_algorithm").ToLocalChecked()) == Nan::Just(true))
            s.mixed_mode_algorithm = (Nan::Get(obj, Nan::New("mixed_mode_algorithm").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("rate_limit_utp").ToLocalChecked()) == Nan::Just(true))
            s.rate_limit_utp = (Nan::Get(obj, Nan::New("rate_limit_utp").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("listen_queue_size").ToLocalChecked()) == Nan::Just(true))
            s.listen_queue_size = (Nan::Get(obj, Nan::New("listen_queue_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("announce_double_nat").ToLocalChecked()) == Nan::Just(true))
            s.announce_double_nat = (Nan::Get(obj, Nan::New("announce_double_nat").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("torrent_connect_boost").ToLocalChecked()) == Nan::Just(true))
            s.torrent_connect_boost = (Nan::Get(obj, Nan::New("torrent_connect_boost").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("seeding_outgoing_connections").ToLocalChecked()) == Nan::Just(true))
            s.seeding_outgoing_connections = (Nan::Get(obj, Nan::New("seeding_outgoing_connections").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("no_connect_privileged_ports").ToLocalChecked()) == Nan::Just(true))
            s.no_connect_privileged_ports = (Nan::Get(obj, Nan::New("no_connect_privileged_ports").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("alert_queue_size").ToLocalChecked()) == Nan::Just(true))
            s.alert_queue_size = (Nan::Get(obj, Nan::New("alert_queue_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("max_metadata_size").ToLocalChecked()) == Nan::Just(true))
            s.max_metadata_size = (Nan::Get(obj, Nan::New("max_metadata_size").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("smooth_connects").ToLocalChecked()) == Nan::Just(true))
            s.smooth_connects = (Nan::Get(obj, Nan::New("smooth_connects").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("always_send_user_agent").ToLocalChecked()) == Nan::Just(true))
            s.always_send_user_agent = (Nan::Get(obj, Nan::New("always_send_user_agent").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("apply_ip_filter_to_trackers").ToLocalChecked()) == Nan::Just(true))
            s.apply_ip_filter_to_trackers = (Nan::Get(obj, Nan::New("apply_ip_filter_to_trackers").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("read_job_every").ToLocalChecked()) == Nan::Just(true))
            s.read_job_every = (Nan::Get(obj, Nan::New("read_job_every").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("use_disk_read_ahead").ToLocalChecked()) == Nan::Just(true))
            s.use_disk_read_ahead = (Nan::Get(obj, Nan::New("use_disk_read_ahead").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("lock_files").ToLocalChecked()) == Nan::Just(true))
            s.lock_files = (Nan::Get(obj, Nan::New("lock_files").ToLocalChecked()).ToLocalChecked())->BooleanValue();
        if (Nan::Has(obj, Nan::New("ssl_listen").ToLocalChecked()) == Nan::Just(true))
            s.ssl_listen = (Nan::Get(obj, Nan::New("ssl_listen").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("tracker_backoff").ToLocalChecked()) == Nan::Just(true))
            s.tracker_backoff = (Nan::Get(obj, Nan::New("tracker_backoff").ToLocalChecked()).ToLocalChecked())->IntegerValue();
        if (Nan::Has(obj, Nan::New("ban_web_seeds").ToLocalChecked()) == Nan::Just(true))
            s.ban_web_seeds = (Nan::Get(obj, Nan::New("ban_web_seeds").ToLocalChecked()).ToLocalChecked())->BooleanValue();

        return s;
    };
};
