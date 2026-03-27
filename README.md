<img width="376" height="272" alt="image" src="https://github.com/user-attachments/assets/c093d677-043c-44f0-9e68-b55a4f96ad38" />

# 🚀 PomoCli

Una herramienta de productividad **Pomodoro** minimalista y moderna construida en **C++17** para la terminal, utilizando la potencia de **FTXUI**.

Diseñada para desarrolladores que buscan un flujo de trabajo sin distracciones directamente desde su emulador de terminal favorito.

---

##  Características

* **🖥️ Interfaz TUI:** Elegante, reactiva y con barras de progreso dinámicas.
* **🔔 Alertas Sonoras:** Notificaciones de audio al cambiar de fase (compatible con `pw-play`, `paplay` o `aplay`).
* **📩 Notificaciones de Escritorio:** Integración nativa con `libnotify` (`notify-send`) para alertas visuales.
* **💾 Persistencia de Datos:** Guardado automático de tus sesiones en un archivo `stats.json` para seguimiento de progreso.
* **⌨️ Controles Rápidos:**
    * `Espacio`: Pausar / Reanudar el cronómetro.
    * `S`: Saltar (Skip) el ciclo actual.
    * `Q`: Salir de la aplicación de forma segura.

## 🛠️ Requisitos (Arch Linux)

Asegúrate de tener instaladas las dependencias base para compilar y ejecutar las alertas:

```bash
sudo pacman -S cmake base-devel alsa-utils libnotify
