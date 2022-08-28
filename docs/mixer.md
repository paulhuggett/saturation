---
title: A Simple Mixer
graphHeight : 150
smallGraphHeight : 90
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

The first two charts below show two input sinusoids. You can experiment
with changing the frequency, amplitude, and phase of each. The next three
charts show the result of mixing the two input signals.

- The [first (titled “True Output”)](#graphSum) shows the mixer’s mathematically
  correct output. Where there is constructive interference between the two input
  waveforms, values may lie outside the [-1.0,1.0] range — these are show in red
  — and a real-world implementation will distort this part of the output
  waveform.
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

<div class="run">
  <div>
{% include mixer_controls.liquid input=1 %}
    <div id="graph1">
{% include mixer_skeleton.liquid maskid=1 height=page.smallGraphHeight %}
    </div>
  </div>

  <div>
{% include mixer_controls.liquid input=2 %}
    <div id="graph2">
{% include mixer_skeleton.liquid maskid=2 height=page.smallGraphHeight %}
    </div>
  </div>
</div>

<figure>
   <figcaption>True Output</figcaption>
  <div id="graphSum">
{% include mixer_skeleton.liquid maskid=3 height=page.graphHeight %}
  </div>
</figure>

<div class="run">
  <figure>
    <figcaption>Saturating Addition</figcaption>
    <div id="graphSatSum">
{% include mixer_skeleton.liquid maskid=4 height=page.graphHeight %}
    </div>
  </figure>
  <figure>
    <figcaption>Modulo Addition</figcaption>
    <div id="graphModSum">
{% include mixer_skeleton.liquid maskid=5 height=page.graphHeight %}
    </div>
  </figure>
</div>

Compare the results when using [saturating addition](#graphSatSum) or
[modulo addition](#graphModSum) to the
[desired (true) output](#graphSum). Saturating addition has produced
output which is much closer to the ideal output.

<script type="module">
  import { mixerPage } from './mixer.js'
  document.addEventListener('DOMContentLoaded', mixerPage)
</script>
