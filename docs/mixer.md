---
title: A Simple Mixer
---

{% assign mask_counter=0 %}

# {{ page.title }}

<link href="mixer.css" rel="stylesheet">

To show how useful a saturating addition function can be, we can look
at a simple DSP signal mixer. This takes two input signals and combines 
them to produce a single output. It’s common for DSP systems to use
[fixed-point integers](https://en.wikipedia.org/wiki/Fixed-point_arithmetic)
to represent amplitudes. The choices made in the formatting of these values
imposes a strict limit on the range of number that can be represented. The
hypothetical system described here can reproduce values in the range
[-1.0, 1.0].

The “mixer inputs” section below shows two input sinusoids. You can experiment
with changing the frequency and amplitude of each.

The “outputs” section contains three charts which show the result of mixing the
two input signals.

- The [first (titled “True Output”)](#graphSum) shows the mixer’s mathematically
  correct output. Where there is constructive interference between the two input
  waveforms, values may lie outside the [-1.0,1.0] range and a real-world
  implementation will distort this part of the output waveform.
- The [second chart (“Saturating Addition”)](#graphSatSum) shows the result of
  using saturating addition. Where the output would exceed the maximum or
  minimum amplitude values, the result is clamped to that range.
- The [third chart (“Modulo Addition”)](#graphModSum) shows the use of
  conventional modulo arithmetic. Here calculations that overflow result in
  dramatic discontinuities in the output waveform as the values “wrap around”
  when they overflow.

It should be clear that neither saturating nor modulo addition will be able to
produce the desired output — there will be distortion of some kind regardless
of our choice — but that using saturating arithmetic yields a waveform that is
much closer to the ideal.

## Example

### Mixer Inputs

Changing the frequency and amplitude sliders for either of the two input
waveforms will cause the three [output](#mixer-output) charts below to be updated.

<div class="run">
  <div>
{% assign input=1 %}
{% include mixer_controls.liquid %}
    <div id="graph1">
{% assign height=90 %}
{% include mixer_skeleton.liquid %}
    </div>
  </div>

  <div>
{% assign input=2 %}
{% include mixer_controls.liquid %}
    <div id="graph2">
{% assign height=90 %}
{% include mixer_skeleton.liquid %}
    </div>
  </div>
</div>

### Mixer Output

<div id="graphSum">
{% include mixer_skeleton.liquid %}
</div>

Areas shown in red lies outside the range [-1.0,1.0] so cannot be represented
and will be distorted in the output.

<div class="run">
  <div>
    <div>Saturating Addition</div>
    <div id="graphSatSum">
{% assign height=150 %}
{% include mixer_skeleton.liquid %}
    </div>
  </div>
  <div>
    <div>Modulo Addition</div>
    <div id="graphModSum">
{% assign height=150 %}
{% include mixer_skeleton.liquid %}
    </div>
  </div>
</div>

Compare the results when using [saturating addition](#graphSatSum) or
[modulo addition](#graphModSum) to the
[desired (true) output above](#graphSum). Saturating addition has produced
output which is much closer to the ideal output.

<script type="module">
  import { mixerPage } from './mixer.js'
  document.addEventListener('DOMContentLoaded', mixerPage)
</script>
