require("math")

SAMPLE_RATE = 44100
FRAMES_PER_BUFFER = 256
DURATION_IN_SECONDS = 7


function square_wave(no_sample, time,frequency_hz)
    local cycles = time * frequency_hz

    local position = no_sample / SAMPLE_RATE * frequency_hz

    local is_positive = (position % 1) < 0.5

    if is_positive then
        return 1
    else
        return -1
    end
end

function sine_wave(no_sample, time,frequency_hz)
    local amplitude = 0.5    -- Amplitude da onda senoidal

    local cycles = time * frequency_hz
    local position = no_sample / SAMPLE_RATE * frequency_hz

    -- Calcular o valor da onda senoidal usando a função seno
    local value = amplitude * math.sin(2 * math.pi * position)

    return value
end

function triangular_wave(no_sample, time,frequency_hz)
    local amplitude = 0.5    -- Amplitude da onda triangular

    local cycles = time * frequency_hz
    local position = no_sample / SAMPLE_RATE * frequency_hz

    -- Calcular o valor da onda triangular usando a função triângulo
    local value = (2 * amplitude / math.pi) * math.asin(math.sin(2 * math.pi * position))

    return value
end

function sawtooth_wave(no_sample, time,frequency_hz)
    local amplitude = 0.5     -- Amplitude da onda dente de serra

    local cycles = time * frequency_hz
    local position = no_sample / SAMPLE_RATE * frequency_hz

    -- Calcular o valor da onda dente de serra
    local value = (2 * amplitude) * (position - math.floor(position + 0.5))

    return value
end

function random_noise()
    return math.random() * 2 - 1
end

function mix_waveforms(no_sample, amplitude_a,frequency_a, amplitude_b,frequency_b)

    local position_tri = no_sample / SAMPLE_RATE * frequency_a
    local position_saw = no_sample / SAMPLE_RATE * frequency_b

    local mixed_value = amplitude_a * (2 / math.pi) * math.asin(math.sin(2 * math.pi * position_tri))
                       + amplitude_b * (position_saw - math.floor(position_saw + 0.5))

    return mixed_value
end



function PLAY(no_sample, time)
    if time < 1 then
        return square_wave(no_sample, time,220)
    elseif time < 2 then
        return sine_wave(no_sample, time,330)
    elseif time < 3 then
        return triangular_wave(no_sample, time,440)
    elseif time < 4 then
        return sawtooth_wave(no_sample, time,550)
    elseif time < 5 then
        local sqwave = square_wave(no_sample, time,220)
        local sqwavef = 220
        local swave = sine_wave(no_sample, time,220)
        local swavef = 220
        return mix_waveforms(no_sample,sqwave,sqwavef,swave,swavef)
    elseif time < 6 then
        local twave = square_wave(no_sample, time,440)
        local twavef = 440
        local stwave = sine_wave(no_sample, time,440)
        local stwavef = 440
        return mix_waveforms(no_sample,twave,twavef,stwave,stwavef)
    elseif time < 7 then
        return random_noise()
    end
    return 0
end
