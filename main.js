document.getElementById("executeButton").addEventListener("click", function() {
    // Llama a una función para ejecutar el script Bash
    executeBashScript();
});

function executeBashScript() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "start.sh", true);
    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
            // Muestra la salida del script en un div
            document.getElementById("output").innerHTML = "<pre>" + xhr.responseText + "</pre>";
        }
    };
    xhr.send();
}
