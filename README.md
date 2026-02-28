# Calculadora Universitaria en C++ 🧮

Una herramienta de consola interactiva diseñada para estudiantes universitarios, escrita en C++. Permite calcular rápidamente el progreso y la nota final de un curso específico, así como el Promedio General Acumulado (PGA) de todos los cursos.

## ✨ Características Principales

*   **Interfaz de Consola Amigable:** Interfaz text-based robusta con uso de colores para mejorar la legibilidad y la experiencia de usuario.
*   **Cálculo de Notas de Curso:**
    *   Soporta la división de un curso en múltiples componentes (Unidad 1, Examen Parcial, Unidad 2, Examen Final), con pesos.
    *   Permite el ingreso de sub-notas (por ejemplo, múltiples prácticas o tareas dentro de una misma unidad).
    *   Muestra un reporte final detallado y una barra de progreso visual de cuánto porcentaje del curso se ha completado.
    *   Calcula cuánto necesitas en las evaluaciones restantes para alcanzar diferentes promedios de aprobación (desde 10.5 hasta 20).
*   **Cálculo de PGA (Promedio General Acumulado):**
    *   Ingresa múltiples cursos, sus notas y la cantidad de créditos correspondientes.
    *   Calcula el promedio ponderado exacto de tu ciclo o carrera.
*   **Ventanas Emergentes Nativas (Pop-ups):** Utiliza la API de Windows (`MessageBoxA`) en hilos independientes (`std::thread`) para mostrar resultados y alertas sin bloquear el flujo de la consola.
*   **Manejo de Errores Riguroso:** Blindado contra entradas incorrectas del usuario (como ingresar letras en lugar de números o nombres con múltiples espacios).

## 🚀 Cómo Empezar

### Requisitos Previos

*   Sistema Operativo **Windows** (debido al uso de `<windows.h>` para colores de consola y ventanas emergentes).
*   Un compilador de C++ (como **MinGW** / `g++`).

### Instalación y Compilación

1.  Clona este repositorio o descarga el código fuente:
    ```bash
    git clone https://github.com/Pikuusito/CalculadoraPromedio.git
    cd CalculadoraPromedio
    ```

2.  Compila el archivo `calculadora.cpp` desde tu terminal:
    ```bash
    g++ calculadora.cpp -o calculadora
    ```

3.  Ejecuta el programa:
    ```bash
    .\calculadora
    ```

## 🛠️ Tecnologías Usadas

*   **C++ Estándar:** `iostream`, `string`, `vector`, `iomanip`, `thread`.
*   **Windows API:** `windows.h` (Para la personalización visual de la consola y diálogos nativos).

