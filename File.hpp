#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include "TreeNode.hpp"
#include "HashMap.hpp"

class File {
public:
    File(const std::string& filename);
    ~File();

    void read() const;
    void insert(const std::string& content);
    void update(const std::string& content);
    void snapshot(const std::string& message);
    void rollback(int version_id = -1);
    void history() const;

    const std::string& getName() const;

private:
    std::string name;
    TreeNode* root;
    TreeNode* active_version;
    HashMap<int, TreeNode*> version_map;
    int total_versions;

    void deleteTree(TreeNode* node);
};

#endif // FILE_HPP
