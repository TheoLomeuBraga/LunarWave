require("math")

SAMPLE_RATE = 44100
FRAMES_PER_BUFFER = 256
DURATION_IN_SECONDS = 8



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

function mix_waveforms(no_sample, time_a,frequency_a, time_b,frequency_b)

    local position_tri = no_sample / SAMPLE_RATE * frequency_a
    local position_saw = no_sample / SAMPLE_RATE * frequency_b

    local mixed_value = time_a * (2 / math.pi) * math.asin(math.sin(2 * math.pi * position_tri))
                       + time_b * (position_saw - math.floor(position_saw + 0.5))

    return mixed_value
end


local custom_wave_patern = {1,0,0}
function PLAY(no_sample, time)
    
    
    if time < 1 then
        return square_wave(time * 220)
    elseif time < 2 then
        return sine_wave(time * 330)
    elseif time < 3 then
        return triangular_wave(time * 440)
    elseif time < 4 then
        return sawtooth_wave(time * 550)
    elseif time < 5 then
        local sqwave = square_wave(time * 220)
        local sqwavef = 220
        local swave = sine_wave(time * 220)
        local swavef = 220
        return mix_waveforms(no_sample,sqwave,sqwavef,swave,swavef)
    elseif time < 6 then
        local twave = square_wave(time * 440)
        local twavef = 440
        local stwave = sine_wave(time * 440)
        local stwavef = 440
        return mix_waveforms(no_sample,twave,twavef,stwave,stwavef)
    elseif time < 7 then
        return custom_wave(time * 660,custom_wave_patern)
    elseif time < 8 then
        return random_noise()
    end
    return 0
end
