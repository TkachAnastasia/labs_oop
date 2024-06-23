#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Company {
    std::string officialName;
    std::vector<std::string> synonyms;
    int mentions;
    std::vector<std::string> filesMentioned;

public:
    Company(const std::string& name, const std::vector<std::string>& syns)
        : officialName(name), synonyms(syns), mentions(0) {}

    const std::string& getOfficialName() const {
        return officialName;
    }

    const std::vector<std::string>& getSynonyms() const {
        return synonyms;
    }

    void addMention(const std::string& filename) {
        mentions++;
        filesMentioned.push_back(filename);
    }

    int getMentions() const {
        return mentions;
    }

    json toJson() const {
        json j;
        j["official_name"] = officialName;
        j["synonyms"] = synonyms;
        j["mentions"] = mentions;
        j["files_mentioned"] = filesMentioned;
        return j;
    }
};

#include <algorithm>
#include <sstream>
#include <vector>
#include <string>

class TextAnalyzer {
public:
    static std::string preprocessText(const std::string& text) {
        std::string processedText = text;
        std::transform(processedText.begin(), processedText.end(), processedText.begin(), ::tolower);
        processedText.erase(std::remove_if(processedText.begin(), processedText.end(), ::ispunct), processedText.end());
        return processedText;
    }

    static int countMentions(const std::string& text, const std::string& keyword) {
        int count = 0;
        std::string word;
        std::istringstream stream(text);
        while (stream >> word) {
            if (word == keyword) {
                count++;
            }
        }
        return count;
    }
};

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class FileManager {
public:
    static std::vector<std::string> readTextFiles(const std::string& directoryPath) {
        std::vector<std::string> files;
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                files.push_back(entry.path().string());
            }
        }
        return files;
    }

    static std::string readFileContent(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file " + filePath);
        }

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return content;
    }
};

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class ReportGenerator {
public:
    static void generateReport(const std::vector<Company>& companies, const std::string& outputFilePath) {
        json report;
        for (const auto& company : companies) {
            report.push_back(company.toJson());
        }

        std::ofstream file(outputFilePath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file " + outputFilePath);
        }

        file << report.dump(4);
        file.close();
    }
};

#include <iostream>
#include <vector>

int main() {
    std::vector<Company> companies = {
        Company("Google", {"google", "goog"}),
        Company("Apple", {"apple", "appl"})
    };

    std::string directoryPath = "./texts";
    std::vector<std::string> files = FileManager::readTextFiles(directoryPath);

    for (const auto& file : files) {
        std::string content = FileManager::readFileContent(file);
        std::string processedText = TextAnalyzer::preprocessText(content);

        for (auto& company : companies) {
            for (const auto& synonym : company.getSynonyms()) {
                int mentions = TextAnalyzer::countMentions(processedText, synonym);
                if (mentions > 0) {
                    company.addMention(file);
                }
            }
        }
    }

    ReportGenerator::generateReport(companies, "report.json");

    std::cout << "Report generated: report.json" << std::endl;

    return 0;
}

