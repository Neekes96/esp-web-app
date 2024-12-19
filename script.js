// Wstępne dane (na razie statyczne)
const mockData = {
  status: "Wyłączony",
  avgMoisture: 55,
  temperature: 22.5,
};

// Aktualizacja danych na stronie
function updateUI() {
  document.getElementById("system-status").textContent = mockData.status;
  document.getElementById("avg-moisture").textContent = mockData.avgMoisture;
  document.getElementById("temperature").textContent = mockData.temperature;
}

// Obsługa przełącznika
document.getElementById("system-toggle").addEventListener("change", (event) => {
  mockData.status = event.target.checked ? "Włączony" : "Wyłączony";
  updateUI();
});

// Inicjalizacja UI
updateUI();
