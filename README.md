# Welcome to LunarWave!

LunarWave is a powerful tool for creating audio samples using Lua. With LunarWave, you can easily create a variety of sound waveforms and explore the magic of music in a whole new way!

## About LunarWave

LunarWave is an open-source tool designed to provide a simple and accessible experience in audio sample creation. Developed with Lua, a powerful and easy-to-learn programming language, LunarWave empowers users to express their musical creativity in a unique and intuitive way.

### Key features:

- **Easily Create**: Use Lua to generate a variety of sound waveforms, including sine, square, triangular, sawtooth, and more.
- **Total Flexibility**: Adjust parameters such as frequency, amplitude, and duration to create the perfect sound.
- **Wide Compatibility**: Export your audio samples to a variety of popular formats, such as WAV, MP3, and OGG.
- **Fully Customizable**: Adapt the LunarWave source code to your specific needs and add new features with ease.

## Usage Example

See how easy it is to create a simple sine wave in Lua using LunarWave:

```lua
function PLAY(no_sample, time, frequency_hz)
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

Here's how you can run this script.

```shell
    ./build/lunarwave ./test/basics.lua -o basics.wav
```
