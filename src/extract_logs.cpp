#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

void extractLogs(const string& logFilePath, const string& date) {
    ifstream logFile(logFilePath);
    if (!logFile.is_open()) {
        cerr << "Error: Unable to open log file." << endl;
        return;
    }
    
    string outputDir = "output";
    filesystem::create_directory(outputDir);
    string outputFilePath = outputDir + "/output_" + date + ".txt";
    ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to create output file." << endl;
        return;
    }
    
    string line;
    size_t dateLength = date.size();
    while (getline(logFile, line)) {
        if (line.compare(0, dateLength, date) == 0) {
            outputFile << line << endl;
        }
    }
    
    logFile.close();
    outputFile.close();
    cout << "Logs for " << date << " extracted to " << outputFilePath << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <log_file_path> <YYYY-MM-DD>" << endl;
        return 1;
    }
    
    string logFilePath = argv[1];
    string date = argv[2];
    extractLogs(logFilePath, date);
    return 0;
}