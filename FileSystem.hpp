#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include "File.hpp"
#include "HashMap.hpp"
#include "Heap.hpp"

class FileSystem {
public:
    FileSystem();
    ~FileSystem();

    void createFile(const std::string& filename);
    void readFile(const std::string& filename);
    void insertIntoFile(const std::string& filename, const std::string& content);
    void updateFile(const std::string& filename, const std::string& content);
    void snapshotFile(const std::string& filename, const std::string& message);
    void rollbackFile(const std::string& filename, int version_id = -1);
    void showHistory(const std::string& filename);
    void recentFiles(int num);
    void biggestTrees(int num);
// list files, an array with filenames mapped to the root version of them


private:
    HashMap<std::string, File*> files_map;
    Heap recent_files_heap;
    Heap biggest_trees_heap;

    // Additional helper methods
};

#endif // FILESYSTEM_HPP
