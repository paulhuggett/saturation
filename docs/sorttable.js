/* global Node */

// This code is derived from SortTable version 2 by Stuart Langridge. It has
// been heavily revised to use “modern” language and browser features as well
// as to remove functionality that I don’t need.

/*
  SortTable
  version 2
  7th April 2007
  Stuart Langridge, http://www.kryogenix.org/code/browser/sorttable/

  Instructions:
  Download this file
  Add <script src="sorttable.js"></script> to your HTML
  Add class="sortable" to any table you'd like to make sortable
  Click on the headers to sort

  Thanks to many, many people for contributions and suggestions.
  Licenced as X11: http://www.kryogenix.org/code/browser/licence.html
  This basically means: do what you want with it.
*/

const sorttable = (function () {
  'use strict'

  const increasing = {
    indicatorClass: 'sorttable_incrind',
    headClass: 'sorttable_sorted',
    innerHTML: '&nbsp;&#x25B4;' // // U+25B4 BLACK UP-POINTING SMALL TRIANGLE
  }
  const decreasing = {
    indicatorClass: 'sorttable_decind',
    headClass: 'sorttable_sorted_reverse',
    innerHTML: '&nbsp;&#x25BE;' // U+25BE BLACK DOWN-POINTING SMALL TRIANGLE
  }

  // Credit: https://observablehq.com/@mbostock/localized-number-parsing
  class NumberParser {
    constructor (locale) {
      const parts = new Intl.NumberFormat(locale).formatToParts(12345.6)
      const numerals = [...new Intl.NumberFormat(locale, { useGrouping: false }).format(9876543210)].reverse()
      const index = new Map(numerals.map((d, i) => [d, i]))
      this._group = new RegExp(`[${parts.find(d => d.type === 'group').value}]`, 'g')
      this._decimal = new RegExp(`[${parts.find(d => d.type === 'decimal').value}]`)
      this._numeral = new RegExp(`[${numerals.join('')}]`, 'g')
      this._index = d => index.get(d)
    }

    parse (string) {
      string = string.trim().replace(this._group, '').replace(this._decimal, '.').replace(this._numeral, this._index)
      return (string) ? +string : NaN
    }
  }

  // gets the text we want to use for sorting for a cell.
  // strips leading and trailing whitespace.
  function getInnerText (node) {
    return node.textContent.trim()
  }

  // guess the type of a column based on its first non-blank row
  function guessType (table, column) {
    const collator = new Intl.Collator('en').compare
    const numberParser = new NumberParser('en')

    const sortNop = (a, b) => 0
    const sortAlpha = (a, b) => collator(a[0], b[0])
    const sortNumeric = (a, b) => {
      const aa = numberParser.parse(a[0])
      const bb = numberParser.parse(b[0])
      return (isNaN(aa) ? 0 : aa) - (isNaN(bb) ? 0 : bb)
    }

    return Array.from(table.tBodies[0].rows).map(el => {
      // Pick a suitable sorting function for this cell.
      const text = getInnerText(el.cells[column])
      if (text.length === 0) {
        return sortNop // No text, keep looking.
      }
      // Choose the sort based on the contents of this cell.
      return isNaN(numberParser.parse(text)) ? sortAlpha : sortNumeric
    }).reduce((acc, el) => {
      // If we've guessed something already, just return it.
      return acc !== sortNop ? acc : el
    }, sortNop)
  }

  function createIndicator (indicator) {
    const el = document.createElement('span')
    el.classList.add(indicator.indicatorClass)
    el.innerHTML = indicator.innerHTML
    return el
  }

  // If we're already sorted by this column, just reverse the table.
  function reverseOrder (table, headCell, a, b) {
    const tbody = table.tBodies[0]
    Array.from(tbody.rows).reverse().forEach(x => tbody.appendChild(x))

    headCell.classList.replace(a.headClass, b.headClass)
    headCell.parentElement.querySelector('.' + a.indicatorClass).replaceWith(createIndicator(b))
  }

  function columnClick (table, headCell) {
    if (headCell.classList.contains(increasing.headClass)) {
      return reverseOrder(table, headCell, increasing, decreasing)
    }
    if (headCell.classList.contains(decreasing.headClass)) {
      return reverseOrder(table, headCell, decreasing, increasing)
    }

    // Remove the sorttable_sorted classes from each element in the header row.
    headCell.parentNode.childNodes.forEach(el => {
      if (el.nodeType === Node.ELEMENT_NODE) {
        el.classList.remove(decreasing.headClass)
        el.classList.remove(increasing.headClass)
      }
    })
    // Remove any exiting indicator element.
    const removeIfFound = element => {
      if (element) {
        element.remove()
      }
    }
    removeIfFound(table.querySelector('.' + decreasing.indicatorClass))
    removeIfFound(table.querySelector('.' + increasing.indicatorClass))
    headCell.classList.add(increasing.headClass)
    headCell.appendChild(createIndicator(increasing))

    // build an array to sort. This is a Schwartzian transform thing,
    // i.e., we "decorate" each row with the actual sort key,
    // sort based on the sort keys, and then put the rows back in order
    // which is a lot faster because you only do getInnerText once per row
    Array.from(table.tBodies[0].rows)
      .map(row => [getInnerText(row.cells[headCell.sorttable_columnIndex]), row])
      .sort(headCell.sorttable_sortFunction)
      .forEach(r => table.tBodies[0].appendChild(r[1]))
  }

  return {
    makeSortable: function (table) {
      if (table.querySelector('thead') === null) {
        // The table doesn't have a tHead so create one and put the first table row in it.
        const tHead = document.createElement('thead')
        tHead.appendChild(table.rows[0])
        table.insertBefore(tHead, table.firstChild)
      }
      if (table.tHead.rows.length !== 1) {
        return // Can only cope with one header row.
      }

      // work through each column, calculate its type, and listen for a click.
      Array.from(table.tHead.rows[0].cells).forEach((cell, index) => {
        if (cell.classList.contains('sorttable_nosort')) {
          return // Skip this column.
        }
        cell.sorttable_sortFunction = guessType(table, index)
        cell.sorttable_columnIndex = index
        cell.addEventListener('click', _ => columnClick(table, cell))
      })
    }
  }
})()
