#include <iostream>
#include <sstream>
#include <string>
#include "FileSystem.hpp"

int main() {
    FileSystem fs;
    std::string line;

    std::cout << "Time-Travelling File System"<<endl;
    std::cout << "Enter commands (CREATE, READ, INSERT, UPDATE, SNAPSHOT, ROLLBACK, HISTORY, RECENT FILES, BIGGEST TREES, EXIT)"<<endl;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        if (line == "EXIT") break;

        std::istringstream iss(line);
        std::string command, filename;
        iss >> command;

        if (command == "CREATE") {
            iss >> filename;
            fs.createFile(filename);
        } else if (command == "READ") {
            iss >> filename;
            fs.readFile(filename);
        } else if (command == "INSERT") {
            iss >> filename;
            std::string content;
            std::getline(iss, content);
            if (!content.empty() && content[0] == ' ') content.erase(0, 1);
            fs.insertIntoFile(filename, content);
        } else if (command == "UPDATE") {
            iss >> filename;
            std::string content;
            std::getline(iss, content);
            if (!content.empty() && content[0] == ' ') content.erase(0, 1);
            fs.updateFile(filename, content);
        } else if (command == "SNAPSHOT") {
            iss >> filename;
            std::string message;
            std::getline(iss, message);
            if (!message.empty() && message[0] == ' ') message.erase(0, 1);
            fs.snapshotFile(filename, message);
        } else if (command == "ROLLBACK") {
            iss >> filename;
            int version_id = -1;
            if (!(iss >> version_id)) version_id = -1; // no version specified
            fs.rollbackFile(filename, version_id);
        } else if (command == "HISTORY") {
            iss >> filename;
            fs.showHistory(filename);
        } else if (command == "RECENT") {
            int num;
            iss >> num;
            fs.recentFiles(num);
        } else if (command == "BIGGEST") {
            int num;
            iss >> num;
            fs.biggestTrees(num);
        } else {
            std::cout << "Unknown command.\n";
        }
    }

    return 0;
}
