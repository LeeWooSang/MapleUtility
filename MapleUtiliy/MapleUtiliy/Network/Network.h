#pragma once
#include "../Common/Macro.h"
#include <string>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <unordered_set>
class Network
{
    SINGLE_TONE(Network)

public:
    void HttpRequest(const std::string& data);
    void HttpResponse(boost::beast::tcp_stream* stream);

private:
    std::unordered_set<boost::beast::tcp_stream*> mStreamSet;
};

