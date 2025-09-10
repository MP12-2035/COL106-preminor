#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

class File;

class TreeNode {
    friend class File;

private:
    int version_id;
    std::string content;
    std::string message;

    const time_t created_timestamp;     
    time_t last_modified_timestamp;    
    time_t snapshot_timestamp;

    TreeNode* parent;
    std::vector<TreeNode*> children;

public:
    // Constructors
    TreeNode(int id, const std::string& cont, TreeNode* par);
    TreeNode(int id, const std::string& cont);
    TreeNode(int id);
    TreeNode();

    // Rule of three: disable copying to avoid shallow copies & double deletes
    TreeNode(const TreeNode&) = delete;
    TreeNode& operator=(const TreeNode&) = delete;

    ~TreeNode();

    void addChild(TreeNode* child);

    // Removes and deletes child to avoid memory leaks
    bool removeChild(TreeNode* child);

    int childCount() const;

    std::vector<TreeNode*> rootpath();

    bool isSnapshot() const;

    void updateContent(const std::string& newContent);

    void updateMessage(const std::string& newMessage);

    // Accessors for timestamps
    time_t getCreatedTimestamp() const { return created_timestamp; }
    time_t getLastModifiedTimestamp() const { return last_modified_timestamp; }
    void setSnapshotTimestamp(time_t t) { snapshot_timestamp = t; }
    time_t getSnapshotTimestamp() const { return snapshot_timestamp; }
};

// Implementation

TreeNode::TreeNode(int id, const std::string& cont, TreeNode* par)
    : version_id(id), content(cont), parent(par), message(""), snapshot_timestamp(0),
      created_timestamp(std::time(nullptr)), last_modified_timestamp(created_timestamp) {}

TreeNode::TreeNode(int id, const std::string& cont)
    : TreeNode(id, cont, nullptr) {}

TreeNode::TreeNode(int id)
    : TreeNode(id, "", nullptr) {}

TreeNode::TreeNode()
    : TreeNode(0, "", nullptr) {}

TreeNode::~TreeNode() {
    for (TreeNode* child : children) {
        delete child;
    }
    children.clear();
}

void TreeNode::addChild(TreeNode* child) {
    if (child) {
        child->parent = this;
        children.push_back(child);
    }
}

bool TreeNode::removeChild(TreeNode* child) {
    for (auto it = children.begin(); it != children.end(); ++it) {
        if (*it == child) {
            children.erase(it);
            child->parent = nullptr;
            delete child;  // Safe delete to avoid leaks
            return true;
        }
    }
    return false;
}

int TreeNode::childCount() const {
    return static_cast<int>(children.size());
}

std::vector<TreeNode*> TreeNode::rootpath() {
    std::vector<TreeNode*> path;
    TreeNode* current = this;
    while (current) {
        path.push_back(current);
        current = current->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

bool TreeNode::isSnapshot() const {
    return snapshot_timestamp != 0;
}

void TreeNode::updateContent(const std::string& newContent) {
    content = newContent;
    last_modified_timestamp = std::time(nullptr);
}

void TreeNode::updateMessage(const std::string& newMessage) {
    message = newMessage;
    last_modified_timestamp = std::time(nullptr);
}

#endif // TREENODE_HPP
