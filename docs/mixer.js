import * as d3 from 'https://cdn.skypack.dev/d3@7'

const width = 650 / 2
const margin = { top: 12, left: 30, bottom: 12, right: 4 }
const internalWidth = width - margin.left - margin.right
let clipNum = 1

function getYDomain (data) {
  const yMax = Math.ceil(d3.max(data, d => Math.abs(d[1])))
  return [-yMax, yMax]
}

function sine (id, data) {
  const height = d3.select(id).select('svg').attr('height')
  const internalHeight = height - margin.top - margin.bottom

  d3.select(id).selectChildren().remove()
  const svg = d3.select(id)
    .append('svg')
    .attr('width', width)
    .attr('height', height)

  const xDomain = d3.extent(data, d => d[0])
  const yDomain = getYDomain(data)

  const xScale = d3.scaleLinear().range([0, internalWidth]).domain(xDomain)
  const yScale = d3.scaleLinear().range([internalHeight, 0]).domain(yDomain)
  const xAxis = d3.axisBottom().scale(xScale).tickValues([1, 2, 3, 4, 5, 6])
  const yAxis = d3.axisLeft().scale(yScale).ticks(3)

  const line = d3.line()
    .x(d => xScale(d[0]))
    .y(d => yScale(d[1]))

  const clipId = 'clip' + clipNum++
  const clipGt1 = svg.append('clipPath')
    .attr('id', clipId)
  clipGt1.append('rect')
    .attr('x', xScale(0))
    .attr('y', yScale(yDomain[1]))
    .attr('width', xScale(xDomain[1]) - xScale(0))
    .attr('height', yScale(1) - yScale(yDomain[1]))
  clipGt1.append('rect')
    .attr('x', xScale(0))
    .attr('y', yScale(-1))
    .attr('width', xScale(xDomain[1]) - xScale(0))
    .attr('height', yScale(yDomain[0]) - yScale(-1))

  const g = svg.append('g')
    .attr('transform', 'translate(' + margin.left + ',' + margin.top + ')')
  // Are we showing clipped output?
  if (yDomain[0] < -1.0 || yDomain[1] > 1.0) {
    // Create a copy of the line data and add a single data point at which
    // forces the y value to 0 and will enable the path to be safely filled.
    const areaData = data.slice()
    areaData.push([xDomain[1], 0.0])
    g.append('path')
      .datum(areaData)
      .attr('d', line)
      .attr('stroke-width', 0)
      .attr('fill', 'red')
      .attr('clip-path', 'url(#' + clipId + ')')

    g.append('line')
      .attr('x1', xScale(xDomain[0]))
      .attr('y1', yScale(1))
      .attr('x2', xScale(xDomain[1]))
      .attr('y2', yScale(1))
      .attr('stroke', 'red')
      .attr('stroke-dasharray', 4)
    g.append('line')
      .attr('x1', xScale(xDomain[0]))
      .attr('y1', yScale(-1))
      .attr('x2', xScale(xDomain[1]))
      .attr('y2', yScale(-1))
      .attr('stroke', 'red')
      .attr('stroke-dasharray', 4)
  }
  g.append('path')
    .datum(data)
    .attr('d', line)
    .attr('stroke', 'black')
    .attr('stroke-width', 1.5)
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

    sine('#graph1', makeXArray().map(makePoints(c1)))
    sine('#graph2', makeXArray().map(makePoints(c2)))

    sine('#graphSum', makeXArray().map(x => [x, c1(x) + c2(x)]))
    sine('#graphSatSum', makeXArray().map(x => [x, Math.max(Math.min(c1(x) + c2(x), 1), -1)]))
    sine('#graphModSum', makeXArray().map(x => [x, (c1(x) + c2(x)) % 1.0]))
  }

  [frequency1, amplitude1, frequency2, amplitude2].forEach(el => el.addEventListener('input', update))
  update()
}
