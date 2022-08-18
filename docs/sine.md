---
title: A Simple Mixer
---

= Sine Curve

<script type="module">
  import * as d3 from 'https://cdn.skypack.dev/d3@7'
  import { sine, internalWidth } from './sine.js'

  document.addEventListener('DOMContentLoaded', event => {
    const frequency1 = document.querySelector('#frequency1')
    const freqvalue1 = document.querySelector('#freqvalue1')

    const amplitude1 = document.querySelector('#amplitude1')
    const ampvalue1 = document.querySelector('#ampvalue1')

    const frequency2 = document.querySelector('#frequency2')
    const freqvalue2 = document.querySelector('#freqvalue2')

    const amplitude2 = document.querySelector('#amplitude2')
    const ampvalue2 = document.querySelector('#ampvalue2')

    const height = 150
    const points = internalWidth / 2
    const makeXArray = () => d3.range(0, points).map(k => k / (points - 1) * Math.PI * 2)
    const makePoints = f => (x => [x, f(x)])

    const update = () => {
      const c1 = x => Math.sin(x * frequency1.value) * amplitude1.value
      const c2 = x => Math.sin(x * frequency2.value) * amplitude2.value

      freqvalue1.innerText = frequency1.value
      ampvalue1.innerText = amplitude1.value 
      freqvalue2.innerText = frequency2.value
      ampvalue2.innerText = amplitude2.value 
      sine('#graph1', height, null, makeXArray().map(makePoints(c1)))
      sine('#graph2', height, null, makeXArray().map(makePoints(c2)))
      sine('#graphSum', height * 1.25, "True Mix (i.e. Input 1 + Input 2)", makeXArray().map(x => [x, c1(x) + c2(x)]))
      sine('#graphSatSum', height * 1, "Saturating Addition", makeXArray().map(x => [x, Math.max(Math.min(c1(x) + c2(x), 1), -1)]))
      sine('#graphModSum', height * 1, "Modulo Addition", makeXArray().map(x => [x, (c1(x) + c2(x)) % 1.0]))
    }

    [frequency1, amplitude1, frequency2, amplitude2].forEach(el => el.addEventListener('input', update))
    update ()
  })

</script>


To show the usefulness of a saturating addition function, we can look 
at a simple DSP signal mixer. This takes two input signals and combines 
them to produce a single output.

The graphs below show the two input waveforms followed by the ideal, 
mathematically correct, output. If we assume that the range of amplitudes
our system can reproduce is [-1.0, 1.0] we can see there are some values 
we cannot represent.
      
We then see the output of our software mixer produces using first 
conventional modulo arithmetic and then by saturating arithmetic. It 
should be clear that neither is perfect and both exhibit some distortion,
but the latter yields a waveform that is much closer to the ideal.

== Inputs

<fieldset>
  <legend>Input 1</legend>
  <div id="graph1"></div>
  <label for="frequency1">Frequency:</label>
  <input type="range" list="ticksfrequency1" min="0.5" max="4.0" value="2.0" step="0.05" id="frequency1">
    <datalist id="ticksfrequency1">
    <option value="0.5"></option>
    <option value="1"></option>
    <option value="2"></option>
    <option value="3"></option>
    <option value="4"></option>
  </datalist>
  <div id="freqvalue1"></div>
  <label for="amplitude1">Amplitude:</label>
  <input type="range" list="ticksamplitude1" min="0.0" max="1.0" value="0.8" step="0.01" id="amplitude1">
    <datalist id="ticksamplitude1">
    <option value="0"></option>
    <option value="0.2"></option>
    <option value="0.4"></option>
    <option value="0.6"></option>
    <option value="0.8"></option>
    <option value="1.0"></option>
  </datalist>
  <div id="ampvalue1"></div>
</fieldset>

<fieldset>
  <legend>Input 2</legend>
  <div id="graph2"></div>
  <label for="frequency2">Frequency:</label>
  <input type="range" list="ticksfrequency2" min="0.5" max="4.0" value="2.0" step="0.05" id="frequency2">
    <datalist id="ticksfrequency2">
      <option value="0.5"></option>
      <option value="1"></option>
      <option value="2"></option>
      <option value="3"></option>
      <option value="4"></option>
    </datalist>
    <div id="freqvalue2"></div>
    <div>
    <label for="amplitude2">Amplitude:</label>
    <input type="range" list="ticksamplitude2" min="0.0" max="1.0" value="0.8" step="0.01" id="amplitude2">
      <datalist id="ticksamplitude2">
      <option value="0"></option>
      <option value="0.2"></option>
      <option value="0.4"></option>
      <option value="0.6"></option>
      <option value="0.8"></option>
      <option value="1.0"></option>
    </datalist>
    <div id="ampvalue2"></div>
  </div>
  </fieldset>

<div id="graphSum"></div>
<div id="graphSatSum"></div>
<div id="graphModSum"></div>
