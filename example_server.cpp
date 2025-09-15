#include "inc/tcp_server.h"
#include <iostream>
#include <thread>

int main() {
    try {
        boost::asio::io_context io_context;
        TcpServer server(io_context, 8080);
        
        // Set command handler
        server.setMessageHandler([](const std::string& command) -> std::string {
            std::cout << "Received: " << command << std::endl;
            
            if (command.find("GET_STATUS") != std::string::npos) {
                return "STATUS:OK";
            } else if (command.find("SET_CONFIG") != std::string::npos) {
                return "CONFIG:UPDATED";
            }
            return "UNKNOWN_COMMAND";
        });
        
        std::cout << "Server listening on port 8080..." << std::endl;
        server.run();
        
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}