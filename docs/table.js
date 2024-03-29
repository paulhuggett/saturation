import { sortable } from './sorttable.js'

function showHideColumn (checkBoxSelector, columnSelector) {
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

function findMenuElements (menuIDs) {
  return menuIDs.map(id => {
    const el = document.querySelector(id)
    if (el === null) {
      console.error(`${id} was not found`)
    }
    return el
  }).filter(el => el !== null)
}

// Show just the selected rows.
function showSelectedRows (tableBodyEl, menus) {
  // Turn the menu selections into a CSS selector.
  const selector =
      menus.map(el => el.value).filter(s => s.length > 0).join('.')
  const dot = selector.length > 0 ? '.' : ''
  // Hide all of rows.
  tableBodyEl.querySelectorAll('tr').forEach(el => el.setAttribute('style', 'display:none;'))
  // Show the selected ones.
  tableBodyEl.querySelectorAll(`tr${dot}${selector}`).forEach(el => el.setAttribute('style', 'display:table-row;'))
}

window.addEventListener('DOMContentLoaded', _ => {
  showHideColumn('#show-target', '.column-target')
  showHideColumn('#show-cpp', '.column-cpp')
  showHideColumn('#show-op', '.column-op')
  showHideColumn('#show-asm', '.column-asm')

  const body = document.querySelector('tbody')
  const menus = findMenuElements(['#sign-select', '#bits-select', '#targets-select', '#op-select'])
  menus.forEach(el => el.addEventListener('change', _ => showSelectedRows(body, menus)))
  sortable.makeSortable(document.getElementById('out-table'))
  showSelectedRows(body, menus) // make the correct table rows visible
})
