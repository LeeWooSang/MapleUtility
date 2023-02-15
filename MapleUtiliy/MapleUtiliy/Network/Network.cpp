#include "Network.h"
#include <iostream>
#include <future>
#include "../Common/ThreadPool/ThreadPool.h"

INIT_INSTACNE(Network)
Network::Network()
{
}

Network::~Network()
{
}

void Network::HttpRequest(const std::string& data)
{
    std::cout << "this is http test" << std::endl;

    boost::asio::io_context ioc;
    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::beast::tcp_stream stream(ioc);
    //boost::beast::tcp_stream* stream = new boost::beast::tcp_stream(ioc);

    try
    {
        std::string method = "GET";
        //std::string host = "10.0.6.31";
        std::string host = "localhost";
        std::string port = "40101";
        std::string target = "/wallet/v1/rest/coin/info?service=SOFTWIZ&coin=";
        int version = 11;

        auto const results = resolver.resolve(host, port);
        stream.connect(results);
        //stream->connect(results);

        std::string url = host + ":" + port;
        std::string apiKey = "UzAwOTk6WjhabnZCOHZyMC9jZG5tdytzcUFoL0k2SThib3JGSkg0Y1IrcGo4S3RaNzhuWlZXM3ZHSE16UkNGRU1aa1BLa1o3bE9BUWU2RW5LYzNOeEhEQ3B0TmNKTDFBQzlJdHFlWmtURnlYblJTMkU1YVB6TGR6NEY2eHhMYWlHZlJwdzlNU1J4RW8xNHh3VHBMVUxGRlZQcUtpSUszd0dsOTRjMExLSTYvc2RMRVZvPQ==";

        boost::beast::http::request<boost::beast::http::string_body> req;

        if (method == "POST")
        {
            req.method(boost::beast::http::verb::post);
        }
        else if (method == "PUT")
        {
            req.method(boost::beast::http::verb::put);
        }
        else
        {
            req.method(boost::beast::http::verb::get);
        }

        // request header ¼¼ÆÃ
        req.target(target);
        req.version(version);
        req.set(boost::beast::http::field::host, url);
        req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(boost::beast::http::field::content_type, "application/json; charset=utf-8");
        req.set(boost::beast::http::field::accept, "*/*");
        req.set("apikey", apiKey);
        req.set(boost::beast::http::field::content_length, std::to_string(data.length()));
        req.set(boost::beast::http::field::connection, "close");
        req.body() = data;

        // send
        boost::beast::http::write(stream, req);

        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::dynamic_body> res;
        boost::beast::http::read(stream, buffer, res);
        std::string responseData = boost::beast::buffers_to_string(res.body().data());
        std::cout << responseData << std::endl;

        boost::beast::error_code ec;
        stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        if (ec && ec != boost::beast::errc::not_connected)
        {
            std::cout << "error: " << ec.message() << std::endl;
            return;
        }
    }
    catch (std::exception const& ex)
    {
        std::cout << "exception: " << ex.what() << std::endl;
        return;
    }

    std::cout << "http request end" << std::endl;
}

void Network::HttpResponse(boost::beast::tcp_stream* stream)
{

}
