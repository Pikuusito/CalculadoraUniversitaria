# Calculadora Universitaria 🧮

Una herramienta de cálculo diseñada especialmente para estudiantes universitarios. Permite calcular rápidamente el progreso y la nota final de un curso específico, así como el Promedio General Acumulado (PGA) de todos tus cursos ponderado por créditos.

El proyecto cuenta con **dos versiones** que comparten la misma lógica matemática subyacente: una versión web moderna y una versión clásica de consola en C++.

## ✨ Versión Web (Recomendada)

La versión web es una aplicación interactiva, moderna y 100% responsiva (Mobile-First) diseñada para ser utilizada desde cualquier dispositivo. 

**Características de la Web:**
*   **Diseño Premium:** Interfaz oscura (Dark Mode) utilizando principios de *Glassmorphism*, con ventanas translúcidas, colores vibrantes y animaciones fluidas al interactuar con los elementos.
*   **Totalmente Dinámica:** Añade sub-notas o múltiples cursos para tu PGA al instante sin tener que recargar la página.
*   **Barra de Progreso Visual:** Observa cómo se llena en vivo el porcentaje de tu curso a medida que introduces notas.
*   **Sistema de Metas:** Si aún no completas el curso, la calculadora te dice exactamente qué nota necesitas en el resto de tus evaluaciones para lograr promedios desde 10.5 hasta 20.
*   **Notificaciones Emergentes (Toasts):** Alertas visuales no obstructivas al calcular información o cometer errores.

**🌐 Pruébalo ahora mismo:**
La aplicación está construida únicamente con HTML5, CSS3 y Vanilla JavaScript, lo que la hace ultra ligera y perfecta para alojar gratuitamente en plataformas como Vercel o GitHub Pages.
Para ejecutarla localmente, simplemente abre el archivo `web/index.html` en tu navegador de preferencia.

---

## 💻 Versión Consola (C++)

Para aquellos que prefieren la velocidad y simplicidad de la terminal nativa, la versión original escrita en C++ ofrece una experiencia textual enriquecida y robusta.

**Características de la Consola:**
*   **Interfaz CLI Amigable:** Uso de colores de consola nativos para los menús y tablas de resultados.
*   **Diálogos Nativos (Pop-ups):** Integración con la API de Windows (`MessageBoxA`) utilizando hilos independientes (`std::thread`), para mostrar ventanas reales de Windows sin congelar la terminal de comandos.
*   **Manejo de Errores Riguroso:** Sistema blindado capaz de sobrevivir a entradas múltiples e incorrectas (letras en lugares de números, nombres de cursos con múltiples espacios, etc).

### Requisitos Previos (C++)
*   Sistema Operativo **Windows** (debido al uso de `<windows.h>`).
*   Un compilador de C++ (ejemplo: MinGW / `g++`).

### Compilación (C++)
1.  Clona este repositorio:
    ```bash
    git clone https://github.com/Pikuusito/CalculadoraUniversitaria.git
    cd CalculadoraUniversitaria
    ```
2.  Compilado de `calculadora.cpp`:
    ```bash
    g++ calculadora.cpp -o calculadora
    ```
3.  Ejecución en terminal:
    ```bash
    .\calculadora.exe
    ```

## 🛠️ Tecnologías Usadas
*   **Web Frontend:** HTML5, CSS3 variables, Vanilla JS, Lucide Icons.
*   **Backend Analógico:** C++ (Librerías estándar + `windows.h` + `thread`).
