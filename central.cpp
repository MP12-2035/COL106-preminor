#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include "file_system.hpp"

bool ART_MODE = false;

void to_upper(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void print_header() {
    if (ART_MODE) {
        std::cout << "=============================\n";
        std::cout << "   FILE SYSTEM - ART MODE   \n";
        std::cout << "=============================\n";
    } else {
        std::cout << "File System Ready. Enter command:\n";
    }
}

void print_help() {
    std::cout << "Available commands:\n";
    std::cout << "CREATE <filename>          - Create a new file\n";
    std::cout << "READ <filename>            - Read file content\n";
    std::cout << "INSERT <filename> <text>   - Append content to file\n";
    std::cout << "UPDATE <filename> <text>   - Replace content in file\n";
    std::cout << "SNAPSHOT <filename> <msg>  - Save snapshot with message\n";
    std::cout << "ROLLBACK <filename> [id]   - Rollback to version\n";
    std::cout << "HISTORY <filename>         - Show version history\n";
    std::cout << "RECENT FILES [num]         - Show recent files\n";
    std::cout << "BIGGEST TREES [num]        - Show files with most versions\n";
    std::cout << "COMMAND_HISTORY           - Show past commands\n";
    std::cout << "HELP                      - Show this menu\n";
    std::cout << "ARTMODE ON|OFF             - Enable or disable ASCII art mode\n";
    std::cout << "EXIT                      - Exit the program\n";
}

int main() {
    std::string line;
    print_header();

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line.empty()) continue;

        fs.command_history.push(line);

        std::stringstream ss(line);
        std::string command;
        ss >> command;
        to_upper(command);

        if (command == "CREATE") {
            std::string filename;
            if (!(ss >> filename)) {
                std::cout << "Error: CREATE requires a filename.\n";
                continue;
            }
            fs.create_file(filename);
        }
        else if (command == "READ") {
            std::string filename;
            if (!(ss >> filename)) {
                std::cout << "Error: READ requires a filename.\n";
                continue;
            }
            fs.read_file(filename);
        }
        else if (command == "INSERT") {
            std::string filename, content;
            if (!(ss >> filename)) {
                std::cout << "Error: INSERT requires a filename and content.\n";
                continue;
            }
            std::getline(ss, content);
            if (!content.empty() && content[0] == ' ') content = content.substr(1);
            if (content.empty()) {
                std::cout << "Error: INSERT requires content to append.\n";
                continue;
            }
            fs.insert_into_file(filename, content);
        }
        else if (command == "UPDATE") {
            std::string filename, content;
            if (!(ss >> filename)) {
                std::cout << "Error: UPDATE requires a filename and content.\n";
                continue;
            }
            std::getline(ss, content);
            if (!content.empty() && content[0] == ' ') content = content.substr(1);
            if (content.empty()) {
                std::cout << "Error: UPDATE requires content to replace with.\n";
                continue;
            }
            fs.update_file(filename, content);
        }
        else if (command == "SNAPSHOT") {
            std::string filename, message;
            if (!(ss >> filename)) {
                std::cout << "Error: SNAPSHOT requires a filename.\n";
                continue;
            }
            std::getline(ss, message);
            if (!message.empty() && message[0] == ' ') message = message.substr(1);
            fs.snapshot_file(filename, message);
        }
        else if (command == "ROLLBACK") {
            std::string filename;
            int version_id = -1;
            if (!(ss >> filename)) {
                std::cout << "Error: ROLLBACK requires a filename.\n";
                continue;
            }
            if (ss >> version_id) {
                fs.rb_file(filename, version_id);
            } else {
                fs.rb_file(filename);
            }
        }
        else if (command == "HISTORY") {
            std::string filename;
            if (!(ss >> filename)) {
                std::cout << "Error: HISTORY requires a filename.\n";
                continue;
            }
            fs.show_history(filename);
        }
        else if (command == "RECENT") {
            std::string keyword;
            int num = 5;
            if (!(ss >> keyword) || keyword != "FILES") {
                std::cout << "Error: Use 'RECENT FILES [num]'.\n";
                continue;
            }
            if (ss >> num) {
                fs.recent_files(num);
            } else {
                fs.recent_files(5);
            }
        }
        else if (command == "BIGGEST") {
            std::string keyword;
            int num = 5;
            if (!(ss >> keyword) || keyword != "TREES") {
                std::cout << "Error: Use 'BIGGEST TREES [num]'.\n";
                continue;
            }
            if (ss >> num) {
                fs.biggest_trees(num);
            } else {
                fs.biggest_trees(5);
            }
        }
        else if (command == "COMMAND_HISTORY") {
            fs.show_command_history();
        }
        else if (command == "HELP") {
            print_help();
        }
        else if (command == "ARTMODE") {
            std::string setting;
            if (!(ss >> setting)) {
                std::cout << "Error: ARTMODE requires ON or OFF.\n";
                continue;
            }
            to_upper(setting);
            if (setting == "ON") {
                ART_MODE = true;
                std::cout << "Art mode enabled!\n";
            } else if (setting == "OFF") {
                ART_MODE = false;
                std::cout << "Art mode disabled!\n";
            } else {
                std::cout << "Error: ARTMODE requires ON or OFF.\n";
            }
        }
        else if (command == "EXIT") {
            std::cout << "Exiting...\n";
            break;
        }
        else {
            std::cout << "INVALID COMMAND. Type HELP for a list of commands.\n";
        }
    }
    return 0;
}


