
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <string>
#include <vector>


namespace po = boost::program_options;
namespace pt = boost::property_tree;

int main(int argc, char* argv[]) {
    try {
        // Declare options
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("config", po::value<std::vector<std::string>>()->multitoken(), "path to config file(s)");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (argc == 1 || vm.count("help")) {
            std::cout << desc << "\n";
            return 1;
        }

        if (vm.count("config")) {
            const std::vector<std::string>& config_paths = vm["config"].as<std::vector<std::string>>();
            
            for (const auto& path : config_paths) {
                pt::ptree config;
                pt::read_json(path, config);
                
                try {
                    std::string ip = config.get<std::string>("ip");
                    std::string socket = config.get<std::string>("socket");
                    std::string spi_path = config.get<std::string>("spi_path");

                    std::cout << "Configuration from " << path << ":" << std::endl;
                    std::cout << "IP: " << ip << std::endl;
                    std::cout << "Socket: " << socket << std::endl; 
                    std::cout << "SPI Path: " << spi_path << std::endl;
                }
                catch (pt::ptree_error& e) {
                    std::cerr << "Error reading config " << path << ": " << e.what() << std::endl;
                }
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}