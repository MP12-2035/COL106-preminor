#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "file_system.hpp"
#include "art.hpp"

class CommandHandler {
private:
    file_system& fs;
    ArtMode& art;

public:
    CommandHandler(file_system& fs_ref, ArtMode& art_ref)
        : fs(fs_ref), art(art_ref) {}

    void execute(const std::string& cmd_line) {
        std::istringstream iss(cmd_line);
        std::string cmd;
        iss >> cmd;
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

        fs.command_history.push(cmd_line);

        if (cmd == "CREATE") {
            std::string filename;
            if (iss >> filename) {
                std::string created_name = fs.create_file(filename);
                if (!created_name.empty())
                    std::cout << "'" << created_name << "' has been created." << std::endl;
                else
                    std::cout << "File creation failed." << std::endl;
            }
            else {
                std::string created_name = fs.create_file();
                if (!created_name.empty())
                    std::cout << "'" << created_name << "' has been created." << std::endl;
                else
                    std::cout << "File creation failed." << std::endl;
            }
        }
        else if (cmd == "READ") {
            std::string filename;
            if (iss >> filename) {
                std::cout << "'" << filename << "' : ";
                fs.read_file(filename);
            }
            else std::cout << "Usage: READ <filename>" << std::endl;
        }
        else if (cmd == "INSERT") {
            std::string filename, text;
            if (iss >> filename) {
                std::getline(iss, text);
                if (!text.empty() && text[0] == ' ') text.erase(0, 1);
                fs.insert_into_file(filename, text);
            }
            else std::cout << "Usage: INSERT <filename> <text>" << std::endl;
        }
        else if (cmd == "UPDATE") {
            std::string filename, text;
            if (iss >> filename) {
                std::getline(iss, text);
                if (!text.empty() && text[0] == ' ') text.erase(0, 1);
                fs.update_file(filename, text);
            }
            else std::cout << "Usage: UPDATE <filename> <text>" << std::endl;
        }
        else if (cmd == "SNAPSHOT") {
            std::string filename, message;
            if (iss >> filename) {
                std::getline(iss, message);
                if (!message.empty() && message[0] == ' ') message.erase(0, 1);
                fs.snapshot_file(filename, message);
            }
            else std::cout << "Usage: SNAPSHOT <filename> <message>" << std::endl;
        }
        else if (cmd == "ROLLBACK") {
            std::string filename;
            int ver_id = -1;
            if (iss >> filename) {
                if (iss >> ver_id)
                    fs.rb_file(filename, ver_id);
                else
                    fs.rb_file(filename);
            }
            else std::cout << "Usage: ROLLBACK <filename> [version_id]" << std::endl;
        }
        else if (cmd == "HISTORY") {
            std::string filename;
            if (iss >> filename) {
                std::cout << "-----------------------------------------" << std::endl;
                std::cout << "History of '" << filename << "' :" << std::endl;
                fs.show_history(filename);
                std::cout << "-----------------------------------------" << std::endl;
            }
            else std::cout << "Usage: HISTORY <filename>" << std::endl;
        }
        else if (cmd == "RECENT") {
            int num = 5;
            iss >> num;
            fs.recent_files(num);
        }
        else if (cmd == "BIGGEST") {
            int num = 5;
            iss >> num;
            fs.biggest_trees(num);
        }
        else if (cmd == "COMMAND_HISTORY") {
            fs.show_command_history();
        }
        else if (cmd == "ARTMODE") {
            std::string mode;
            if (iss >> mode)
                art.set_enabled(mode == "ON" || mode == "on");
            else
                std::cout << "Usage: ARTMODE ON|OFF" << std::endl;
        }
        else if (cmd == "RENAME") {
            std::string old_name, new_name;
            if (iss >> old_name >> new_name) {
                if (!fs.rnm_file(old_name, new_name))
                    std::cout << "Rename failed." << std::endl;
            }
            else {
                std::cout << "Usage: RENAME <old_filename> <new_filename>" << std::endl;
            }
        }
        else if (cmd == "SWITCH") {
            std::string filename;
            int version_id;
            if (iss >> filename >> version_id) {
                fs.switch_version(filename, version_id);
            } else {
                std::cout << "Usage: SWITCH <filename> <version_id>" << std::endl;
            }
        }
        else if (cmd == "CURRENT_VERSION") {
            std::string filename;
            if (iss >> filename) {
                fs.show_active_version(filename);
            } else {
                std::cout << "Usage: CURRENT_VERSION <filename>" << std::endl;
            }
        }
        else if (cmd == "TREE") {
            std::string filename;
            if (iss >> filename) {
                std::cout << "-----------------------------------------" << std::endl;
                std::cout << "VERSION TREE of '" << filename << "' :" << std::endl;
                fl* f_ptr = nullptr;
                if (fs.files_map.find(filename, f_ptr) && f_ptr) {
                    if (art.is_enabled()) {
                        art.show_version_tree_bubbles(f_ptr->find_ver(0));
                    }
                    else {
                        fs.print_version_tree(filename, false);
                    }
                }
                else std::cout << "File '" << filename << "' not found." << std::endl;
            }
            else std::cout << "Usage: TREE <filename>" << std::endl;
        }
        else if (cmd == "HELP") {
            std::cout << "-----------------------------------------" << std::endl;
            art.display("Available commands with descriptions:");
            art.display("CREATE <filename>       : Create a new file (or 'Untitled' if no name given)");
            art.display("READ <filename>         : Display contents of a file");
            art.display("INSERT <filename> <text>: Insert text at the end of a file");
            art.display("UPDATE <filename> <text>: Overwrite file contents with new text");
            art.display("SNAPSHOT <filename> <msg>: Save a version of the file with a message");
            art.display("ROLLBACK <filename> [id]: Revert file to a previous version by ID");
            art.display("HISTORY <filename>      : Show all snapshots and messages of a file");
            art.display("RECENT [num]            : Show the most recently accessed files (default 5)");
            art.display("BIGGEST [num]           : Show files with largest version trees (default 5)");
            art.display("COMMAND_HISTORY         : Show history of executed commands");
            art.display("ARTMODE ON|OFF          : Enable or disable Art Mode for nicer output");
            art.display("RENAME <old> <new>      : Rename a file");
            art.display("TREE <filename>         : Display the version tree of a file visually");
            art.display("HELP                    : Show this help menu with descriptions");
            art.display("EXIT                    : Exit the program");
            std::cout << "-----------------------------------------" << std::endl;
        }
        else if (cmd == "EXIT") {
            std::cout << "-----------------------------------------" << std::endl;
            art.display("Exiting...");
            if (art.is_enabled()) art.show_bye();
            exit(0);
        }
        else {
            art.display("Unknown command: " + cmd);
        }
    }
};

#endif // COMMANDS_HPP
