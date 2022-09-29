---
title: EG Curve
height: 300
width: 450
min: 0.8000
max: 0.9999
step: 0.0001
---

# {{ page.title }}

<link href="envelope.css" rel="stylesheet">
<style>
.row {
  display: grid;
  grid-auto-flow: column;
  gap: 5%;
}
.col {
}
</style>

<svg viewBox="0 0 418 259" width="418" height="259">

  <defs>
    <marker id="arrow" markerHeight="6" markerUnits="strokeWidth" markerWidth="7" orient="auto" overflow="visible" viewBox="-1 -3 7 6">
      <path d="M 4.8 0 L 0 -1.8 L 0 1.8 Z" fill="currentColor"
            stroke-linejoin="miter" stroke-miterlimit="10" stroke-width="1"  stroke="currentColor"></path>
    </marker>
  </defs>

  <g fill="none" font-family="sans-serif" font-size="18">
    <g>
      <!-- z^-1 box -->
      <rect class="diagram-shape" x="321" y="126" width="50" height="50"></rect>
      <text class="diagram-text" transform="translate(346 156)">
        <tspan>z</tspan>
        <tspan font-size="11" x="8" y="-6">-1</tspan>
      </text>
    </g>
    <g>
      <!-- triangle b -->
      <path class="diagram-shape" d="M 255 246 L 181 216 L 255 186 Z"></path>
      <text class="diagram-text" x="230" y="216">b</text>
    </g>
    <g>
      <!-- Circle Sigma -->
      <circle class="diagram-shape" cx="91" cy="82" r="25"></circle>
      <text   class="diagram-text"   x="91"  y="82" font-size="29">&#x01A9;</text>
    </g>
    <g>
      <!-- -x_0 -->
      <text class="diagram-text" transform="translate(91 11)">
        <tspan>-x</tspan>
        <tspan font-size="11" x="9" y="4">0</tspan>
      </text>
    </g>
    <g class="diagram-edge" marker-end="url(#arrow)">
      <line x1="91"  y1="13" x2="91"  y2="44"></line>  <!-- -x0 -> Σ -->
      <line x1="4"   y1="82" x2="52"  y2="82"></line>  <!-- left -> Σ -->
      <line x1="116" y1="82" x2="400" y2="82"></line>  <!-- Σ -> right -->
      <path d="M 181 216 L 91 216 L 91 120"></path>    <!-- b -> Σ -->
      <line x1="346" y1="82" x2="346" y2="113"></line> <!-- top line -> z^-1 -->
      <path d="M 346 176 L 346 216 L 268 216"></path>  <!-- z^-1 -> b -->
    </g>
    <circle cx="346" cy="82" r="5" fill="black"></circle>
  </g>
</svg>

<div></div>


<div class="keep-together">
  <label for="ts-range" class="vertical-center">t<sub>s</sub>:</label>
  <input id="ts-range" type="range"  min="0.001" max="2" step="0.001" value="0.95" required class="vertical-center">
  <input id="ts-value" type="number" min="0.001" max="2" step="0.001" class="vertical-center">
  <span class="validity"></span>
</div>

<div></div>

<div class="keep-together">
  <label for="b-range" class="vertical-center">b (&lt;1.0):</label>
  <input id="b-range" type="range"  min="{{ page.min }}" max="{{ page.max }}" step="{{ page.step }}" value="0.95" required class="vertical-center">
  <input id="b-value" type="number" min="{{ page.min }}" max="{{ page.max }}" step="{{ page.step }}" class="vertical-center">
  <span class="validity"></span>
</div>

<div class="keep-together">
  <label for="x0-range" class="vertical-center">x<sub>0</sub>(&lt;1.0):</label>
  <input id="x0-range" type="range" min="0" max="0.1" step="0.0001" value="0.0" required class="vertical-center">
  <input id="x0-value" type="number" min="0" max="0.1" step="0.0001" class="vertical-center">
  <span class="validity"></span>
</div>

<div>
  <input type="radio" id="radio-charge" name="charge-discharge" value="1">
  <label for="radio-charge">Charge</label>

  <input type="radio" id="radio-discharge" name="charge-discharge" value="0" checked>
  <label for="radio-discharge">Discharge</label>
</div>

<div></div>

<svg id="envelope" width="{{ page.width }}" height="{{ page.height }}" viewBox="0 0 {{ page.width }} {{ page.height }}">

  <defs>
    <mask id="sparkle-mask">
      <rect fill="#777" width="100%" height="100%"></rect>
      <rect class="loading maskr2" fill="hsla(200,0%,10%,0.6)" width="100%" height="100%"></rect>
    </mask>
  </defs>

  <rect class="loading" mask="url(#sparkle-mask)" fill="#dadada" width="100%" height="100%"></rect>
