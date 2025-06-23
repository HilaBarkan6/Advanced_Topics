#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

#ifdef _WIN32
#include <cstdlib>
#else
#include <cstdlib>
#endif

/* We used ChatGPT to learn how work with GTest*/

// Structure to hold the file paths for one test case
struct GameTestParam {
    std::string input_file;
    std::string expected_output_file;
};

// Helper function to run the game and read the actual output from the generated file
std::string runGameAndCaptureOutput(const std::string& input_file) {
    // Extract just the filename from the full input path
    std::string base_name = input_file.substr(input_file.find_last_of("/\\") + 1);

    // Construct corresponding output file path
    std::string output_file = "output/output_" + base_name;

#ifdef _WIN32
    // Windows: run the game executable with the input file
    const std::string command = "tank_game.exe " + input_file + " > nul 2>&1"; // Redirect output to null
    int exit_code = system(command.c_str());
#else
    // Linux/Mac: run the executable normally
    const std::string command = "./tank_game.exe " + input_file + " > nul 2>&1"; // Redirect output to null
    int exit_code = system(command.c_str());
#endif

    
    if (exit_code != 0) {
        return "ERROR";
    }

    std::ifstream file(output_file);
    if (!file.is_open()) {
        return "ERROR";
    }

    std::string line, last_non_empty_line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            last_non_empty_line = line;
        }
    }

    return last_non_empty_line;
}

// Helper function to read the expected output file
std::string readExpectedOutput(const std::string& expected_file) {
    std::ifstream file(expected_file);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// Helper to extract the last non-empty line from a string
std::string getLastNonEmptyLine(const std::string& text) {
    std::istringstream iss(text);
    std::string line, lastLine;
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            lastLine = line;
        }
    }
    return lastLine;
}

// Define the test fixture for parameterized tests
class GameTests : public ::testing::TestWithParam<GameTestParam> {};

TEST_P(GameTests, CompareLastLine) {
    GameTestParam param = GetParam();

    std::string actual = runGameAndCaptureOutput(param.input_file);
    std::string expected_full = readExpectedOutput(param.expected_output_file);
    std::string expected = getLastNonEmptyLine(expected_full);

    // Compare the actual and expected last lines
    EXPECT_EQ(actual, expected);
}

// Helper function to generate all test file pairs from input_a.txt to input_j.txt
std::vector<GameTestParam> generateTestCases() {
    std::vector<GameTestParam> tests;
    for (char c = 'a'; c <= 'l'; ++c) {
        if (c == 'f') continue; // test 'f' should result in "ERROR", so we skip it
        std::string letter(1, c);
        tests.push_back({
            "test/input_tests/input_" + letter + ".txt",
            "test/expected_output/expected_output_input_" + letter + ".txt"
        });
    }
    return tests;
}

// Instantiate all test cases from the list generated
INSTANTIATE_TEST_CASE_P(
    AllInputs,             
    GameTests,         
    ::testing::ValuesIn(generateTestCases()) 
);


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(color) = "yes";  
    return RUN_ALL_TESTS();
}