// Obsługa przełącznika
document.getElementById("system-toggle").addEventListener("change", (event) => {
  const isChecked = event.target.checked;
  document.getElementById("system-status").textContent = isChecked ? "Włączony" : "Wyłączony";
});
