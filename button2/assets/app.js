const ui = new WebUI();
const statusEl = document.querySelector('#status');
const timers = {};

ui.on_connect(() => {
  statusEl.textContent = '연결됨';
  statusEl.className = 'status on';
});

ui.on_disconnect(() => {
  statusEl.textContent = '연결 끊김';
  statusEl.className = 'status off';
});

function showCounts(counts) {
  document.querySelector('#count-A').textContent = counts.A;
  document.querySelector('#count-B').textContent = counts.B;
}

function flash(name) {
  const box = document.querySelector(`#box-${name}`);
  if (!box) return;
  box.classList.add('flash');
  clearTimeout(timers[name]);
  timers[name] = setTimeout(() => box.classList.remove('flash'), 300);
}

ui.on_message('counts', data => showCounts(data.counts));

ui.on_message('pressed', data => {
  showCounts(data.counts);
  flash(data.last);
});

document.querySelector('#reset').addEventListener('click', () => {
  ui.send_message('reset');
});

fetch('/counts')
  .then(r => r.json())
  .then(showCounts)
  .catch(() => {});
