#pragma once

// #include "lua_extra.h"
#include "lua5.4/lua.hpp"
#include <vector>
#include <iostream>
#include <cstring> // Para a função memcpy
#include <math.h>

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

const unsigned int SAMPLE_RATE = 44100;
const unsigned int FRAMES_PER_BUFFER = 256;

sf::SoundBuffer buffer;
sf::Sound sound;

int play_song(std::string path = getTemporaryFileName())
{

    if (!buffer.loadFromFile(path.c_str()))
    {
        return 1;
    }

    sound.setBuffer(buffer);
    sound.play();

    return 0;
}

lua_Number get_luanumber(lua_State *L, std::string var_name)
{
    lua_getglobal(L, var_name.c_str());

    if (!lua_isnumber(L, -1))
    {
        std::cerr << "variable: " << var_name << " not found" << std::endl;
        lua_pop(L, 1); // Remova o valor inválido da pilha
        lua_close(L);
        return 0;
    }
    return lua_tonumber(L, -1);
}

void create_music(std::string song_script_path)
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    //test_create_song_square(getTemporaryFileName());

    if (luaL_loadfile(L, song_script_path.c_str()) || lua_pcall(L, 0, 0, 0))
    {
        std::cerr << "Error in the script: " << lua_tostring(L, -1) << std::endl;
    }

    unsigned int sample_rate = get_luanumber(L, "SAMPLE_RATE");
    unsigned int frames_per_buffer = get_luanumber(L, "FRAMES_PER_BUFFER");
    lua_Number duration_in_seconds = get_luanumber(L, "DURATION_IN_SECONDS");

    
    {
        // Definir parâmetros do arquivo de áudio
        SF_INFO fileInfo;
        fileInfo.samplerate = sample_rate;                  // Taxa de amostragem
        fileInfo.channels = 1;                              // Número de canais (monofônico)
        fileInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; // Formato de áudio (WAV, 16 bits PCM)

        SNDFILE *sndFile = sf_open(getTemporaryFileName().c_str(), SFM_WRITE, &fileInfo);
        if (!sndFile)
        {
            std::cerr << "Error creating audio file" << std::endl;
        }

        int numSamples = sample_rate * duration_in_seconds;

        std::vector<short> audioData(numSamples);

        for (int i = 0; i < numSamples; ++i)
        {
            lua_getglobal(L, "PLAY");
            lua_pushnumber(L, i);
            lua_pushnumber(L, i / sample_rate);

            if (lua_pcall(L, 2, 1, 0) != LUA_OK)
            {
                // Se houver um erro ao chamar a função Lua, imprima o erro e saia
                const char *errorMessage = lua_tostring(L, -1);
                std::cerr << "Error PLAY function not found" << std::endl;
                lua_close(L);
            }
            lua_Number ret = std::max(std::min(lua_tonumber(L, -1),1.0),0.0);
            audioData[i] = ret * SHRT_MAX;
        }

        sf_count_t writeCount = sf_writef_short(sndFile, audioData.data(), numSamples);
        if (writeCount != numSamples)
        {
            std::cerr << "Error writing audio data to file" << std::endl;
            sf_close(sndFile);
        }

        sf_close(sndFile);
    }

    std::cout << "Audio sample (square wave) created successfully" << std::endl;

    lua_close(L);

    play_song(getTemporaryFileName());
}

void save_music(const std::string &outputFilename)
{
    // Abrir o arquivo de áudio de entrada para leitura
    SF_INFO info;
    SNDFILE *file = sf_open(getTemporaryFileName().c_str(), SFM_READ, &info);
    if (!file)
    {
        std::cerr << "Error opening audio file for reading: " << sf_strerror(nullptr) << std::endl;
        return;
    }

    // Criar a estrutura SF_INFO para o arquivo de saída
    SF_INFO info_out;
    std::memcpy(&info_out, &info, sizeof(SF_INFO)); // Copiar a estrutura original

    // Abrir o arquivo de áudio de saída para escrita
    SNDFILE *file_out = sf_open(outputFilename.c_str(), SFM_WRITE, &info_out);
    if (!file_out)
    {
        std::cerr << "Error opening audio file for writing: " << sf_strerror(nullptr) << std::endl;
        sf_close(file); // Fechar o arquivo de entrada
        return;
    }

    // Definir o tamanho do buffer de leitura/gravação
    const int BUFFER_SIZE = 1024;
    float buffer[BUFFER_SIZE];

    // Ler dados do arquivo de entrada e gravá-los no arquivo de saída
    sf_count_t read_count;
    while ((read_count = sf_read_float(file, buffer, BUFFER_SIZE)) > 0)
    {
        // Escrever os dados lidos no arquivo de saída
        sf_count_t write_count = sf_write_float(file_out, buffer, read_count);
        if (write_count != read_count)
        {
            std::cerr << "Error writing audio data to output file." << std::endl;
            sf_close(file); // Fechar ambos os arquivos
            sf_close(file_out);
            return;
        }
    }

    // Fechar os arquivos de áudio
    sf_close(file);
    sf_close(file_out);

    std::cout << "Audio file saved successfully: " << outputFilename << std::endl;
}
