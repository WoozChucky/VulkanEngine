//
// Created by nunol on 12/27/2021.
//
#include <Channel.hpp>

#include <rabbitmq-c/tcp_socket.h>
#include <rabbitmq-c/amqp.h>
#include <cstdlib>
#include <cstdio>

void die_on_amqp_error(amqp_rpc_reply_t x, char const *context) {
	switch (x.reply_type) {
		case AMQP_RESPONSE_NORMAL:
			return;

		case AMQP_RESPONSE_NONE:
			fprintf(stderr, "%s: missing RPC reply type!\n", context);
			break;

		case AMQP_RESPONSE_LIBRARY_EXCEPTION:
			fprintf(stderr, "%s: %s\n", context, amqp_error_string2(x.library_error));
			break;

		case AMQP_RESPONSE_SERVER_EXCEPTION:
			switch (x.reply.id) {
				case AMQP_CONNECTION_CLOSE_METHOD: {
					amqp_connection_close_t *m =
						(amqp_connection_close_t *)x.reply.decoded;
					fprintf(stderr, "%s: server connection error %uh, message: %.*s\n",
							context, m->reply_code, (int)m->reply_text.len,
							(char *)m->reply_text.bytes);
					break;
				}
				case AMQP_CHANNEL_CLOSE_METHOD: {
					amqp_channel_close_t *m = (amqp_channel_close_t *)x.reply.decoded;
					fprintf(stderr, "%s: server channel error %uh, message: %.*s\n",
							context, m->reply_code, (int)m->reply_text.len,
							(char *)m->reply_text.bytes);
					break;
				}
				default:
					fprintf(stderr, "%s: unknown server error, method id 0x%08X\n",
							context, x.reply.id);
					break;
			}
			break;
	}

	exit(1);
}

Channel::Channel() {
	amqp_socket_t *socket = NULL;
	amqp_connection_state_t conn;

	conn = amqp_new_connection();

	socket = amqp_tcp_socket_new(conn);
	if (!socket) {
		exit(-1);
	}

	auto status = amqp_socket_open(socket, "127.0.0.1", 5672);
	if (status != AMQP_STATUS_OK) {
		exit(-1);
	}

	auto loginResponse = amqp_login(
		conn,
		"/",
		0,
		131072,
		0,
		AMQP_SASL_METHOD_PLAIN,
		"user",
		"password"
	);

	if (loginResponse.reply.decoded == nullptr) {
	}

	amqp_channel_open(conn, 1);
	amqp_get_rpc_reply(conn);

	{
		amqp_basic_properties_t props;
		props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
		props.content_type = amqp_cstring_bytes("text/plain");
		props.delivery_mode = 2; /* persistent delivery mode */
		amqp_basic_publish(conn, 1, amqp_cstring_bytes("exchange"),
						   amqp_cstring_bytes("routingkey"), 0, 0,
						   &props, amqp_cstring_bytes("messagebody"));
	}

	amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
	amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
	amqp_destroy_connection(conn);
}

