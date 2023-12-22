// generated by stomptalkgen.js 2023-01-27T15:30:19.295Z
#pragma once

#include "stomptalk/method.h"
#include "stomplay/fnv1a.hpp"
#include <string_view>
#include <cstdint>

namespace stomplay {
namespace method {
namespace tag {

using namespace std::literals;

struct abort {
	constexpr static auto num = 0;
	constexpr static auto text = "ABORT"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<abort, st_method_abort>::value;
};

struct ack {
	constexpr static auto num = abort::num + 1;
	constexpr static auto text = "ACK"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<ack, st_method_ack>::value;
};

struct begin {
	constexpr static auto num = ack::num + 1;
	constexpr static auto text = "BEGIN"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<begin, st_method_begin>::value;
};

struct commit {
	constexpr static auto num = begin::num + 1;
	constexpr static auto text = "COMMIT"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<commit, st_method_commit>::value;
};

struct connect {
	constexpr static auto num = commit::num + 1;
	constexpr static auto text = "CONNECT"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<connect, st_method_connect>::value;
};

struct connected {
	constexpr static auto num = connect::num + 1;
	constexpr static auto text = "CONNECTED"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<connected, st_method_connected>::value;
};

struct disconnect {
	constexpr static auto num = connected::num + 1;
	constexpr static auto text = "DISCONNECT"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<disconnect, st_method_disconnect>::value;
};

struct error {
	constexpr static auto num = disconnect::num + 1;
	constexpr static auto text = "ERROR"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<error, st_method_error>::value;
};

struct message {
	constexpr static auto num = error::num + 1;
	constexpr static auto text = "MESSAGE"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<message, st_method_message>::value;
};

struct nack {
	constexpr static auto num = message::num + 1;
	constexpr static auto text = "NACK"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<nack, st_method_nack>::value;
};

struct receipt {
	constexpr static auto num = nack::num + 1;
	constexpr static auto text = "RECEIPT"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<receipt, st_method_receipt>::value;
};

struct send {
	constexpr static auto num = receipt::num + 1;
	constexpr static auto text = "SEND"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<send, st_method_send>::value;
};

struct stomp {
	constexpr static auto num = send::num + 1;
	constexpr static auto text = "STOMP"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<stomp, st_method_stomp>::value;
};

struct subscribe {
	constexpr static auto num = stomp::num + 1;
	constexpr static auto text = "SUBSCRIBE"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<subscribe, st_method_subscribe>::value;
};

struct unsubscribe {
	constexpr static auto num = subscribe::num + 1;
	constexpr static auto text = "UNSUBSCRIBE"sv;
	constexpr static auto text_size = text.size();
	constexpr static auto text_hash = stomplay::static_hash<unsubscribe, st_method_unsubscribe>::value;
};

constexpr static auto count = unsubscribe::num + 1;

}
}
}
