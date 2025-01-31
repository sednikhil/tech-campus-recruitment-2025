#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

long getFileSize(std::ifstream& file) {
    file.seekg(0, std::ios::end);
    long size = file.tellg();
    file.seekg(0, std::ios::beg);
    return size;
}

long findStartPosition(std::ifstream& file, const std::string& targetDate, long fileSize) {
    long low = 0;
    long high = fileSize - 1;
    long result = fileSize;

    while (low <= high) {
        long mid = low + (high - low) / 2;
        file.seekg(mid);

        if (mid != 0) {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        long lineStart = file.tellg();
        if (lineStart >= fileSize) {
            high = mid - 1;
            continue;
        }

        std::string line;
        std::getline(file, line);
        if (line.empty()) {
            high = mid - 1;
            continue;
        }

        std::string currentDate = line.substr(0, 10);

        if (currentDate < targetDate) {
            low = file.tellg();
        } else {
            if (currentDate == targetDate) {
                result = lineStart;
            }
            high = mid - 1;
        }
    }

    return result;
}

long findEndPosition(std::ifstream& file, const std::string& targetDate, long fileSize) {
    long low = 0;
    long high = fileSize - 1;
    long result = fileSize;

    while (low <= high) {
        long mid = low + (high - low) / 2;
        file.seekg(mid);

        if (mid != 0) {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        long lineStart = file.tellg();
        if (lineStart >= fileSize) {
            high = mid - 1;
            continue;
        }

        std::string line;
        std::getline(file, line);
        if (line.empty()) {
            high = mid - 1;
            continue;
        }

        std::string currentDate = line.substr(0, 10);

        if (currentDate <= targetDate) {
            low = file.tellg();
        } else {
            result = lineStart;
            high = mid - 1;
        }
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " YYYY-MM-DD" << std::endl;
        return 1;
    }

    std::string targetDate = argv[1];
    std::ifstream file("test_logs.log", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return 1;
    }

    long fileSize = getFileSize(file);
    long startPos = findStartPosition(file, targetDate, fileSize);
    long endPos = findEndPosition(file, targetDate, fileSize);

    fs::create_directories("output");
    std::ofstream outputFile("output/output_" + targetDate + ".txt");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    if (startPos >= endPos) {
        return 0;
    }

    file.clear();
    file.seekg(startPos);
    std::streampos currentPos = file.tellg();
    while (currentPos < endPos && !file.eof()) {
        std::string line;
        std::getline(file, line);
        outputFile << line << '\n';
        currentPos = file.tellg();
    }

    file.close();
    outputFile.close();

    return 0;
}