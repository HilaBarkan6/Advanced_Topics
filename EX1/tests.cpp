#include "src/GameManager.h"
#include "src/RotatingPlayer.h"
#include "src/SimplePlayer.h"
#include "src/SecondPlayer.h"
#include "src/DefensivePlayer.h"
#include "src/OffensivePlayer.h"
#include <iostream>
#include <memory>
#include <string>

int main() {
    try {
        
        // Game 2
        std::unique_ptr<Player> p2_1(new SimplePlayer());
        std::unique_ptr<Player> p2_2(new SimplePlayer());
        std::string input2 = "C:/Projects/Advanced_Topics/EX1/input/input_2.txt";
        std::string output2 = "output/output_input_2.txt";
        GameManager gm2(std::move(p2_1), std::move(p2_2), input2, output2);
        gm2.runGame();

        // Game 3
        std::unique_ptr<Player> p3_1(new SimplePlayer());
        std::unique_ptr<Player> p3_2(new SimplePlayer());
        std::string input3 = "C:/Projects/Advanced_Topics/EX1/input/input_3.txt";
        std::string output3 = "output/output_input_3.txt";
        GameManager gm3(std::move(p3_1), std::move(p3_2), input3, output3);
        gm3.runGame();

        // Game 4
        std::unique_ptr<Player> p4_1(new SecondPlayer());
        std::unique_ptr<Player> p4_2(new SecondPlayer());
        std::string input4 = "C:/Projects/Advanced_Topics/EX1/input/input_4.txt";
        std::string output4 = "output/output_input_4.txt";
        GameManager gm4(std::move(p4_1), std::move(p4_2), input4, output4);
        gm4.runGame();

        // Game 5
        std::unique_ptr<Player> p5_1(new SecondPlayer());
        std::unique_ptr<Player> p5_2(new RotatingPlayer());
        std::string input5 = "C:/Projects/Advanced_Topics/EX1/input/input_5.txt";
        std::string output5 = "output/output_input_5.txt";
        GameManager gm5(std::move(p5_1), std::move(p5_2), input5, output5);
        gm5.runGame();

        // Game 6
        std::unique_ptr<Player> p6_1(new SimplePlayer());
        std::unique_ptr<Player> p6_2(new SecondPlayer());
        std::string input6 = "C:/Projects/Advanced_Topics/EX1/input/input_6.txt";
        std::string output6 = "output/output_input_6.txt";
        GameManager gm6(std::move(p6_1), std::move(p6_2), input6, output6);
        gm6.runGame();

        // Game 7
        std::unique_ptr<Player> p7_1(new RotatingPlayer());
        std::unique_ptr<Player> p7_2(new OffensivePlayer());
        std::string input7 = "C:/Projects/Advanced_Topics/EX1/input/input_7.txt";
        std::string output7 = "output/output_input_7.txt";
        GameManager gm7(std::move(p7_1), std::move(p7_2), input7, output7);
        gm7.runGame();

        // Game 1
        std::unique_ptr<Player> p1_1(new RotatingPlayer());
        std::unique_ptr<Player> p1_2(new RotatingPlayer());
        std::string input1 = "C:/Projects/Advanced_Topics/EX1/input/input_1.txt";
        std::string output1 = "output/output_input_1.txt";
        GameManager gm1(std::move(p1_1), std::move(p1_2), input1, output1);
        gm1.runGame();

        // // Game 8
        // std::unique_ptr<Player> p8_1(new SimplePlayer());
        // std::unique_ptr<Player> p8_2(new OffensivePlayer());
        // std::string input8 = "C:/Projects/Advanced_Topics/EX1/input/input_8.txt";
        // std::string output8 = "output/output_input_8.txt";
        // GameManager gm8(std::move(p8_1), std::move(p8_2), input8, output8);
        // gm8.runGame();

        // // Game 9
        // std::unique_ptr<Player> p9_1(new DefensivePlayer());
        // std::unique_ptr<Player> p9_2(new SecondPlayer());
        // std::string input9 = "C:/Projects/Advanced_Topics/EX1/input/input_9.txt";
        // std::string output9 = "output/output_input_9.txt";
        // GameManager gm9(std::move(p9_1), std::move(p9_2), input9, output9);
        // gm9.runGame();

        // // Game 10
        // std::unique_ptr<Player> p10_1(new SimplePlayer());
        // std::unique_ptr<Player> p10_2(new SimplePlayer());
        // std::string input10 = "C:/Projects/Advanced_Topics/EX1/input/input_10.txt";
        // std::string output10 = "output/output_input_10.txt";
        // GameManager gm10(std::move(p10_1), std::move(p10_2), input10, output10);
        // gm10.runGame();

    } catch (const std::exception& e) {
        std::cerr << "Cannot recover from error, finishing game." << std::endl;
        return 1;
    }

    return 0;
}