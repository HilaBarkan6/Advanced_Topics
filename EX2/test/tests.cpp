#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <string>

#ifdef _WIN32
#include <cstdio>
#else
#include <stdio.h>
#endif

// Helper function to run the game and capture output
std::string runGameAndCaptureOutput(const std::string& input_file) {
#ifdef _WIN32
    //const std::string command = "main.exe " + input_file;  
    const std::string command = "main.exe " + input_file;
    FILE* pipe = _popen(command.c_str(), "r");
#else
    const std::string command = "./main.exe " + input_file; 
    FILE* pipe = popen(command.c_str(), "r");
#endif

    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif

    return result;
}

// Helper function to read expected output from file
std::string readExpectedOutput(const std::string& expected_file) {
    std::ifstream file(expected_file);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// Helper to get last non-empty line from a string
std::string getLastNonEmptyLine(const std::string& text) {
    std::istringstream iss(text);
    std::string line;
    std::string lastLine;
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            lastLine = line;
        }
    }
    return lastLine;
}

// Test cases for inputs A to J
TEST(TankGameTests, InputA) {
    std::string actual = runGameAndCaptureOutput("inputs/input_a.txt");
    std::string expected = readExpectedOutput("expected_output/expected_output_input_a.txt");

    std::string actualLastLine = getLastNonEmptyLine(actual);
    std::string expectedLastLine = getLastNonEmptyLine(expected);

    EXPECT_EQ(actualLastLine, expectedLastLine);
}

TEST(TankGameTests, InputB) {
    std::string actual = runGameAndCaptureOutput("inputs/input_b.txt");
    std::string expected = readExpectedOutput("expected_output/expected_output_input_b.txt");

    std::string actualLastLine = getLastNonEmptyLine(actual);
    std::string expectedLastLine = getLastNonEmptyLine(expected);

    EXPECT_EQ(actualLastLine, expectedLastLine);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}