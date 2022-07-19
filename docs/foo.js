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

  const targetsSelect = document.querySelector('#targets-select')
  if (targetsSelect == null) {
    console.error('#targets-select select was not found')
  }
  const opSelect = document.querySelector('#op-select')
  if (opSelect == null) {
    console.error('#op-select select was not found')
  }
  const body = document.querySelector('tbody')
  const change = () => {
    // Hide all of rows.
    body.querySelectorAll('tr').forEach(el => el.setAttribute('style', 'display:none;'))
    // Show just the selected rows.
    body.querySelectorAll(`tr.${targetsSelect.value}.${opSelect.value}`).forEach(el => el.setAttribute('style', 'display:table-row;'))
  }
  targetsSelect.addEventListener('change', change)
  opSelect.addEventListener('change', change)
  change() // make the correct table rows visible
}
