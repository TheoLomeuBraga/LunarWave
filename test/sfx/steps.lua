require("math")

SAMPLE_RATE = 44100
FRAMES_PER_BUFFER = 256
DURATION_IN_SECONDS = 2



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

function custom_wave(t, wave)

    local time = t % 1

    if type(wave) ~= "table" then
        error("A forma de onda deve ser uma tabela de valores", 2)
    end

    if #wave == 0 then
        error("A forma de onda deve conter pelo menos um valor", 2)
    end

    local index = time * (#wave - 1) + 1
    local lowerIndex = math.floor(index)
    local upperIndex = math.ceil(index)

    if lowerIndex < 1 or upperIndex > #wave then
        error("O tempo está fora do alcance da forma de onda", 2)
    end

    local lowerValue = wave[lowerIndex]
    local upperValue = wave[upperIndex]
    local fraction = index - lowerIndex
    local interpolatedValue = lowerValue + fraction * (upperValue - lowerValue)

    return interpolatedValue
end

function random_noise()
    return math.random() * 2 - 1
end

function noise(t, cutoff_frequency)
    -- Gere um valor aleatório entre -1 e 1
    local noise = math.random() * 2 - 1
    
    -- Ajuste a frequência de corte para controlar a tonalidade do ruído
    local filter_factor = math.exp(-cutoff_frequency * t)
    noise = noise * filter_factor
    
    return noise
end

function stuffy_noise(stuffy_power)
    return (math.min(1,math.random() * stuffy_power) * 2) - 1
end

function mix_waveforms(no_sample, time_a,frequency_a, time_b,frequency_b)

    local position_tri = no_sample / SAMPLE_RATE * frequency_a
    local position_saw = no_sample / SAMPLE_RATE * frequency_b

    local mixed_value = time_a * (2 / math.pi) * math.asin(math.sin(2 * math.pi * position_tri))
                       + time_b * (position_saw - math.floor(position_saw + 0.5))

    return mixed_value
end

function change_precision(v,precision)
    return math.floor(v * precision) / precision
end



function PLAY(no_sample, time)
    time = time % 1
    local volume = sine_wave(time)
    if volume >= 0.8 then
        volume = 1
    else
        volume = 0
    end
    local step_sound =  noise(time, 7) * 2
    return step_sound * volume
end
