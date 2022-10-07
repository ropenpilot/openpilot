#pragma once
#include "inc.h"
void load_server_certificate(ssl::context &ctx);

// this hardcode is in src/ssl_gen.sh also
#define CURDIR string("/data/openpilot/wss/")
#define SERVER_PEM_FILE string(CURDIR + "keys/server.key.pem")
#define SERVER_CERT_FILE string(CURDIR + "keys/server.cert.pem")
//
#define SERVER_DOC_ROOT string(CURDIR + "public")
//#define SERVER_SSL_REDIRECT_LOCATION (string("https://") + SERVER_BIND_ADDRESS + ":" + std::to_string(SERVER_LISTEN_PORT))


string ssl_ctx_get_passtord() { return "password"; }
//string ssl_ctx_get_passtord() { return "test"; }
void load_server_certificate(ssl::context &context_) {
    context_.set_options(
        boost::asio::ssl::context::default_workarounds
        | boost::asio::ssl::context::no_sslv2
        | boost::asio::ssl::context::single_dh_use);
    //context_.set_password_callback(boost::bind(&server::get_password, this));
    context_.set_password_callback(boost::bind(ssl_ctx_get_passtord));
    context_.use_certificate_chain_file(SERVER_CERT_FILE);
    context_.use_private_key_file(SERVER_PEM_FILE, boost::asio::ssl::context::pem);
    // TODO?
    //context_.use_tmp_dh_file("dh4096.pem");
}
