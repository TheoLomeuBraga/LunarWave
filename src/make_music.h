#pragma once

#include "lua_extra.h"
#include <vector>
#include <iostream>

#include "sndfile/sndfile.h"
#include <SFML/Audio.hpp>

#ifdef _WIN32
#include <windows.h>
#include <string>

std::string getTemporaryFileName()
{
    char tempDir[MAX_PATH];
    DWORD result = GetTempPathA(MAX_PATH, tempDir);
    if (result == 0 || result > MAX_PATH)
    {
        // Erro ao obter o diretório temporário
        return "";
    }
    string(tempDir) + "/temporary_lunarwave.wav";
}
#else
#include <cstdlib> // Para std::getenv
#include <string>

std::string getTemporaryFileName()
{
    const char *tempDir = std::getenv("TMPDIR");
    if (tempDir == nullptr || tempDir[0] == '\0')
    {
        // Caso TMPDIR não esteja definido, use /tmp como padrão
        tempDir = "/tmp";
    }
    return std::string(tempDir) + "/temporary_lunarwave.wav";
}

#endif

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (256)

int test_create_song(std::string path)
{
    // Definir parâmetros do arquivo de áudio
    SF_INFO fileInfo;
    fileInfo.samplerate = SAMPLE_RATE;                  // Taxa de amostragem
    fileInfo.channels = 2;                              // Número de canais (estéreo)
    fileInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; // Formato de áudio (WAV, 16 bits PCM)

    // Abrir um novo arquivo de áudio para escrita
    SNDFILE *sndFile = sf_open(path.c_str(), SFM_WRITE, &fileInfo);
    if (!sndFile)
    {
        std::cerr << "Erro ao criar o arquivo de áudio" << std::endl;
        return 1;
    }

    // Criar dados de áudio para a amostra
    std::vector<short> audioData(fileInfo.channels * 44100, 0); // Amostra de 1 segundo de áudio em silêncio

    // Gravar os dados de áudio no arquivo
    sf_writef_short(sndFile, audioData.data(), audioData.size());

    // Fechar o arquivo de áudio
    sf_close(sndFile);

    std::cout << "Amostra de áudio criada com sucesso" << std::endl;

    return 0;
}

sf::SoundBuffer buffer;
sf::Sound sound;
int play_song(std::string path = getTemporaryFileName())
{

    if (!buffer.loadFromFile(path.c_str())) {
        return 1;
    }
    
    sound.setBuffer(buffer);
    sound.play();
    

    return 0;
}

void create_music(std::string song_script_path)
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    
    test_create_song(getTemporaryFileName());
    

    if (luaL_loadfile(L, song_script_path.c_str()) || lua_pcall(L, 0, 0, 0))
    {
        std::cerr << "Error in the script: " << lua_tostring(L, -1) << std::endl;
    }

    lua_close(L);

    play_song(getTemporaryFileName());
}

void save_music(std::string output_path){

}

