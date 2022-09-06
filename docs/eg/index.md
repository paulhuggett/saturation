---
title: EG Curve
height: 400
width: 650
min: 0.8000
max: 0.9999
step: 0.0001
---

# {{ page.title }}

<link href="envelope.css" rel="stylesheet">


<svg viewBox="0 0 418 259" width="418" height="259">

  <defs>
    <marker id="arrow" markerHeight="6" markerUnits="strokeWidth" markerWidth="7" orient="auto" overflow="visible" viewBox="-1 -3 7 6">
      <path d="M 4.8 0 L 0 -1.8 L 0 1.8 Z" fill="currentColor" stroke-linejoin="miter" stroke-miterlimit="10" stroke-width="1"  stroke="currentColor"></path>
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
    <g class="diagram-edge" marker-end="url(#arrow)">
      <path d="M 181 216 L 91 216 L 91 132"></path>    <!-- b -> Σ -->
      <line x1="127" y1="82" x2="400" y2="82"></line>  <!-- Σ -> end -->
      <line x1="346" y1="82" x2="346" y2="113"></line> <!-- top lie -> z^-1 -->
      <path d="M 346 176 L 346 216 L 268 216"></path>  <!-- z^-1 -> b -->
      <line x1="91" y1="13" x2="91" y2="33"></line>    <!-- -x0 -> Σ -->
      <line x1="4" y1="82" x2="42" y2="82"></line>     <!-- start -> Σ -->
    </g>
    <circle cx="346" cy="82" r="5" fill="black"></circle>
    <g>
      <!-- triangle b -->
      <path class="diagram-shape" d="M 255 246 L 181 216 L 255 186 Z"></path>
      <text class="diagram-text" x="230" y="216">b</text>
    </g>
    <g>
      <!-- Circle Sigma -->
      <circle class="diagram-shape" cx="91" cy="82" r="37"></circle>
      <text class="diagram-text" x="91" y="82" font-size="29">&#x01A9;</text>
    </g>
    <g>
      <!-- -x_0 -->
      <text class="diagram-text" transform="translate(91 11)">
        <tspan>-x</tspan>
        <tspan font-size="11" x="9" y="4">0</tspan>
      </text>
    </g>
  </g>
</svg>



<div class="keep-together">
  <label for="ts-range" class="vertical-center">t<sub>s</sub>:</label>
  <input id="ts-range" type="range"  min="0.001" max="2" step="0.001" value="0.95" required class="vertical-center">
  <input id="ts-value" type="number" min="0.001" max="2" step="0.001" class="vertical-center">
  <span class="validity"></span>
</div>


<div class="keep-together">
  <label for="b-range" class="vertical-center">Decay (&lt;1.0):</label>
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

<svg id="envelope" width="{{ page.width }}" height="{{ page.height }}" viewBox="0 0 {{ page.width }} {{ page.height }}">
  <defs>
    <mask id="sparkle-mask">
      <rect fill="#777" width="100%" height="100%"></rect>
      <rect class="loading maskr2" fill="hsla(200,0%,10%,0.6)" width="100%" height="100%"></rect>
    </mask>
  </defs>
  <rect class="loading" mask="url(#sparkle-mask)" fill="#dadada" width="100%" height="100%"></rect>
</svg>

<script type="module">
import { controls } from './envelope.js'
document.addEventListener('DOMContentLoaded', () => {
  controls()
})
</script>
