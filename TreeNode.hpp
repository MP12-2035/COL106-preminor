#ifndef TREENODE_HPP
#define TREENODE_HPP
#include <string>
#include <vector>
#include <queue>
#include <ctime>
using namespace std;

class File;

class TreeNode { //TreeNode ADT
    friend class File;
private:
    int version_id;
    std::string content;
    std::string message;
    time_t created_timestamp;
    time_t snapshot_timestamp;
    TreeNode* parent;
    std::vector<TreeNode*> children;

public:
    TreeNode(int id, const std::string& cont, TreeNode* par);
    TreeNode(int id, const std::string& cont);
    TreeNode(int id);
    TreeNode();
//constructors with varying levels of input data

    ~TreeNode();

    void addChild(TreeNode* child);
    bool removeChild(TreeNode* child);
    int childCount() const;
    // TreeNode* findNode(int id);
    std::vector<TreeNode*> rootpath();
    bool isSnapshot() const;
    void updateContent(const std::string& newContent);
    void updateMessage(const std::string& newMessage);
};

// the implementation
//constructors
TreeNode::TreeNode(int id, const std::string& cont, TreeNode* par)
    : version_id(id), content(cont), parent(par), message(""), snapshot_timestamp(0) {
    created_timestamp = std::time(nullptr);
}

TreeNode::TreeNode(int id, const std::string& cont)
    : version_id(id), content(cont), parent(nullptr), message(""), snapshot_timestamp(0) {
    created_timestamp = std::time(nullptr);
}

TreeNode::TreeNode(int id)
    : version_id(id), content(""), parent(nullptr), message(""), snapshot_timestamp(0) {
    created_timestamp = std::time(nullptr);
}

TreeNode::TreeNode()
    : version_id(0), content(""), parent(nullptr), message(""), snapshot_timestamp(0) {
    created_timestamp = std::time(nullptr);
}

//destructor
TreeNode::~TreeNode() {
    for (TreeNode* child : children) {
        delete child;
    }
    children.clear();
}

//member functions
void TreeNode::addChild(TreeNode* child) {
    if (child) {
        child->parent = this;
        children.push_back(child);
    }
}

bool TreeNode::removeChild(TreeNode* child) {
    for (auto i = children.begin(); i != children.end(); ++i) {
        if (*i == child) {
            children.erase(i);
            child->parent = nullptr;
            delete child;
            return true;
        }
    }
    return false;
}

int TreeNode::childCount() const {
    return (int)children.size();
}

// TreeNode* TreeNode::findNode(int id) { //iterative bfs
//     std::queue<TreeNode*> q;
//     q.push(this);  
    
//     while (!q.empty()) {
//         TreeNode* current = q.front();
//         q.pop();

//         if (current->version_id == id) {
//             return current;
//         }

//         for (TreeNode* child : current->children) {
//             q.push(child);
//         }
//     }
//     return nullptr;  
// }

std::vector<TreeNode*> TreeNode::rootpath() {
    std::vector<TreeNode*> path;
    TreeNode* current = this;
    while (current) {
        path.insert(path.begin(), current);
        current = current->parent;
    }
    return path;
}

bool TreeNode::isSnapshot() const {
    return snapshot_timestamp != 0;
}

void TreeNode::updateContent(const std::string& newContent) {
    content = newContent;
    created_timestamp = std::time(nullptr);
}

void TreeNode::updateMessage(const std::string& newMessage) {
    message = newMessage;
    created_timestamp = std::time(nullptr);
}



#endif // TREENODE_HPP
