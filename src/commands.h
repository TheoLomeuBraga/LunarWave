#pragma once
#include <iostream>
#include <string>
#include "make_music.h"


#ifndef VERSION
    #define VERSION "undefined"
#endif

void version(){
    std::cout << VERSION << std::endl;
}

void help(){
    std::cout << "help:" << std::endl;
    std::cout << "  -v : say the version" << std::endl;
    std::cout << "  -help : display the help menssage" << std::endl;
    std::cout << "  -o : define the output file" << std::endl;
}



void play(std::string song_script_path){
    std::cout << "playing: " << song_script_path << std::endl;
    create_music(song_script_path);
}

void output(std::string output_path){
    std::cout << "compiling to: " << output_path << std::endl;
    save_music(output_path);
}   