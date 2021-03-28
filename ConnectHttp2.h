#define ASIO_STANDALONE

#include <iostream>
#include <asio/io_context.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "tickers/s_p_500.h"


// https://sandbox.iexapis.com/stable/stock/market/aapl&types=quote&token=Tpk_7ce77b32d77a45a3af97e29b3ae931cb

// {{base_url}}/stock/market/batch?symbols=AAPL&types=intraday-prices

class ConnectHttp2 {
private:
  std::string mApiPublicKey;  // Env variable
  std::string mApiPrivateKey; // Env variable
  std::string url = "";

  asio::error_code ec;
  asio::io_context ctx;
  // asio::ip::tcp::socket _socket;

  std::string GetEnvVar(const std::string& key);

  bool is_connection_open = false;
public:
  using unique_http_connection_ptr = std::unique_ptr<ConnectHttp2>;

  template <typename S=std::string>
  ConnectHttp2::ConnectHttp2(S&& _url):
    url(std::forward<S> _url),
    mApiPublicKey(
      [this](){ return ConnectHttp2::GetEnvVar("API_SANDBOX_PUB_KEY"); }()
    ),
    mApiPrivateKey(
      [this](){ return ConnectHttp2::GetEnvVar("API_SANDBOX_PRIV_KEY"); }()
    )
  {
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address((url+"&token="+mApiPrivateKey), ec), 80);
    _socket.connect(endpoint, ec);

    if (!ec) {
      std::cout<<"Successfully connected to socket\r\n";
    } else {
      std::cout<<"Failed connecting to socket\r\n";
    }
  };

  // ConnectHttp2::~ConnectHttp2()
  // {
  //   if (_socket.is_open()) _socket.close();
  //   return;
  // }

  // void ConnectHttp2::Connect()
  // {
  //   if (_socket.is_open()) {
  //     std::string req = "GET /v1/stock/market/batch?symbols=AAPL&types=intraday-prices&token=Tpk_7ce77b32d77a45a3af97e29b3ae931cb HTTP/1.1\r\n"
  //                       "Host: sandbox.iexapis.com";
  //     _socket.write_some(asio::buffer(req.data(), req.size()), ec);
  //   }
  // }

  void ConnectHttp2::Connect()
  {
    if () {
      std::string req = "GET /v1/stock/market/batch?symbols=AAPL&types=intraday-prices&token=Tpk_7ce77b32d77a45a3af97e29b3ae931cb HTTP/1.1\r\n"
                        "Host: sandbox.iexapis.com";
      _socket.write_some(asio::buffer(req.data(), req.size()), ec);
    }
  }
};
