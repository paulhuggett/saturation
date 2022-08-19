import * as d3 from 'https://cdn.skypack.dev/d3@7'

const width = 360
const margin = { top: 12, left: 40, bottom: 12, right: 10 }
const internalWidth = width - margin.left - margin.right

function sine (id, height, title, data) {
  const internalHeight = height - margin.top - margin.bottom

  d3.select(id).selectChildren().remove()
  const svg = d3.select(id)
    .append('svg')
    .attr('width', width)
    .attr('height', height)

  const xScale = d3.scaleLinear().range([0, internalWidth]).domain(d3.extent(data, d => d[0]))
  const yMax = Math.ceil(d3.max(data, d => Math.abs(d[1])))
  const yScale = d3.scaleLinear().range([internalHeight, 0]).domain([-yMax, yMax])
  const xAxis = d3.axisBottom().scale(xScale).tickValues([1, 2, 3, 4, 5, 6])
  const yAxis = d3.axisLeft().scale(yScale).ticks(3)

  const line = d3.line().x(d => xScale(d[0])).y(d => yScale(d[1]))

  if (title !== null) {
    svg.append('text')
      .text(title)
      .attr('x', (width / 2))
      .attr('y', 17)
      .attr('text-anchor', 'middle')
      .style('font-size', '16px')
      .style('text-decoration', 'underline')
  }
  const g = svg.append('g')
    .attr('transform', 'translate(' + margin.left + ',' + margin.top + ')')
  g.append('path')
    .datum(data)
    .attr('d', line)
    .attr('stroke', 'black')
    .attr('stroke-width', 1)
    .attr('fill', 'none')
  g.append('g')
    .classed('axis', true)
    .attr('transform', 'translate(0, ' + internalHeight / 2 + ')')
    .call(xAxis)
  g.append('g')
    .classed('axis', true)
    .call(yAxis)
}

export function mixerPage () {
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
  const makePoints = f => x => [x, f(x)]

  const update = () => {
    const c1 = x => Math.sin(x * frequency1.value) * amplitude1.value
    const c2 = x => Math.sin(x * frequency2.value) * amplitude2.value

    freqvalue1.innerText = frequency1.value
    ampvalue1.innerText = amplitude1.value
    freqvalue2.innerText = frequency2.value
    ampvalue2.innerText = amplitude2.value

    sine('#graph1', 100, null, makeXArray().map(makePoints(c1)))
    sine('#graph2', 100, null, makeXArray().map(makePoints(c2)))

    sine('#graphSum', height * 1.25, 'True Output', makeXArray().map(x => [x, c1(x) + c2(x)]))
    sine('#graphSatSum', height, 'Saturating Addition', makeXArray().map(x => [x, Math.max(Math.min(c1(x) + c2(x), 1), -1)]))
    sine('#graphModSum', height, 'Modulo Addition', makeXArray().map(x => [x, (c1(x) + c2(x)) % 1.0]))
  }

  [frequency1, amplitude1, frequency2, amplitude2].forEach(el => el.addEventListener('input', update))
  update()
}
