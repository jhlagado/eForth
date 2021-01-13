#pragma once
#include <Arduino.h>

const char index_html[] PROGMEM = R"=====(
<!html>
<head>
<title>eForth (esp32forth)</title>
<style>
body {
    background-color: black;
    color: #00e676;
    font-family: 'Source Code Pro', monospace;
    font-size: 20px;
    height: 100%;
    margin: 10px;
    overflow: hidden;
    width: 100%;
}
#screen {
    display: flex;
    flex-direction: column;
    height: 100%;
    overflow-y: scroll;
    position: absolute;
    width: 100%;
}
#buttons * {
    background-color: #4CAF50;
    border: none;
    border-radius: 3px;
    color: white;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
    margin: 4px 2px;
    cursor: pointer;
}
#output {
    background-color: black;
    border: 0;
    color: #00e676;
    flex-grow: 1;
    font-family: 'Source Code Pro', monospace;
    font-size: 16px;
    height: 80%;
    resize: none;
    width: 100%;
}
#prompt {
    background-color: black;
    border: 0;
    color: #00e676;
    font-family: 'Source Code Pro', monospace;
    font-size: 16px;
    outline:0;
    padding: 5px;
    width: 100%;
}
</style>
</head>
<body>
<div id="screen">
<div id="buttons">
    eForth <input id="filepick" type="file" name="files[]"></input>
    <button onclick="ask('hex')">hex</button>
    <button onclick="ask('decimal')">decimal</button>
    <button onclick="ask('words')">words</button>
</div>
<textarea id="output" readonly></textarea>
<input id="prompt" type="text"></input>
</div>
<script>
const prompt = document.getElementById('prompt');
const filepick = document.getElementById('filepick');
const output = document.getElementById('output');
async function httpPost(url, items) {
    const fd = new FormData();
    for (k in items) {
        fd.append(k, items[k]);
    }
    try {
        const response = await fetch(url, {method:'POST', body: fd})
        const text = await response.text();
        return text;
    } catch (e) {
        return null;
    }
}
async function ask(cmd) {
    const data = await httpPost('/input', {cmd: cmd + '\n'});
    if (data !== null) { output.value += data; }
    output.scrollTop = output.scrollHeight;  // Scroll to the bottom
}
prompt.onkeyup = async function(event) {
  if (event.keyCode === 13) {
    event.preventDefault();
    await ask(prompt.value);
    prompt.value = '';
  }
};
prompt.onblur = () => {
    prompt.focus();
};
prompt.focus();
filepick.onchange = function(event) {
  if (event.target.files.length > 0) {
    const reader = new FileReader();
    reader.onload = function(e) {
      const parts = e.target.result.split('\\n');
      async function upload() {
        if (parts.length === 0) { filepick.value = ''; return; }
        await ask(parts.shift());
        upload();
      }
      upload();
    }
    reader.readAsText(event.target.files[0]);
  }
};
window.onload = async function() {
  await ask('');
  prompt.focus();
};
</script>
</body>
</html>
)=====";
