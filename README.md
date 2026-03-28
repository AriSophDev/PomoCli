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
```
## Instalacion 

```bash
git clone https://github.com/AriSophDev/PomoCli.git
```
```bash
./install.sh
```


## 🗺️ Roadmap de Desarrollo

### 🟢 Fase 1: Core & Estabilidad (Completado)
- [x] **Motor TUI:** Interfaz reactiva con `FTXUI`.
- [x] **Lógica de Hilos:** Temporizador asíncrono en C++ (evita bloqueos de UI).
- [x] **Gestión de CMake:** Configuración con `FetchContent` para dependencias automáticas.


### 🟡 Fase 2: Experiencia de Usuario & Datos (En Progreso)
- [x] **Controles de Teclado:** Soporte para `Espacio` (Pausa), `S` (Skip) y `Q` (Salir).
- [ ] **Persistencia JSON:** Guardado automático de sesiones en `stats.json` (`nlohmann/json`).
- [ ] **Temas Dinámicos:** Cambios de color (Rojo/Verde/Azul) según el estado del timer.
- [ ] **Argumentos de Usuario:** Soporte para tiempos personalizados (e.g., `./pomodoro 25 5`).

### 🔵 Fase 3: Portabilidad & Distribución (Próximamente)
- [ ] **Soporte Fedora:** Crear un archivo `.spec` o instrucciones para `dnf` y asegurar compatibilidad con `Pipewire`.
- [ ] **Soporte macOS:** Adaptar las llamadas de sistema de audio a `afplay` (nativo de Mac) y soporte para `brew`.
- [ ] **Binary Release:** Scripts de instalación automatizada (`install.sh`) para diferentes distros.
