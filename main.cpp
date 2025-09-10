#include <iostream>
#include <string>
#include "file_system.hpp"
#include "commands.hpp"
#include "art.hpp"

int main() {
    file_system fs;
    ArtMode art;

    std::string art_input;
    std::cout<<"-----------------------------------------"<<std::endl;
    std::cout << "[*]Do you want to enable Art Mode? (Enter ON/OFF) "<<std::endl;
    std::getline(std::cin, art_input);
    art.set_enabled(art_input == "ON" || art_input == "on");
    
    std::cout<<"-----------------------------------------"<<std::endl;

    CommandHandler handler(fs, art);
    

    std::cout << "[*]File System Ready."<<"\n"<< "[*]Note: all programs must end with 'EXIT'." << std::endl;
    std::cout<<"-----------------------------------------"<<std::endl;

    std::string command;
    while (true) {
        std::getline(std::cin, command);

        if (command.empty()) continue; 

        handler.execute(command);
    }

    return 0;
}
