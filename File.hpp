#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <stack>
#include <iostream>  // for std::cout in history method
#include "TreeNode.hpp"
#include "HashMap.hpp"

class File {
private:
    std::string name;
    TreeNode* root;
    TreeNode* active_version;
    HashMap<int, TreeNode*> version_map;
    int total_versions;

    // Recursively delete all nodes in the version tree
    void deleteTree(TreeNode* node);

public:
    File(const std::string& filename);
    ~File();

    std::string read() const;
    void insert(const std::string& content);
    void update(const std::string& content);
    void snapshot(const std::string& message);
    void rollback(int version_id = -1);
    void history() const;
    const std::string& getName() const;
    void printLeafVersions() const;

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
    if (!active_version) return;

    if (active_version->isSnapshot()) {
        // Create new child version
        TreeNode* new_node = new TreeNode(total_versions, active_version->content, active_version);
        new_node->updateContent(new_node->content + content);
        active_version->addChild(new_node);
        active_version = new_node;
        version_map.insert(total_versions, new_node);
        ++total_versions;
    } else {
        // Modify content in place
        active_version->updateContent(active_version->content + content);
    }
}

// Replace content of active version or create new child if snapshot
void File::update(const std::string& content) {
    if (!active_version) return;

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
void File::snapshot(const std::string& message) {
    if (!active_version) return;

    active_version->updateMessage(message);
    active_version->snapshot_timestamp = std::time(nullptr);
}

// Rollback to specified version or parent if no version specified
void File::rollback(int version_id) {
    if (version_id == -1) {  // rollback to parent
        if (active_version && active_version->parent) {
            active_version = active_version->parent;
        } else {
            std::cout << "No parent version to rollback to.\n";
        }
    } else {  // rollback to specific version
        TreeNode* target = nullptr;
        if (version_map.find(version_id, target) && target != nullptr) {
            active_version = target;
        } else {
            std::cout << "Version ID " << version_id << " not found.\n";
        }
    }
}

// Print all snapshotted versions from root to active version chronologically
void File::history() const {
    if (!active_version) return;

    std::stack<TreeNode*> snapshots;
    TreeNode* curr = active_version;

    while (curr) {
        if (curr->isSnapshot())
            snapshots.push(curr);
        curr = curr->parent;
    }

    while (!snapshots.empty()) {
        TreeNode* node = snapshots.top();
        snapshots.pop();
        std::cout << "Version " << node->version_id 
                  << " - " << std::ctime(&node->snapshot_timestamp)
                  << " - " << node->message << std::endl;
    }
}

// Return filename string
const std::string& File::getName() const {
    return name;
}

void File::printLeafVersions() const {
    if (!root) return;

    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();

        if (current->childCount() == 0) {
            std::cout << "Version ID: " << current->version_id
                      << " | Message: " << current->message
                      << " | Updated: " << std::ctime(&current->created_timestamp);
        } else {
            for (TreeNode* child : current->children) {
                q.push(child);
            }
        }
    }
}


#endif // FILE_HPP
