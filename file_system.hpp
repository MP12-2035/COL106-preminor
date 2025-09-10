#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <string>
#include <iostream>
#include <stack>
#include "file.hpp"
#include "hash_map.hpp"
#include "heap.hpp"

class file_system {
private:
    hp biggest_trees_h;
    std::stack<std::string> recent_files_s;
    int op_count = 0;

    std::string gen_untitled_name() {
        return "untitled" + std::to_string(++untitled_cnt);
    }

    void remind_snapshot() {
        if (++op_count % 10 == 0) {
            std::cout << "Reminder: Consider taking a snapshot after important changes." << std::endl;
        }
    }

    void print_node(tree_node* node, const std::string& prefix = "", bool is_last = true, bool art_mode = false) {
        if (!node) return;

        std::cout << prefix << (is_last ? "└─ " : "├─ ") << "V" << node->version_id;
        if (!node->message.empty()) std::cout << " : \"" << node->message << "\"";
        std::cout << std::endl;

        for (size_t i = 0; i < node->children.size(); ++i) {
            print_node(node->children[i], prefix + (is_last ? "    " : "│   "), i == node->children.size() - 1, art_mode);
        }
    }

public:
    int untitled_cnt = 0;
    hash_map<std::string, fl*> files_map;
    std::stack<std::string> command_history;

    file_system() {}
    ~file_system() {}

    std::string create_file(const std::string& filename) {
        fl* existing_file = nullptr;
        if (files_map.find(filename, existing_file)) {
            std::cout << "File '" << filename << "' already exists." << std::endl;
            return "";
        }
        fl* new_file = new fl(filename);
        files_map.ins(filename, new_file);
        biggest_trees_h.ins(filename, new_file->total_versions);
        accessed_file(filename);
        remind_snapshot();
        return filename;
    }

    std::string create_file() {
        std::string untitled_name = gen_untitled_name();
        return create_file(untitled_name);
    }

    bool rnm_file(const std::string& old_n, const std::string& new_n) {
        fl* file = nullptr;
        if (!files_map.find(old_n, file)) {
            std::cout << "File '" << old_n << "' not found." << std::endl;
            return false;
        }
        if (files_map.find(new_n, file)) {
            std::cout << "File '" << new_n << "' already exists." << std::endl;
            return false;
        }
        files_map.rm(old_n);
        file->rnm(new_n);
        files_map.ins(new_n, file);
        biggest_trees_h.rm(old_n);
        biggest_trees_h.ins(new_n, file->total_versions);
        std::cout << "File renamed from '" << old_n << "' to '" << new_n << "'" << std::endl;
        remind_snapshot();
        return true;
    }

    void read_file(const std::string& filename) {
        fl* file = nullptr;
        if (!files_map.find(filename, file)) {
            std::cout << "File '" << filename << "' not found." << std::endl;
            return;
        }
        std::cout << file->read() << std::endl;
        accessed_file(filename);
        remind_snapshot();
    }

    void insert_into_file(const std::string& filename, const std::string& content) {
        fl* file = nullptr;
        if (!files_map.find(filename, file)) {
            std::cout << "File '" << filename << "' not found." << std::endl;
            return;
        }
        file->ins(content);
        biggest_trees_h.upd(filename, file->total_versions);
        accessed_file(filename);
        remind_snapshot();
    }

    void update_file(const std::string& filename, const std::string& content) {
        fl* file = nullptr;
        if (!files_map.find(filename, file)) {
            std::cout << "File '" << filename << "' not found." << std::endl;
            return;
        }
        file->upd(content);
        biggest_trees_h.upd(filename, file->total_versions);
        accessed_file(filename);
        remind_snapshot();
    }

    void snapshot_file(const std::string& filename, const std::string& message) {
        fl* file = nullptr;
        if (!files_map.find(filename, file)) {
            std::cout << "File '" << filename << "' not found." << std::endl;
            return;
        }
        file->ss(message);
        biggest_trees_h.upd(filename, file->total_versions);
        accessed_file(filename);
        remind_snapshot();
    }

    void rb_file(const std::string& filename, int ver_id = -1) {
        fl* file = nullptr;
        if (!files_map.find(filename, file)) {
            std::cout << "File '" << filename << "' not found." << std::endl;
            return;
        }
        file->rb(ver_id);
        accessed_file(filename);
        remind_snapshot();
    }

    void show_history(const std::string& filename) {
        fl* file = nullptr;
        if (!files_map.find(filename, file)) {
            std::cout << "File '" << filename << "' not found." << std::endl;
            return;
        }
        file->history();
        remind_snapshot();
    }

    void recent_files(int num) {
        if (num <= 0) return;
        std::stack<std::string> temp_s = recent_files_s;
        int count = 0;
        while (!temp_s.empty() && count < num) {
            std::cout << temp_s.top() << std::endl;
            temp_s.pop();
            ++count;
        }
        remind_snapshot();
    }

    void biggest_trees(int num) {
        biggest_trees_h.print_top(num);
        remind_snapshot();
    }

    void accessed_file(const std::string& filename) {
        recent_files_s.push(filename);
    }

    void show_command_history() {
        std::stack<std::string> temp = command_history;
        while (!temp.empty()) {
            std::cout << temp.top() << std::endl;
            temp.pop();
        }
    }

    void print_version_tree(const std::string& filename, bool use_art = false) {
        fl* file_ptr = nullptr;
        if (!files_map.find(filename, file_ptr) || !file_ptr) {
            std::cout << "File '" << filename << "' not found.\n";
            return;
        }
        print_node(file_ptr->root, "", true, use_art);
    }

    void switch_version(const std::string& filename, int version_id) {
        fl* file = nullptr;
        if (!files_map.find(filename, file)) {
            std::cout << "File '" << filename << "' not found." << std::endl;
            return;
        }
        if (file->switch_version(version_id)) {
            std::cout << "Switched to version " << version_id << " of file '" << filename << "'." << std::endl;
            accessed_file(filename);
            remind_snapshot();
        }
    }

    void show_active_version(const std::string& filename) {
        fl* file = nullptr;
        if (!files_map.find(filename, file)) {
            std::cout << "File '" << filename << "' not found." << std::endl;
            return;
        }
        file->print_active_version_info();
    }

};

using fs = file_system;

#endif // FILE_SYSTEM_HPP
