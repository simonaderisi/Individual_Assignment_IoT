import serial
import numpy as np
import time
import sounddevice as sd

if __name__ == "__main__":

    # Samples per second
    sps = 44100

    # Frequency / pitch
    freq_hz = 40
    freq_hz_2 = 10

    # Duration
    duration_s = 120.0

    # Attenuation so the sound is reasonable
    atten = 1.0 # 0.3

    # calculate the waveform
    each_sample_number = np.arange(duration_s * sps)
    waveform = 2* np.sin(2 * np.pi * each_sample_number * freq_hz / sps) + 3 * np.sin(2 * np.pi * each_sample_number * freq_hz_2 / sps) 
    waveform_quiet = waveform * atten

    # Play the waveform out the speakers
    sd.play(waveform_quiet, sps)
    time.sleep(duration_s)
    sd.stop()
