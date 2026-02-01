# Synthesizer Demo Suite – Dynamics and Effects

The **Dynamics and Effects** section of the Tonic-based Synthesizer Demo Suite explores real-time dynamic range control using compressor generators. These demos illustrate how to shape the amplitude of tonal and noise signals with attack, release, ratio, threshold, gain, and bypass controls. Users can tweak parameters programmatically or interactively via the Vorogui Voronoi interface.

## CompressorTestSynth

### Overview

`CompressorTestSynth` emulates a simple percussive signal (tones + noise) processed through a compressor. It exposes key compressor parameters as `ControlParameter`s, builds ADSR-shaped generators, and routes the mix through a `Compressor` before sending it to the audio output .

### Control Parameters

| 🎛️ Parameter | Default | Range | Description |
| --- | --- | --- | --- |
| **threshold** | `-12.f` | –60 … 0 dBFS | Compressor threshold (dBFS) |
| **ratio** | `1.0` | 1 … 64 (log scale) | Compression ratio |
| **attackTime** | `0.001` | 0.001 … 0.1 s (log) | Attack time in seconds |
| **releaseTime** | `0.05` | 0.01 … 0.08 s (log) | Release time in seconds |
| **gain** | `0.f` | 0 … 36 dBFS | Makeup gain after compression |
| **bypass** | `false` | toggle | Enable/disable compression |


### Signal Flow

1. **Noise Source**: `Noise()` → HPF → LPF → scaled
2. **Tones**: Two sine waves at 180 Hz and 332 Hz, mixed and scaled
3. **ADSR Envelopes**: Triggered by a `ControlMetro`/`ControlPulse` gate
4. **Compression**: Combined generators pass through a `Compressor`
5. **Output**: Final mix halves amplitude and feeds `setOutputGen`

```cpp
Compressor compressor = Compressor()
  .attack(attack)
  .release(0.06)
  .threshold(ControlDbToLinear().input(threshold))
  .ratio(ratio)
  .lookahead(0.001)
  .makeupGain(ControlDbToLinear().input(gain))
  .bypass(bypass);

Generator outputGen =
  (((tones * toneADSR) + (hpNoise * noiseEnv))
   >> compressor) * 0.5;

setOutputGen(outputGen);
```

### Vorogui Integration

Vorogui reads all `ControlParameter`s via `global_pSynth->getParameters()` and assigns each to a point in the Voronoi diagram. It normalizes parameter values to [0,1] and visualizes them as cell heights.

- On **left-click drag**, users adjust a parameter by moving its cell’s Y-ratio .
- Internally, Vorogui updates the synth via `setParameter(name, value)` .

---

## Related Synths

### CompressorDuckingTestSynth

**Ducking demo**: This synth side-chains a bass signal under a snare trigger. It uses:

- A fixed snare noise + tone generator
- A high-ratio `Compressor` with `audioInput(randomBass >> delay).sidechainInput(snare)`
- Adjustable release time via `compRelease` parameter .

### CompressorExpSynth

**Exponential compressor**: An alternate approach that applies compression per voice in a multi-voice synth. It demonstrates dynamic control over multiple `Compressor` instances chained into a voice summation .

---

**Control Tools & Dependencies**

- Tonic’s `Compressor`, `ControlDbToLinear`, `ADSR`, `Noise`, `SineWave`, `LPF12`, `HPF24`
- PortAudio/ASIO for low-latency audio I/O
- FreeImage and libsndfile for any media I/O (not shown here)
- Vorogui (`c_pointset.cpp` / `c_vorogui.cpp`) for interactive parameter mapping

These demos showcase fundamental dynamic-range techniques, suitable both for audio experimentation and as templates for more complex DSP applications.