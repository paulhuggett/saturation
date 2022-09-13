import * as d3 from 'https://cdn.skypack.dev/d3@7'

function outputArray (samples, domain, b, x0) {
  const arr = Array(samples).fill(0)
  arr[0] = 1.0 // the initial impulse.
  let z = 0.0 // z is z^-1: the previous sample.
  return arr.map((v, idx) => {
    z = z * b + v - x0
    return [idx / samples * domain, z]
  })
}

function envelope (svgSelector) {
  const margin = { top: 12, left: 30, bottom: 30, right: 12 }

  const svg = d3.select(svgSelector)
  svg.selectAll('.loading').remove() // Remove the loading animation.

  const width = +svg.attr('width') - margin.left - margin.right
  const height = +svg.attr('height') - margin.top - margin.bottom
  const xScale = d3.scaleLinear().range([0, width]).domain([0.0, 2.0])
  const yScale = d3.scaleLinear().range([height, 0])

  const line = d3.line().x(d => xScale(d[0])).y(d => yScale(d[1]))

  const g = svg.append('g').attr('transform', `translate(${margin.left},${margin.top})`)
  const xAxis = g.append('g').classed('axis', true)
  const yAxis = g.append('g').classed('axis', true)
  const path = g.append('path').classed('line', true)

  return (invert, b, x0) => {
    const data = outputArray(width, xScale.domain()[1], b, x0)
      .map(d => invert ? [d[0], 1.0 - d[1]] : d)

    const yDomain = d3.extent(data, d => d[1])
    yDomain[0] = Math.min(yDomain[0], 0.0)
    yScale.domain(yDomain)
    xAxis.attr('transform', `translate(0, ${yScale(0.0)} )`)
      .call(d3.axisBottom().scale(xScale))
    yAxis.call(d3.axisLeft().scale(yScale))
    path.datum(data).attr('d', line)
  }
}

export function controls () {
  const chargeDischarge = {
    charge: document.querySelector('#radio-charge'),
    discharge: document.querySelector('#radio-discharge')
  }

  const inputControls = {
    t: { range: document.querySelector('#ts-range'), value: document.querySelector('#ts-value') },
    b: { range: document.querySelector('#b-range'), value: document.querySelector('#b-value') },
    x: { range: document.querySelector('#x0-range'), value: document.querySelector('#x0-value') }
  }

  const drawChart = envelope('#envelope')
  const update = () => drawChart(chargeDischarge.charge.checked, inputControls.b.range.value, inputControls.x.range.value)

  const values = Object.values(inputControls)
  // Called when one of the 'range' (slider) controls has a new value.
  const rangeEvent = () => {
    values.forEach(v => { v.value.value = v.range.value }) // Make the text control match the slider.
    update() // Redraw the chart.
  }
  values.forEach(v => {
    v.range.addEventListener('input', rangeEvent)
    v.value.addEventListener('input', () => {
      if (values.every(v => v.value.validity.valid)) {
        values.forEach(v => { v.range.value = v.value.value })
        update()
      }
    })
  })
  Object.values(chargeDischarge).forEach(el => {
    el.addEventListener('change', _ => {
      update()
    })
  })
  rangeEvent()
}
