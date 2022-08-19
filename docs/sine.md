---
title: A Simple Mixer
---

# {{ page.title }}

<link href="sine.css" rel="stylesheet">

<script type="module">
  import { mixerPage } from './sine.js'
  document.addEventListener('DOMContentLoaded', mixerPage)
</script>

To show how useful a saturating addition function can be, we can look
at a simple DSP signal mixer. This takes two input signals and combines 
them to produce a single output. The range of the range of amplitudes
our system can reproduce is [-1.0, 1.0].

The “inputs” section below shows two input sinusoids. You can experiment with
changing the frequency and amplitude of each.

The “outputs” section contains three charts which show the result of mixing the
two input signals.

- The [first (titled “True Output”)](#graphSum) shows the mixer’s mathematically
  correct output. Where there is constructive interference between the two input
  waveforms, values may lie outside the [-1.0,1.0] range and a real-world
  implementation will distort this part of the output waveform.
- The [second chart (“Saturating Addition”)](#graphSatSum) shows the result of
  using saturating addition. Where the output would exceed the maximum or
  minimum amplitude values, the result is clamped to that range.
- The [third chart (”Modulo Addition”)](#graphModSum) shows the use of
  conventional modulo arithmetic. Here calculations that overflow result in
  dramatic discontinuities in the output waveform as the values ”wrap around”
  when they overflow.

It should be clear that neither saturating nor modulo addition will be able to
produce the desired output but that using saturating arithmetic yields a
waveform that is much closer to the ideal.

## Example

### Inputs

Changing the frequency and amplitude sliders for either of the two input
frequencies, will cause the output charts below to be updated.

<fieldset>
  <legend>Input 1</legend>
    <div class="run">
      <label for="frequency1">Frequency:</label>
      <input type="range" list="ticksfrequency1" min="0.1" max="4.0" value="2.0" step="0.05" id="frequency1">
      <datalist id="ticksfrequency1">
        <option value="0.1"></option>
        <option value="1"></option>
        <option value="2"></option>
        <option value="3"></option>
        <option value="4"></option>
      </datalist>
      <div id="freqvalue1" class="num"></div>
    </div>
    <div class="run">
      <label for="amplitude1">Amplitude:</label>
      <input type="range" list="ticksamplitude1" min="0.0" max="1.0" value="0.9" step="0.01" id="amplitude1">
        <datalist id="ticksamplitude1">
        <option value="0"></option>
        <option value="0.2"></option>
        <option value="0.4"></option>
        <option value="0.6"></option>
        <option value="0.8"></option>
        <option value="1.0"></option>
      </datalist>
      <div id="ampvalue1" class="num"></div>
    </div>
    <div id="graph1"></div>
</fieldset>

<fieldset>
  <legend>Input 2</legend>
  <div class="run">
    <label for="frequency2">Frequency:</label>
    <input type="range" list="ticksfrequency2" min="0.1" max="4.0" value="3.0" step="0.05" id="frequency2">
    <datalist id="ticksfrequency2">
      <option value="0.1"></option>
      <option value="1.0"></option>
      <option value="2.0"></option>
      <option value="3.0"></option>
      <option value="4.0"></option>
    </datalist>
    <div id="freqvalue2" class="num"></div>
  </div>
  <div class="run">
    <label for="amplitude2">Amplitude:</label>
    <input type="range" list="ticksamplitude2" min="0.0" max="1.0" value="0.9" step="0.01" id="amplitude2">
    <datalist id="ticksamplitude2">
      <option value="0"></option>
      <option value="0.2"></option>
      <option value="0.4"></option>
      <option value="0.6"></option>
      <option value="0.8"></option>
      <option value="1.0"></option>
    </datalist>
    <div id="ampvalue2" class="num"></div>
  </div>
  <div id="graph2"></div>
</fieldset>

### Output

<div id="graphSum"></div>

Areas shown in red cannot be represented and will be distorted in the output.

<div class="run">
  <div id="graphSatSum"></div>
  <div id="graphModSum"></div>
</div>
