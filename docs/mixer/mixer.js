import * as d3 from 'https://cdn.skypack.dev/d3@7'

function getYDomain (data) {
  const yMax = Math.ceil(d3.max(data, d => Math.abs(d[1])))
  return [-yMax, yMax]
}
function radiansToUser (v) {
  return d3.format('d')(v * (180 / Math.PI)) + '°'
}

// Create a copy of the line data and add data point at both x extremes to
// enable the path to be safely filled.
function areaData (data, xDomain) {
  const r = data.slice()
  r.unshift([xDomain[0], 0.0])
  r.push([xDomain[1], 0.0])
  return r
}
function sine (id, index) {
  const margin = { top: 12, left: 30, bottom: 12, right: 12 }

  const svg = d3.select(id).select('svg')
  const width = +svg.attr('width') - margin.left - margin.right
  const height = +svg.attr('height') - margin.top - margin.bottom

  const xDomain = [0, Math.PI * 2]
  const xScale = d3.scaleLinear().range([0, width]).domain(xDomain)
  const xAxis = d3.axisBottom().scale(xScale).tickValues([])
  const yAxis = d3.axisLeft().ticks(3)

  const clipPos = svg.select(`#clip-${index}-pos`).attr('x', xScale(0)).attr('width', xScale(xDomain[1]) - xScale(0))
  const clipNeg = svg.select(`#clip-${index}-neg`).attr('x', xScale(0)).attr('width', xScale(xDomain[1]) - xScale(0))

  const g = svg.append('g').attr('transform', `translate(${margin.left},${margin.top})`)
  const maxLine = g.append('line').classed('limit-line', true)
  const minLine = g.append('line').classed('limit-line', true)

  g.append('g').classed('axis', true).attr('transform', `translate(0, ${height / 2})`).call(xAxis)
  const yAxisEl = g.append('g').classed('axis', true)

  const fill = g.append('path').classed('limit-fill', true)
  const path = g.append('path').classed('line', true)

  const line = d3.line()
    .x(d => xScale(d[0]))

  svg.selectAll('.loading').remove()

  return data => {
    const yDomain = getYDomain(data)
    const yScale = d3.scaleLinear().range([height, 0]).domain(yDomain)
    yAxisEl.call(yAxis.scale(yScale))
    path.datum(data).attr('d', line.y(d => yScale(d[1])))

    // Set the fill path.
    fill.datum(areaData(data, xDomain)).attr('d', line).attr('clip-path', `url(#clip-${index})`)

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

export function mixerPage () {
  const inputControls = [{
    frequency: { range: document.querySelector('#frequency1'), value: document.querySelector('#freqvalue1') },
    amplitude: { range: document.querySelector('#amplitude1'), value: document.querySelector('#ampvalue1') },
    phase: { range: document.querySelector('#phase1'), value: document.querySelector('#phasevalue1') }
  }, {
    frequency: { range: document.querySelector('#frequency2'), value: document.querySelector('#freqvalue2') },
    amplitude: { range: document.querySelector('#amplitude2'), value: document.querySelector('#ampvalue2') },
    phase: { range: document.querySelector('#phase2'), value: document.querySelector('#phasevalue2') }
  }]

  const points = (width - margin.left - margin.right) / 2
  const makeXArray = () => d3.range(0, points).map(k => k / (points - 1) * Math.PI * 2)
  const yValue = input => x => Math.sin((x + +input.phase.range.value) * +input.frequency.range.value) * +input.amplitude.range.value
  const input1 = yValue(inputControls[0])
  const input2 = yValue(inputControls[1])

  const input1Graph = sine('#graph1', 1)
  const input2Graph = sine('#graph2', 2)

  const trueGraph = sine('#graphSum', 3)
  const satMathGraph = sine('#graphSatSum', 4)
  const moduloMathGraph = sine('#graphModSum', 5)

  const update = () => {
    inputControls.forEach(setValues)
    input1Graph(makeXArray().map(x => [x, input1(x)]))
    input2Graph(makeXArray().map(x => [x, input2(x)]))

    trueGraph(makeXArray().map(x => [x, input1(x) + input2(x)]))
    satMathGraph(makeXArray().map(x => [x, Math.max(Math.min(input1(x) + input2(x), 1.0), -1.0)]))
    moduloMathGraph(makeXArray().map(x => [x, (input1(x) + input2(x)) % 1.0]))
  }

  inputControls.forEach(v => [v.frequency, v.amplitude, v.phase].forEach(p => p.range.addEventListener('input', update)))
  update()
}
