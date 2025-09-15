#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <functional>

using boost::asio::ip::tcp;

class TcpServer {
public:
    using MessageHandler = std::function<std::string(const std::string&)>;
    
    TcpServer(boost::asio::io_context& io_context, short port);
    void setMessageHandler(MessageHandler handler);
    void run();

private:
    void startAccept();
    void handleAccept(std::shared_ptr<tcp::socket> socket, const boost::system::error_code& error);
    void handleClient(std::shared_ptr<tcp::socket> socket);

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    MessageHandler message_handler_;
};