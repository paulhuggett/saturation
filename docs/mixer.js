import * as d3 from 'https://cdn.skypack.dev/d3@7'

function getYDomain (data) {
  const yMax = Math.ceil(d3.max(data, d => Math.abs(d[1])))
  return [-yMax, yMax]
}
function radiansToUser (v) {
  const format = d3.format('d')
  return format(v * (180 / Math.PI)) + '°'
}
function sine (id, index) {
  const margin = { top: 12, left: 30, bottom: 12, right: 12 }

  const svg = d3.select(id).select('svg')
  const width = +svg.attr('width') - margin.left - margin.right
  const height = +svg.attr('height') - margin.top - margin.bottom

  const xDomain = [0, Math.PI * 2]
  const xScale = d3.scaleLinear().range([0, width]).domain(xDomain)
  const xAxis = d3.axisBottom().scale(xScale)
    .tickValues(Array(4).fill(0).map((_, i) => (i + 1) / 2 * Math.PI))
    .tickFormat(radiansToUser)
  const yAxis = d3.axisLeft().ticks(3)

  const clipPos = svg.select(`#clip-${index}-pos`).attr('x', xScale(0)).attr('width', xScale(xDomain[1]) - xScale(0))
  const clipNeg = svg.select(`#clip-${index}-neg`).attr('x', xScale(0)).attr('width', xScale(xDomain[1]) - xScale(0))

  const g = svg.append('g').attr('transform', `translate(${margin.left},${margin.top})`)
  const maxLine = g.append('line').classed('limit-line', true)
  const minLine = g.append('line').classed('limit-line', true)

  const path = g.append('path').classed('line', true)
  const fill = g.append('path').classed('fill', true)

  g.append('g').classed('axis', true).attr('transform', `translate(0, ${height / 2})`).call(xAxis)
  const yAxisEl = g.append('g').classed('yaxis', true)

  const line = d3.line()
    .x(d => xScale(d[0]))

  svg.selectAll('.loading').remove()

  return data => {
    const yDomain = getYDomain(data)
    const yScale = d3.scaleLinear().range([height, 0]).domain(yDomain)
    yAxisEl.call(yAxis.scale(yScale))
    path.datum(data).attr('d', line.y(d => yScale(d[1])))

    // Create a copy of the line data and add a single data point at which
    // forces the y value to 0 and will enable the path to be safely filled.
    const areaData = data.slice()
    areaData.unshift([xDomain[0], 0.0])
    areaData.push([xDomain[1], 0.0])
    // Set the fill path.
    fill.datum(areaData).attr('d', line).attr('clip-path', `url(#clip-${index})`)

    const redLine = {
      yPlus1: yScale(1.0),
      yMinus1: yScale(-1.0),
      x1: xScale(xDomain[0]),
      x2: xScale(xDomain[yDomain[0] < -1.0 || yDomain[1] > 1.0 ? 1.0 : 0.0])
    }
    clipPos.attr('y', yScale(yDomain[1])).attr('height', redLine.yPlus1 - yScale(yDomain[1]))
    clipNeg.attr('y', redLine.yMinus1).attr('height', yScale(yDomain[0]) - redLine.yMinus1)
    maxLine
      .attr('x1', redLine.x1).attr('y1', redLine.yPlus1)
      .attr('x2', redLine.x2).attr('y2', redLine.yPlus1)
    minLine
      .attr('x1', redLine.x1).attr('y1', redLine.yMinus1)
      .attr('x2', redLine.x2).attr('y2', redLine.yMinus1)
  }
}

function setValues (input) {
  input.frequency.value.innerText = input.frequency.range.value
  input.amplitude.value.innerText = input.amplitude.range.value
  input.phase.value.innerText = radiansToUser(input.phase.range.value)
}

const width = 650 / 2
const margin = { top: 12, left: 30, bottom: 12, right: 4 }
const internalWidth = width - margin.left - margin.right

export function mixerPage () {
  const input = [{
    frequency: { range: document.querySelector('#frequency1'), value: document.querySelector('#freqvalue1') },
    amplitude: { range: document.querySelector('#amplitude1'), value: document.querySelector('#ampvalue1') },
    phase: { range: document.querySelector('#phase1'), value: document.querySelector('#phasevalue1') }
  }, {
    frequency: { range: document.querySelector('#frequency2'), value: document.querySelector('#freqvalue2') },
    amplitude: { range: document.querySelector('#amplitude2'), value: document.querySelector('#ampvalue2') },
    phase: { range: document.querySelector('#phase2'), value: document.querySelector('#phasevalue2') }
  }]

  const points = internalWidth / 2
  const makeXArray = () => d3.range(0, points).map(k => k / (points - 1) * Math.PI * 2)
  const yValue = input => x => Math.sin((x + parseFloat(input.phase.range.value)) * parseFloat(input.frequency.range.value)) * parseFloat(input.amplitude.range.value)
  const makePoints = f => x => [x, f(x)]

  const c1 = yValue(input[0])
  const c2 = yValue(input[1])

  const g1 = sine('#graph1', 1)
  const g2 = sine('#graph2', 2)

  const g3 = sine('#graphSum', 3)
  const g4 = sine('#graphSatSum', 4)
  const g5 = sine('#graphModSum', 5)

  const update = () => {
    setValues(input[0])
    setValues(input[1])
    g1(makeXArray().map(makePoints(yValue(input[0]))))
    g2(makeXArray().map(makePoints(yValue(input[1]))))
    g3(makeXArray().map(x => [x, c1(x) + c2(x)]))
    g4(makeXArray().map(x => [x, Math.max(Math.min(c1(x) + c2(x), 1.0), -1.0)]))
    g5(makeXArray().map(x => [x, (c1(x) + c2(x)) % 1.0]))
  }

  [
    input[0].frequency.range,
    input[0].amplitude.range,
    input[0].phase.range,
    input[1].frequency.range,
    input[1].amplitude.range,
    input[1].phase.range
  ].forEach(el => el.addEventListener('input', update))
  update()
}
