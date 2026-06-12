#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

// ============================================================
//  File I/O - Reading, writing, appending, CSV parsing, and
//  basic file statistics
// ============================================================

// Write a vector of lines to a file, overwriting any existing content
void writeFile(const string& filename, const vector<string>& lines) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }
    for (const string& line : lines)
        file << line << "\n";
    file.close();
    cout << "Written " << lines.size() << " lines to " << filename << endl;
}

// Append additional lines to an existing file
void appendToFile(const string& filename, const vector<string>& lines) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Error opening file for appending: " << filename << endl;
        return;
    }
    for (const string& line : lines)
        file << line << "\n";
    file.close();
    cout << "Appended " << lines.size() << " lines to " << filename << endl;
}

// Read all lines from a file into a vector of strings
vector<string> readFile(const string& filename) {
    vector<string> lines;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for reading: " << filename << endl;
        return lines;
    }
    string line;
    while (getline(file, line))
        lines.push_back(line);
    file.close();
    return lines;
}

// Read the entire contents of a file into a single string
string readFileAsString(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return "";
    }
    ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

// Count the number of lines, words, and characters in a file (like `wc`)
struct FileStats {
    long lineCount = 0;
    long wordCount = 0;
    long charCount = 0;
};

FileStats getFileStats(const string& filename) {
    FileStats stats;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return stats;
    }
    string line;
    while (getline(file, line)) {
        stats.lineCount++;
        stats.charCount += (long)line.size() + 1; // +1 for newline
        istringstream iss(line);
        string word;
        while (iss >> word) stats.wordCount++;
    }
    return stats;
}

// Write a simple CSV file from a header row and rows of values
void writeCSV(const string& filename, const vector<string>& header, const vector<vector<string>>& rows) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening CSV for writing: " << filename << endl;
        return;
    }
    for (size_t i = 0; i < header.size(); i++) {
        file << header[i];
        if (i + 1 < header.size()) file << ",";
    }
    file << "\n";
    for (const auto& row : rows) {
        for (size_t i = 0; i < row.size(); i++) {
            file << row[i];
            if (i + 1 < row.size()) file << ",";
        }
        file << "\n";
    }
    file.close();
    cout << "Wrote CSV with " << rows.size() << " data rows to " << filename << endl;
}

// Split a string by a delimiter character
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

// Read a CSV file into a header row and a vector of data rows
pair<vector<string>, vector<vector<string>>> readCSV(const string& filename) {
    vector<string> header;
    vector<vector<string>> rows;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening CSV for reading: " << filename << endl;
        return {header, rows};
    }

    string line;
    bool first = true;
    while (getline(file, line)) {
        vector<string> fields = split(line, ',');
        if (first) {
            header = fields;
            first = false;
        } else {
            rows.push_back(fields);
        }
    }
    return {header, rows};
}

// Check if a file exists by attempting to open it
bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// Copy contents from one file to another
bool copyFile(const string& source, const string& destination) {
    ifstream src(source, ios::binary);
    if (!src.is_open()) {
        cerr << "Cannot open source file: " << source << endl;
        return false;
    }
    ofstream dst(destination, ios::binary);
    if (!dst.is_open()) {
        cerr << "Cannot open destination file: " << destination << endl;
        return false;
    }
    dst << src.rdbuf();
    return true;
}

int main() {
    cout << "=== Basic Write and Read ===" << endl;
    vector<string> data = {"Hello, World!", "Second line", "Third line"};
    writeFile("output.txt", data);

    vector<string> result = readFile("output.txt");
    cout << "Read " << result.size() << " lines:" << endl;
    for (const string& line : result)
        cout << "  " << line << endl;

    cout << endl << "=== Appending Data ===" << endl;
    vector<string> moreData = {"Fourth line", "Fifth line"};
    appendToFile("output.txt", moreData);
    result = readFile("output.txt");
    cout << "After append, file has " << result.size() << " lines:" << endl;
    for (const string& line : result)
        cout << "  " << line << endl;

    cout << endl << "=== Reading File As a Single String ===" << endl;
    string fullText = readFileAsString("output.txt");
    cout << "Full text length: " << fullText.size() << " characters" << endl;

    cout << endl << "=== File Statistics ===" << endl;
    FileStats stats = getFileStats("output.txt");
    cout << "Lines: " << stats.lineCount << endl;
    cout << "Words: " << stats.wordCount << endl;
    cout << "Characters (incl. newlines): " << stats.charCount << endl;

    cout << endl << "=== CSV Writing and Reading ===" << endl;
    vector<string> csvHeader = {"name", "age", "city"};
    vector<vector<string>> csvRows = {
        {"Alice", "30", "New York"},
        {"Bob", "25", "Los Angeles"},
        {"Charlie", "35", "Chicago"}
    };
    writeCSV("people.csv", csvHeader, csvRows);

    auto [readHeader, readRows] = readCSV("people.csv");
    cout << "CSV Header: ";
    for (const string& col : readHeader) cout << col << " | ";
    cout << endl;

    cout << "CSV Rows:" << endl;
    for (const auto& row : readRows) {
        cout << "  ";
        for (const string& field : row) cout << field << " | ";
        cout << endl;
    }

    cout << endl << "=== Searching CSV by Column ===" << endl;
    // Find the index of the "age" column and compute the average
    auto ageIt = find(readHeader.begin(), readHeader.end(), "age");
    if (ageIt != readHeader.end()) {
        size_t ageIdx = distance(readHeader.begin(), ageIt);
        double totalAge = 0;
        for (const auto& row : readRows) totalAge += stod(row[ageIdx]);
        cout << "Average age: " << (totalAge / readRows.size()) << endl;
    }

    cout << endl << "=== File Existence and Copying ===" << endl;
    cout << "output.txt exists: " << (fileExists("output.txt") ? "yes" : "no") << endl;
    cout << "does_not_exist.txt exists: " << (fileExists("does_not_exist.txt") ? "yes" : "no") << endl;

    if (copyFile("output.txt", "output_copy.txt")) {
        cout << "Copied output.txt to output_copy.txt" << endl;
        vector<string> copyContents = readFile("output_copy.txt");
        cout << "Copy has " << copyContents.size() << " lines" << endl;
    }

    cout << endl << "=== Error Handling for Missing Files ===" << endl;
    vector<string> missing = readFile("this_file_does_not_exist.txt");
    cout << "Lines read from missing file: " << missing.size() << endl;

    return 0;
}