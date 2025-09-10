#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <iostream>
#include <stack>
#include "File.hpp"
#include "HashMap.hpp"
#include "Heap.hpp"

class FileSystem {
private:
    int untitledCounter = 0;
    HashMap<std::string, File*> files_map;
    Heap biggest_trees_heap;
    std::stack<std::string> recentFilesStack;  // Now a stack for recent files

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
FileSystem::FileSystem() {}

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
        return;
    }
    File* newFile = new File(filename);
    files_map.insert(filename, newFile);
    biggest_trees_heap.insert(filename, newFile->total_versions);
    // Access count: push to stack
    accessedFile(filename);
}

// Create untitled file
void FileSystem::createFile() {
    std::string name = generateUntitledName();
    createFile(name);
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
    files_map.remove(oldName);
    file->rename(newName);
    files_map.insert(newName, file);
    biggest_trees_heap.remove(oldName);
    biggest_trees_heap.insert(newName, file->total_versions);
    std::cout << "File renamed from '" << oldName << "' to '" << newName << "'\n";
    return true;
}

// Generate untitled name
std::string FileSystem::generateUntitledName() {
    return "untitled" + std::to_string(++untitledCounter);
}

// --- File operations ---

void FileSystem::readFile(const std::string& filename) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    std::cout << file->read() << "\n";
    accessedFile(filename);  // Mark as recently accessed
}

void FileSystem::insertIntoFile(const std::string& filename, const std::string& content) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    file->insert(content);
    biggest_trees_heap.update(filename, file->total_versions);
    accessedFile(filename);
}

void FileSystem::updateFile(const std::string& filename, const std::string& content) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    file->update(content);
    biggest_trees_heap.update(filename, file->total_versions);
    accessedFile(filename);
}

void FileSystem::snapshotFile(const std::string& filename, const std::string& message) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    file->snapshot(message);
    biggest_trees_heap.update(filename, file->total_versions);
    accessedFile(filename);
}

void FileSystem::rollbackFile(const std::string& filename, int version_id) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    file->rollback(version_id);
    accessedFile(filename);
}

void FileSystem::showHistory(const std::string& filename) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    file->history();
}

// Print the N most recently accessed files (LIFO order)
void FileSystem::recentFiles(int num) {
    if (num <= 0) return;
    std::stack<std::string> tempStack = recentFilesStack;
    int count = 0;
    while (!tempStack.empty() && count < num) {
        std::cout << tempStack.top() << "\n";
        tempStack.pop();
        ++count;
    }
}

void FileSystem::biggestTrees(int num) {
    biggest_trees_heap.printTop(num);
}

void FileSystem::listFiles() {
    files_map.iterate([](const std::string& name, File* file) {
        std::cout << "File: " << name << "\n";
    });
}

// Mark as accessed: push to recent files stack
void FileSystem::accessedFile(const std::string& filename) {
    recentFilesStack.push(filename);
}

#endif // FILESYSTEM_HPP
