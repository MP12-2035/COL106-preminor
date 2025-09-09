#ifndef FILE_HPP
#define FILE_HPP
#include <string>
#include <iostream>  
#include <vector>
#include <queue>
#include "TreeNode.hpp"
#include "HashMap.hpp"

class File {
friend class TreeNode;
private:
    std::string name;
    TreeNode* root;
    TreeNode* active_version;
    HashMap<int, TreeNode*> version_map;
    int total_versions;

    void deleteTree(TreeNode* node);
    std::vector<TreeNode*> getVersionPath(int version_id);

public:
    File(const std::string& filename);
    ~File();

    std::string read() const;
    void insert(const std::string& content);
    void update(const std::string& content);
    void snapshot(const std::string& message);
    void rollback(int version_id = -1);
    void history() ;  //uses getVersionPath internally

    TreeNode* findVersion(int version_id);
    const std::string& getName() const;
    std::vector<TreeNode*> getLeafVersions() const;
    void print(const std::vector<TreeNode*>& nodes) const;

};

// Constructor
File::File(const std::string& filename) 
    : name(filename), total_versions(1) 
{
    root = new TreeNode(0, "", nullptr);
    root->updateMessage("Initial Snapshot");
    root->snapshot_timestamp = std::time(nullptr);
    active_version = root;
    version_map.insert(0, root);
}

// Destructor
File::~File() {
    deleteTree(root);
}

// Recursively delete versions in the tree
void File::deleteTree(TreeNode* node) {
    if (!node) return;
    for (TreeNode* child : node->children) {
        deleteTree(child);
    }
    delete node;
}

// Returns content of active version
std::string File::read() const {
    if (active_version)
        return active_version->content;
    return "";
}

// Append content to active version or create new child if snapshot
void File::insert(const std::string& content) {
    if (!active_version) {
        std::cout<<"No version selected as active."<<std::endl;
    return;
    }
    if (active_version->isSnapshot()) {
        TreeNode* new_node = new TreeNode(total_versions, active_version->content, active_version);
        new_node->updateContent(new_node->content + content);
        active_version->addChild(new_node);
        active_version = new_node;
        version_map.insert(total_versions, new_node);
        ++total_versions;
    } else {
        active_version->updateContent(active_version->content + content);
    }
}

// Replace content of active version or create new child if snapshot
void File::update(const std::string& content) {
    if (!active_version) {
        std::cout<<"No version selected as active."<<std::endl;
    return;
    }
    if (active_version->isSnapshot()) {
        TreeNode* new_node = new TreeNode(total_versions, content, active_version);
        active_version->addChild(new_node);
        active_version = new_node;
        version_map.insert(total_versions, new_node);
        ++total_versions;
    } else {
        active_version->updateContent(content);
    }
}

// Mark current active version as snapshot with message
void File::snapshot(const std::string& message="") {
    if (!active_version) {
        std::cout<<"No version selected as active."<<std::endl;
    return;
    }
    active_version->updateMessage(message);
    active_version->snapshot_timestamp = std::time(nullptr);
}

// Rollback to specified version or parent if no version specified
void File::rollback(int version_id) {
    if (version_id == -1) {  // rollback to parent
        if (active_version && active_version->parent) {
            active_version = active_version->parent;
        } else {
            std::cout << "No parent version to rollback to."<<std::endl;
        }
    } else {  // rollback to specific version
        TreeNode* target = nullptr;
        if (version_map.find(version_id, target) && target != nullptr) {
            active_version = target;
        } else {
            std::cout << "Version ID: " << version_id << " not found.\n";
        }
    }
}
//think about how to handle cases if the version id isnt an acestor but still present in a tree

// Print snapshot history from root to active version using getVersionPath()
void File::history()  {
    if (!active_version) return;
    std::vector<TreeNode*> path = getVersionPath(active_version->version_id);
    std::vector<TreeNode*> snapshots;
    for (auto node : path) {
        if (node->isSnapshot()) {
            snapshots.push_back(node);
        }
    }
    print(snapshots);
}

// Return filename string
const std::string& File::getName() const {
    return name;
}

// Find version node by ID using version_map
TreeNode* File::findVersion(int version_id) {
    TreeNode* node = nullptr;
    if (version_map.find(version_id, node)) {
        return node;
    }
    std::cout<<"NOT FOUND"<<std::endl;
    return nullptr;  // version not found
}

//get full path from root to given version ID
std::vector<TreeNode*> File::getVersionPath(int version_id) {
    TreeNode* node = findVersion(version_id);
    if (node == nullptr) return {};
    return node->rootpath();  // using original TreeNode method name
}

// function to print details, defined such for better modularity
void File::print(const std::vector<TreeNode*>& nodes) const {
        for (TreeNode* node : nodes) {
            if (node) {
                std::cout << "Version ID: " << node->version_id
                          << " | Message: " << node->message
                          << " | Updated: " << std::ctime(&node->created_timestamp)<<std::endl;
            }
        }
    }
// Return all leaf version nodes
std::vector<TreeNode*> File::getLeafVersions() const {
    std::vector<TreeNode*> leaves;
    if (!root) return leaves;
    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        if (current->childCount() == 0) {
            leaves.push_back(current);
        } else {
            for (TreeNode* child : current->children) {
                q.push(child);
            }
        }
    }
    return leaves;
}

#endif // FILE_HPP
