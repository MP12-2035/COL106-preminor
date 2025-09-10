#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iostream>

class file;

class tree_node {
    friend class file;
public: //private
    int version_id;
    std::string content;
    std::string message;
    const time_t created_ts;
    time_t last_mod_ts;
    time_t ss_ts;
    tree_node* parent;
    std::vector<tree_node*> children;

//public:
    tree_node(int id, const std::string& cont, tree_node* par);
    tree_node(int id, const std::string& cont);
    tree_node(int id);
    tree_node();
    ~tree_node();

    void add_child(tree_node* child);
    bool rm_child(tree_node* child);
    int child_cnt() const;
    std::vector<tree_node*> rootpath();
    bool is_ss() const;
    void upd_cont(const std::string& new_cont);
    void upd_msg(const std::string& new_msg);
    time_t get_created_ts() const;
    time_t get_last_mod_ts() const;
    void set_ss_ts(time_t t);
    time_t get_ss_ts() const;

    // Public getter for content
    std::string get_content() const { return content; }
};

using tn = tree_node;

// Implementation
tn::tree_node(int id, const std::string& cont, tree_node* par)
    : version_id(id), content(cont), parent(par), message(""), ss_ts(0),
      created_ts(std::time(nullptr)), last_mod_ts(created_ts) {}

tn::tree_node(int id, const std::string& cont)
    : tree_node(id, cont, nullptr) {}

tn::tree_node(int id)
    : tree_node(id, "", nullptr) {}

tn::tree_node()
    : tree_node(0, "", nullptr) {}

tn::~tree_node() {
    for (auto child : children) {
        delete child;
    }
    children.clear();
}

void tn::add_child(tree_node* child) {
    if (child) {
        child->parent = this;
        children.push_back(child);
    }
}

bool tn::rm_child(tree_node* child) {
    for (auto it = children.begin(); it != children.end(); ++it) {
        if (*it == child) {
            children.erase(it);
            child->parent = nullptr;
            delete child;
            return true;
        }
    }
    return false;
}

int tn::child_cnt() const {
    return static_cast<int>(children.size());
}

std::vector<tree_node*> tn::rootpath() {
    std::vector<tree_node*> path;
    tree_node* current = this;
    while (current) {
        path.push_back(current);
        current = current->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

bool tn::is_ss() const {
    return ss_ts != 0;
}

void tn::upd_cont(const std::string& new_cont) {
    content = new_cont;
    last_mod_ts = std::time(nullptr);
}

void tn::upd_msg(const std::string& new_msg) {
    message = new_msg;
    last_mod_ts = std::time(nullptr);
}

time_t tn::get_created_ts() const {
    return created_ts;
}

time_t tn::get_last_mod_ts() const {
    return last_mod_ts;
}

void tn::set_ss_ts(time_t t) {
    ss_ts = t;
}

time_t tn::get_ss_ts() const {
    return ss_ts;
}

#endif // TREE_NODE_HPP
