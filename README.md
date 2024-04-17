# Bem-vindo ao LunarWave!

LunarWave é uma poderosa ferramenta para criação de amostras de áudio utilizando Lua. Com LunarWave, você pode criar facilmente uma variedade de formas de onda sonora e explorar a magia da música de uma maneira totalmente nova!

## Sobre o LunarWave

LunarWave é uma ferramenta de código aberto projetada para proporcionar uma experiência simples e acessível na criação de amostras de áudio. Desenvolvido com Lua, uma linguagem de programação poderosa e fácil de aprender, LunarWave capacita os usuários a expressarem sua criatividade musical de forma única e intuitiva.

### Principais recursos:

- **Crie facilmente**: Use Lua para gerar uma variedade de formas de onda sonora, incluindo seno, quadrada, triangular, dente de serra e muito mais.
- **Flexibilidade total**: Ajuste parâmetros como frequência, amplitude e duração para criar o som perfeito.
- **Ampla compatibilidade**: Exporte suas amostras de áudio para uma variedade de formatos populares, como WAV, MP3 e OGG.
- **Totalmente personalizável**: Adapte o código-fonte do LunarWave às suas necessidades específicas e adicione novos recursos com facilidade.

## Exemplo de Uso

Veja como é fácil criar uma onda senoidal simples em Lua usando o LunarWave:

```shell
    ./build/lunarwave ./test/basics.lua -o basics.wav
```

```lua
function PLAY(no_sample, time,frequency_hz)
    local cycles = time * frequency_hz

    local position = no_sample / SAMPLE_RATE * frequency_hz

    local is_positive = (position % 1) < 0.5

    if is_positive then
        return 1
    else
        return -1
    end
end
```
