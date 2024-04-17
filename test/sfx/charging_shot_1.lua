require("math")

SAMPLE_RATE = 44100
FRAMES_PER_BUFFER = 256
DURATION_IN_SECONDS = 3



function square_wave(t)
    local frequency = 0.5
    local period = 1 / frequency
    local phase = 2 * math.pi * (t % period) / period

    if phase < math.pi then
        return 1
    else
        return -1
    end
end

function sine_wave(t)
    local frequency = 1
    local period = 1 / frequency
    local value = math.sin(2 * math.pi * (t % period) / period)

    return value
end

function triangular_wave(t)
    -- Defina a frequência da onda triangular (em Hz)
    local frequency = 1

    -- Calcule o período da onda triangular (em segundos)
    local period = 1 / frequency

    -- Calcule a fase da onda triangular (em radianos)
    local phase = 2 * math.pi * (t % period) / period

    -- Determine o valor da onda triangular (-1 a 1)
    return 2 * math.abs(2 * (phase / math.pi - math.floor(phase / math.pi + 0.5))) - 1
end

function sawtooth_wave(t)
    local frequency = 1
    local period = 1 / frequency
    local phase = (t % period) / period
    return 2 * phase - 1
end

function random_noise()
    return math.random() * 2 - 1
end

function mix_waveforms(no_sample, time_a, frequency_a, time_b, frequency_b)
    local position_tri = no_sample / SAMPLE_RATE * frequency_a
    local position_saw = no_sample / SAMPLE_RATE * frequency_b

    local mixed_value = time_a * (2 / math.pi) * math.asin(math.sin(2 * math.pi * position_tri))
        + time_b * (position_saw - math.floor(position_saw + 0.5))

    return mixed_value
end

charging = false

function PLAY(no_sample, time)
    local volume = 0.5 + (time / 2)
    local frequency_hz = 440 + (440 * time)
    local tri_wave = mix_waveforms(no_sample, triangular_wave(time * frequency_hz), time * frequency_hz,
        triangular_wave((time * frequency_hz) - (frequency_hz / 2)), (time * frequency_hz) - (frequency_hz / 2))
    local noise = random_noise() * (0.5 * time)
    local tri_noise = mix_waveforms(no_sample, tri_wave, frequency_hz, noise, frequency_hz)
    return volume * tri_noise
end
