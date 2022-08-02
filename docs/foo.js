/* global hljs */

function showHide (checkBoxSelector, rowSelector) {
  'use strict'

  const checkBox = document.querySelector(checkBoxSelector)
  if (checkBox === null) {
    console.log(`selector ${checkBoxSelector} was not found`)
    return
  }
  const update = () => {
    const display = 'display:' + (checkBox.checked ? 'table-row;' : 'none;')
    const rows = document.querySelectorAll(rowSelector)
    if (rows.length === 0) {
      console.log(`row selector ${rowSelector} array is empty`)
    }
    rows.forEach(el => el.setAttribute('style', display))
  }
  checkBox.addEventListener('change', update)
  update() // set initial state
}

function showHideColumn (checkBoxSelector, columnSelector) {
  'use strict'

  const checkBox = document.querySelector(checkBoxSelector)
  if (checkBox === null) {
    console.log(`checkbox ${checkBoxSelector} was not found`)
    return
  }
  const update = () => {
    const display = 'display:' + (checkBox.checked ? 'table-cell;' : 'none;')
    const columns = document.querySelectorAll(columnSelector)
    if (columns.length === 0) {
      console.log(`columns selector ${columnSelector} array is empty`)
    }
    columns.forEach(el => el.setAttribute('style', display))
  }
  checkBox.addEventListener('change', update)
  update() // set initial state
}

window.onload = () => {
  'use strict'
  showHideColumn('#show-target', '.column-target')
  showHideColumn('#show-cpp', '.column-cpp')
  showHideColumn('#show-op', '.column-op')
  showHideColumn('#show-asm', '.column-asm')

  const menus =
    ['#sign-select', '#bits-select', '#targets-select', '#op-select'].map(id => {
      const el = document.querySelector(id)
      if (el === null) {
        console.error(`${id} was not found`)
      }
      return el
    }).filter(el => el !== null)
  const body = document.querySelector('tbody')
  const change = () => {
    // Show just the selected rows.
    const selector =
        menus.map(el => el.value).filter(s => s.length > 0).join('.')
    const dot = selector.length > 0 ? '.' : ''
    // Hide all of rows.
    body.querySelectorAll('tr').forEach(el => el.setAttribute('style', 'display:none;'))
    // Show the selected ones.
    body.querySelectorAll(`tr${dot}${selector}`).forEach(el => el.setAttribute('style', 'display:table-row;'))
  }

  menus.forEach(el => el.addEventListener('change', change))
  change() // make the correct table rows visible
}
