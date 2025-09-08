#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include "File.hpp"
#include "HashMap.hpp"
#include "Heap.hpp"
#include <iostream>
#include <queue>


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
    void listFiles();
    std::queue<std::string> recentFilesQueue;

    void accessedFile(const std::string& filename) {
        recentFilesQueue.push(filename);
    }

    void recentFiles(int num) {
        if (num <= 0) return;

        std::queue<std::string> tempQueue = recentFilesQueue;
        int count = 0;
        while (!tempQueue.empty() && count < num) {
            std::cout << tempQueue.front() << "\n";
            tempQueue.pop();
            ++count;
        }
    }



// list files, an array with filenames mapped to the root version of them


private:
    HashMap<std::string, File*> files_map;
    Heap recent_files_heap;
    Heap biggest_trees_heap;

    // Additional helper methods
};


FileSystem::FileSystem() {
    // Initialization if needed
}

FileSystem::~FileSystem() {
    // Clean up dynamically allocated Files
    files_map.iterate([this](const std::string& key, File* filePtr) {
        delete filePtr;
    });
}

// Create a new file if it doesn't exist
void FileSystem::createFile(const std::string& filename) {
    File* existingFile = nullptr;
    if (files_map.find(filename, existingFile)) {
        std::cout << "File '" << filename << "' already exists.\n";
        return;
    }
    File* newFile = new File(filename);
    files_map.insert(filename, newFile);

    // Insert into heaps: 
    // recently accessed heap keyed by current time of root snapshot
    recent_files_heap.insert(filename, static_cast<int>(newFile->read().size())); // or timestamp if you store in File
    // biggest trees keyed by total versions
    biggest_trees_heap.insert(filename, newFile->total_versions);
}

// Read active version content of a file
void FileSystem::readFile(const std::string& filename) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    std::cout << file->read() << "\n";

    // Optionally update recent_files_heap here for access time
    // You need a timestamp in File for this or size as usage proxy
}

// Insert new content into a file
void FileSystem::insertIntoFile(const std::string& filename, const std::string& content) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    file->insert(content);

    // Update heaps if version count or access time may have changed
    biggest_trees_heap.update(filename, file->total_versions);
    // Update recent_files_heap similarly if tracking time
}

// Update a file by replacing content in active version
void FileSystem::updateFile(const std::string& filename, const std::string& content) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    file->update(content);

    biggest_trees_heap.update(filename, file->total_versions);
    // update recent_files_heap similarly if tracking time
}

// Snapshot active version with message
void FileSystem::snapshotFile(const std::string& filename, const std::string& message) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    file->snapshot(message);

    biggest_trees_heap.update(filename, file->total_versions);
    // update recent_files_heap similarly if tracking time
}

// Rollback active version to specified or parent version
void FileSystem::rollbackFile(const std::string& filename, int version_id) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    file->rollback(version_id);

    // Consider recent_files_heap update if rollback counts as access
}

// Show snapshot history of a file
void FileSystem::showHistory(const std::string& filename) {
    File* file = nullptr;
    if (!files_map.find(filename, file)) {
        std::cout << "File '" << filename << "' not found.\n";
        return;
    }
    file->history();
}

// Print the N most recently accessed files
void FileSystem::recentFiles(int num) {
    recent_files_heap.printTop(num);
}

// Print the N files with biggest version trees
void FileSystem::biggestTrees(int num) {
    biggest_trees_heap.printTop(num);
}
#endif // FILESYSTEM_HPP