</svg>

<script>
MathJax = {
  tex: {
    tags: 'ams', // should be 'ams', 'none', or 'all'
    inlineMath: [['$', '$'], ['\\(', '\\)']]
  }
//  ,
//  svg: {
//    fontCache: 'global'
//  }
}
</script>
<script id="MathJax-script" async src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-svg.js"></script>
<script src="https://polyfill.io/v3/polyfill.min.js?features=es6"></script>

<!-- div class="row" -->
<!-- div class="col" -->
### Charge:

$$
V_c = V_0(1 - e^{-t/RC})
$$

Term | Description
---- | -----------
*V<sub>c</sub>* | The voltage across the capacitor after time *t* has elapsed.
*V<sub>0</sub>* | The initial voltage across the capacitor before charging/discharging begins.
*t*  | The time taken for the charge/discharge: determines how much the capacitor charges/discharges.
*R*  | The resistance to which the capacitor is connected. The greater the resistance value, the slower the charge/discharge.
*C*  | The capacitance: the greater the capacitance, the more charge can be held and the longer it takes to charge/discharge.

A CEM3310 charges the capacitor to 77% therefore, if we normalize RC to 1.0:

<div>
\begin{align}
1 - e^{t} &= 0.77       \\
    e^{t} &= 0.23       \\
       -t &= log_e 0.23 \\
        t &\approx 1.4969
\end{align}
</div>

<!-- /div --> <!-- col -->
<!-- div class="col"-->
### Discharge

$$
V_c = V_0 e^{-t/RC}
$$

We discharge the capacitor from 77%. We assume that both *V<sub>0</sub>* and *RC* are 1. First we need to know that initial offset for x:

<div>
\begin{align*}
       e^{-d} &= 0.77         \\
\frac{1}{e^d} &= 0.77         \\
           -d &= log_e 0.77   \\
            d &\approx 0.2614 \\
\end{align*}
</div>

If we we assume that our capacitor start at 77% of its full charge,  and is "fully" discharged with a residual charge of 0.7% (i.e. 99.3% discharged):

<div>
\begin{align*}
e^{-x + log_e 0.77} &= \frac{0.7}{100}          \\
    -x + log_e 0.77 &= log_e 0.007              \\
                  x &= log_e 0.77 - log_e 0.007 \\
                    &\approx 4.7004
\end{align*}
</div>
<!-- /div --> <!-- col -->
<!-- /div --> <!-- row -->

#### Computing the Time Constant Overshoot.

<div>
\begin{align*}
y(x) &= e^{-4.7004x} \\
y(1) &= e^{-4.7004}  \\
     &\approx 9.09 \times 10^{-3}
\end{align*}
</div>

## Stuff About Signal-to-Noise Ratio

The signal to quantization noise ratio (SQNR) can be [calculated from](https://en.wikipedia.org/wiki/Signal-to-noise_ratio#Fixed_point):

$$
SQNR=20 log_{10}(2^{Q})\approx 6.02\cdot Q\ dB
$$

where Q is the number of quantization bits and the result is measured in decibels (dB)

<label for="bit-depth-select">Bit Depth:</label>
<select name="bit depth" id="bit-depth">
  <option value="8">8</option>
  <option value="16">16</option>
  <option value="24">24</option>
  <option value="32">32</option>
</select>
SQNR = <span id="sqnr">0</span> dB


##  Fixed Point Arithmetic Rules

### Addition

<div>
\begin{align*}
U(a, b) + U(a, b) &= U(a + 1, b) \\
S(a, b) + S(a, b) &= S(a + 1, b)
\end{align*}
</div>

The sum of two M-bit numbers requires M+1 bits.

### Multiplication

<div>
\begin{align*}
U(a_1, b_1) \times U(a_2, b_2) &= U(a_1 + a_2, b_1 + b_2) \\
S(a_1, b_1) \times S(a_2, b_2) &= S(a_1 + a_2 + 1, b_1 + b_2)
\end{align*}
</div>


<script type="module">
import { controls } from './envelope.js'
document.addEventListener('DOMContentLoaded', () => {
  controls()

  const bitDepth = document.querySelector('#bit-depth')
  const setSQNR = () => {
    document.querySelector('#sqnr').innerHTML = (20.0 * Math.log10(Math.pow(2, +bitDepth.value))).toPrecision(4)
  }
  bitDepth.addEventListener('change', setSQNR)
  setSQNR()
})
</script>
