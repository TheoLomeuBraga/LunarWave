SAMPLE_RATE = 44100
FRAMES_PER_BUFFER = 256
DURATION_IN_SECONDS = 4

local FREQUENCY_HZ = 440
function square_wave(no_sample, time)
    local cycles = time * FREQUENCY_HZ
    
    local position = no_sample / SAMPLE_RATE * FREQUENCY_HZ
    
    local is_positive = (position % 1) < 0.5
    
    if is_positive then
        return 1
    else
        return -1
    end
end

function PLAY(no_sample, time)
    if time < 1 then
        return square_wave(no_sample, time)
    end
    return 0
end