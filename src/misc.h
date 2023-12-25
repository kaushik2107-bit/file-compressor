#pragma once

#include <random>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <iterator>

inline std::string gen_filename(int length, const std::string& extension) {
    static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int charsetSize = sizeof(charset) - 1;  // Excluding null terminator

    srand(static_cast<unsigned int>(time(nullptr)));

    std::string randomFileName;
    for (int i = 0; i < length; ++i) {
        randomFileName += charset[rand() % charsetSize];
    }

    return randomFileName + extension;
}

inline bool compare_files(const std::string& filename1, const std::string& filename2) {
    std::ifstream file1(filename1, std::ifstream::ate | std::ifstream::binary);
    std::ifstream file2(filename2, std::ifstream::ate | std::ifstream::binary);
    const std::ifstream::pos_type fileSize = file1.tellg();

    if (fileSize != file2.tellg()) {
        return false;
    }

    file1.seekg(0);
    file2.seekg(0);

    std::istreambuf_iterator<char> begin1(file1);
    std::istreambuf_iterator<char> begin2(file2);

    return std::equal(begin1,std::istreambuf_iterator<char>(),begin2);
}
