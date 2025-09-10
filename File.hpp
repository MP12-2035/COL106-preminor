#ifndef FILE_HPP
#define FILE_HPP
#include <string>
#include <iostream>  
#include <vector>
#include <queue>
#include "tree_node.hpp"
#include "hash_map.hpp"
class file {
friend class tree_node;
friend class file_system;
private:
    std::string name;
    tree_node* root;
    tree_node* active_version;
    hash_map<int, tree_node*> version_map;
    int total_versions;
    void deleteTree(tree_node* node);
    std::vector<tree_node*> get_vp(int version_id);
public:
    file(const std::string& filename);
    ~file();
    std::string read() const;
    void ins(const std::string& content);
    void upd(const std::string& content);
    void ss(const std::string& message = "");
    void rb(int version_id = -1);
    void history() ;  
    tree_node* find_ver(int version_id);
    const std::string& get_name() const;
    void rnm(const std::string& newName);
    std::vector<tree_node*> get_leaf_vers() const;
    void print(const std::vector<tree_node*>& nodes) const;
    void tag(const std::string& tag); 
};
using fl = file;

fl::fl(const std::string& filename) 
    : name(filename), total_versions(1) 
{
    root = new tree_node(0, "", nullptr);
    root->upd_msg("Initial Snapshot");
    root->ss_ts = std::time(nullptr);
    active_version = root;
    version_map.ins(0, root);
}
fl::~fl() {
    deleteTree(root);
}
void fl::deleteTree(tree_node* node) {
    if (!node) return;
    for (tree_node* child : node->children) {
        deleteTree(child);
    }
    delete node;
}
std::string fl::read() const {
    if (active_version)
        return active_version->content;
    return "";
}
void fl::ins(const std::string& content) {
    if (!active_version) {
        std::cout<<"No version selected as active."<<std::endl;
        return;
    }
    if (active_version->is_ss()) {
        tree_node* new_node = new tree_node(total_versions, active_version->content, active_version);
        new_node->upd_cont(new_node->content + content);
        active_version->add_child(new_node);
        active_version = new_node;
        version_map.ins(total_versions, new_node);
        ++total_versions;
    } else {
        active_version->upd_cont(active_version->content + content);
    }
}
void fl::upd(const std::string& content) {
    if (!active_version) {
        std::cout<<"No version selected as active."<<std::endl;
        return;
    }
    if (active_version->is_ss()) {
        tree_node* new_node = new tree_node(total_versions, content, active_version);
        active_version->add_child(new_node);
        active_version = new_node;
        version_map.ins(total_versions, new_node);
        ++total_versions;
    } else {
        active_version->upd_cont(content);
    }
}
void fl::ss(const std::string& message) {
    if (!active_version) {
        std::cout<<"No version selected as active."<<std::endl;
        return;
    }
    active_version->upd_msg(message);
    active_version->ss_ts = std::time(nullptr);
}
void fl::rb(int ver_id) {
    if (ver_id == -1) {  
        if (active_version && active_version->parent) {
            active_version = active_version->parent;
        } else {
            std::cout << "No parent version to rb to."<<std::endl;
        }
    } else { 
        tree_node* target = nullptr;
        if (version_map.find(ver_id, target) && target != nullptr) {
            tree_node* cur = active_version;
            bool ancestor_found = false;
            while (cur != nullptr) {
                if (cur == target) {
                    ancestor_found = true;
                    break;
                }
                cur = cur->parent;
            }
            if (ancestor_found) {
                active_version = target;
            } else {
                std::cout << "Version " << ver_id << " is not an ancestor of current version. Rollback denied."<<std::endl;
            }
        } else {
            std::cout << "Version ID: " << ver_id << " not found.\n";
        }
    }
}
void fl::history()  {
    if (!active_version) return;
    std::vector<tree_node*> path = get_vp(active_version->version_id);
    std::vector<tree_node*> snapshots;
    for (auto node : path) {
        if (node->is_ss()) {
            snapshots.push_back(node);
        }
    }
    print(snapshots);
}
const std::string& fl::get_name() const {
    return name;
}
tree_node* fl::find_ver(int version_id) {
    tree_node* node = nullptr;
    if (version_map.find(version_id, node)) {
        return node;
    }
    std::cout<<"NOT FOUND"<<std::endl;
    return nullptr;
}
std::vector<tree_node*> fl::get_vp(int version_id) {
    tree_node* node = find_ver(version_id);
    if (node == nullptr) return {};
    return node->rootpath();
}
void fl::print(const std::vector<tree_node*>& nodes) const {
    for (tree_node* node : nodes) {
        if (node) {
            std::cout << "Version ID: " << node->version_id
                      << " | Message: " << node->message
                      << " | Updated: " << std::ctime(&node->created_ts) << std::endl;
        }
    }
}
std::vector<tree_node*> fl::get_leaf_vers() const {
    std::vector<tree_node*> leaves;
    if (!root) return leaves;
    std::queue<tree_node*> q;
    q.push(root);
    while (!q.empty()) {
        tree_node* current = q.front();
        q.pop();
        if (current->child_cnt() == 0) {
            leaves.push_back(current);
        } else {
            for (tree_node* child : current->children) {
                q.push(child);
            }
        }
    }
    return leaves;
}
void fl::tag(const std::string& tag) {
    version_map.iterate([&tag](const int& ver_id, tree_node* &node){
        if (node) {
            node->message += " " + tag;
        }
    });
}
#endif // FILE_HPP
