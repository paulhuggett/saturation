import * as d3 from 'https://cdn.skypack.dev/d3@7'

const width = 520
const margin = { top: 24, left: 40, bottom: 24, right: 10 }
export const internalWidth = width - margin.left - margin.right

export function sine (id, height, title, data) {
  const internalHeight = height - margin.top - margin.bottom

  d3.select(id).selectChildren().remove()
  const svg = d3.select(id)
    .append('svg')
    .attr('width', width)
    .attr('height', height)

  const rnd = x => x >= 0 ? Math.ceil(x) : Math.floor(x)
  const xScale = d3.scaleLinear().range([0, internalWidth]).domain(d3.extent(data, d => d[0]))
  const yExtent = d3.extent(data, d => rnd(d[1]))
  const yScale = d3.scaleLinear().range([internalHeight, 0]).domain(yExtent)
  const xAxis = d3.axisBottom().scale(xScale)
  const yAxis = d3.axisLeft().scale(yScale)
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
