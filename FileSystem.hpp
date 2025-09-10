#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <iostream>
#include <queue>
#include "File.hpp"
#include "HashMap.hpp"
#include "Heap.hpp"

class FileSystem {
private:
    int untitledCounter = 0;
    HashMap<std::string, File*> files_map;
    Heap recent_files_heap;
    Heap biggest_trees_heap;
    std::queue<std::string> recentFilesQueue;
    std::string generateUntitledName();

public:
    FileSystem();
    ~FileSystem();

    void createFile(const std::string& filename);
    void createFile();

    void readFile(const std::string& filename);
    void insertIntoFile(const std::string& filename, const std::string& content);
    void updateFile(const std::string& filename, const std::string& content);
    void snapshotFile(const std::string& filename, const std::string& message);
    void rollbackFile(const std::string& filename, int version_id = -1);
    void showHistory(const std::string& filename);

    void recentFiles(int num);
    void biggestTrees(int num);
    void listFiles();

    // Rename file and update internal structures, returns success
    bool renameFile(const std::string& oldName, const std::string& newName);

    void accessedFile(const std::string& filename);


};

// Constructor
FileSystem::FileSystem() {
    // Initialization
}

// Destructor
FileSystem::~FileSystem() {
    files_map.iterate([this](const std::string& key, File* filePtr) {
        delete filePtr;
    });
}

// Create named file
void FileSystem::createFile(const std::string& filename) {
    File* existingFile = nullptr;
    if (files_map.find(filename, existingFile)) {
        std::cout << "File '" << filename << "' already exists.\n";
        return nullptr;
    }
    File* newFile = new File(filename);
    files_map.insert(filename, newFile);
    recent_files_heap.insert(filename, int(newFile->read().size()));
    biggest_trees_heap.insert(filename, newFile->total_versions);
    return newFile;
}

// Create untitled file
void FileSystem::createFile() {
    std::string name = generateUntitledName();
    return createFile(name);  // Reuse named createFile
}

// Rename file: returns true if successful, false if newName already exists or oldName missing
bool FileSystem::renameFile(const std::string& oldName, const std::string& newName) {
    File* file = nullptr;

    if (!files_map.find(oldName, file)) {
        std::cout << "File '" << oldName << "' not found.\n";
        return false;
    }
    if (files_map.find(newName, file)) {
        std::cout << "File '" << newName << "' already exists.\n";
        return false;
    }

    // Remove old mapping
    files_map.remove(oldName);

    // Rename file object internally
    file->rename(newName);

    // Insert new mapping
    files_map.insert(newName, file);

    // Update heaps — remove old, insert new keys
    recent_files_heap.remove(oldName);  // Assuming remove method exists
    biggest_trees_heap.remove(oldName);

    recent_files_heap.insert(newName, static_cast<int>(file->read().size())); 
    biggest_trees_heap.insert(newName, file->total_versions);

    std::cout << "File renamed from '" << oldName << "' to '" << newName << "'\n";

    return true;
}

// Other methods (readFile, insertIntoFile, etc.) stay as before with no changes

// generate untitled name
std::string FileSystem::generateUntitledName() {
    return "untitled" + std::to_string(++untitledCounter);
}

#endif // FILESYSTEM_HPP
