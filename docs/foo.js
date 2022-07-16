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
  if (checkBox !== null) {
    const update = () => {
      const display = 'display:' + (checkBox.checked ? 'table-cell;' : 'none;')
      const columns = document.querySelectorAll(columnSelector)
      if (columns.length === 0) {
        console.log(`columns selector ${columnSelector} array is empty`)
      }
      document.querySelectorAll(columnSelector).forEach(el => el.setAttribute('style', display))
    }
    checkBox.addEventListener('change', update)
    update() // set initial state
  }
}
window.onload = () => {
  'use strict'
  showHide('#showX86-64', '.cpu-x86_64')
  showHide('#showARM64', '.cpu-arm64')
  showHide('#showARM', '.cpu-arm')

  showHideColumn('#show-target', '.column-target')
  showHideColumn('#show-cpp', '.column-cpp')
  hljs.highlightAll()
}
