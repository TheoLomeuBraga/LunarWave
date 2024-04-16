
#include <iostream>
#include <string>
#include "commands.h"

bool file_played = false;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "no arguments" << std::endl;
        return 0;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-v") {
            version();
        } else if (arg == "-help") {
            help();
        } else if (arg == "-o") {
            if (i + 1 < argc) {
                std::string output_path(argv[i + 1]);
                if(!file_played){
                    std::cerr << "Error: place the music script before -o" << std::endl;
                    break;
                }
                output(output_path);
                ++i;  // Pule o próximo argumento, pois ele é o caminho de saída
            } else {
                std::cerr << "Error: no arguments after -o" << std::endl;
            }
        } else {
            play(arg);
            file_played = true;
        }
    }

    while (sound.getStatus() == sf::Sound::Playing) {}

    return 0;
}