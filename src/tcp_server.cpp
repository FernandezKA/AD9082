#include "../inc/tcp_server.h"
#include <iostream>

TcpServer::TcpServer(boost::asio::io_context& io_context, short port)
    : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    startAccept();
}

void TcpServer::setMessageHandler(MessageHandler handler) {
    message_handler_ = std::move(handler);
}

void TcpServer::run() {
    io_context_.run();
}

void TcpServer::startAccept() {
    auto socket = std::make_shared<tcp::socket>(io_context_);
    acceptor_.async_accept(*socket,
        [this, socket](const boost::system::error_code& error) {
            handleAccept(socket, error);
        });
}

void TcpServer::handleAccept(std::shared_ptr<tcp::socket> socket, const boost::system::error_code& error) {
    if (!error) {
        std::thread([this, socket]() { handleClient(socket); }).detach();
    }
    startAccept();
}

void TcpServer::handleClient(std::shared_ptr<tcp::socket> socket) {
    try {
        char buffer[1024];
        while (true) {
            boost::system::error_code error;
            size_t length = socket->read_some(boost::asio::buffer(buffer), error);
            
            if (error) break;
            
            std::string request(buffer, length);
            std::string response = message_handler_ ? message_handler_(request) : "OK";
            
            boost::asio::write(*socket, boost::asio::buffer(response));
        }
    } catch (std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
    }
}