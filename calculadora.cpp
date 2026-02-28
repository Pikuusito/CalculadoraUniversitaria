#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>   
#include <windows.h> 
#include <thread>   

using namespace std;

// --- HERRAMIENTAS DE INTERFAZ (UI) ---

// Colores: 10=Verde, 11=Cyan, 12=Rojo, 14=Amarillo, 15=Blanco, 7=Gris, 8=GrisOscuro
void color(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void limpiar() {
    system("cls");
}

void linea(int largo = 60) {
    color(8); 
    for(int i = 0; i < largo; i++) cout << "-";
    cout << endl;
    color(7); 
}

void encabezado(string titulo) {
    limpiar();
    color(11); 
    cout << "============================================================" << endl;
    cout << "   " << titulo << endl;
    cout << "============================================================" << endl;
    color(7);
    cout << endl;
}

// --- LOGICA DE LA CALCULADORA ---

struct ResultadoComponente {
    float puntosGlobalesGanados;     
    float porcentajeGlobalAvanzado;  
};

// Procesa una de las partes del curso con interfaz mejorada
ResultadoComponente procesarComponente(string nombre, float pesoEnPromedio, bool permitirSubnotas) {
    float acumuladoInterno = 0;
    float porcentajeInternoCompletado = 0;
    char confirmacion = 's';

    do {
        // Mostramos encabezado limpio para cada seccion
        encabezado(nombre + " (Vale " + to_string((int)(pesoEnPromedio * 100)) + "% del curso)");
        
        char tieneSubnotas = 'n';
        
        if (permitirSubnotas) {
            color(14); // Amarillo para preguntas
            cout << char(168) << "Tiene sub-notas con porcentajes internos? (s/n): ";
            color(15); // Blanco brillante para input
            cin >> tieneSubnotas;
            cin.ignore(10000, '\n'); // Limpiar exceso
        }

        if (tieneSubnotas == 's' || tieneSubnotas == 'S') {
            int cantidad = 0;
            cout << endl;
            color(14); cout << char(168) << "Cuantas sub-notas tiene " << nombre << "? ";
            color(15); cin >> cantidad;
            if(cin.fail()){ cin.clear(); cin.ignore(10000, '\n'); }
            cout << endl;

            linea();
            for (int i = 0; i < cantidad; i++) {
                float nota = 0, peso = 0;
                color(11); cout << "  [ Sub-nota " << (i + 1) << " ]" << endl;
                
                color(7);  cout << "    > Peso (%): ";
                cin >> peso;
                if(cin.fail()){ cin.clear(); cin.ignore(10000, '\n'); }
                cout << "    > Nota (o -1 si falta): ";
                cin >> nota;
                if(cin.fail()){ cin.clear(); cin.ignore(10000, '\n'); }
                cout << endl;

                if (nota != -1) {
                    acumuladoInterno += nota * (peso / 100.0);
                    porcentajeInternoCompletado += peso;
                }
            }
        } else {
            // Nota unica
            float nota = -1;
            cout << endl;
            color(14); cout << "Ingrese la NOTA FINAL de " << nombre << " (o -1 si falta): ";
            color(15); cin >> nota;
            if(cin.fail()){ cin.clear(); cin.ignore(10000, '\n'); }
            cout << endl;

            if (nota != -1) {
                acumuladoInterno = nota; 
                porcentajeInternoCompletado = 100;
            }
        }
        
        linea();
        color(10); // Verde
        cout << "   --> Informacion guardada temporalmente." << endl; 
        color(7);
        cout << "   " << char(168) << "Es correcta? (s/n): ";
        cin >> confirmacion;
        cin.ignore(10000, '\n'); // Limpiar exceso

    } while (confirmacion != 's' && confirmacion != 'S');

    float puntosGlobales = acumuladoInterno * pesoEnPromedio;
    float avanceGlobalPorcentual = (porcentajeInternoCompletado / 100.0) * (pesoEnPromedio * 100.0);

    return {puntosGlobales, avanceGlobalPorcentual};
}

void calcularCurso() {
    encabezado("CALCULADORA DE CURSO");
    
    string nombreCurso;
    color(10); // Verde
    cout << "Hola! Empecemos." << endl;
    color(7);
    cout << "Ingrese nombre del curso: ";
    
    // Limpiamos el buffer antes de getline (ya que venimos de un cin >> opcion)
    cin.ignore(10000, '\n'); 
    getline(cin, nombreCurso);

    // Pesos temporales o quemados
    float pesoUnidad1 = 0.20;
    float pesoParcial = 0.25;
    float pesoUnidad2 = 0.20;
    float pesoFinal = 0.35;
    const float NOTA_MINIMA = 10.5;

    float sumaGlobalPuntos = 0;
    float sumaGlobalPorcentaje = 0;

    // Procesamos componentes
    ResultadoComponente u1 = procesarComponente("Unidad 1", pesoUnidad1, true);
    sumaGlobalPuntos += u1.puntosGlobalesGanados;
    sumaGlobalPorcentaje += u1.porcentajeGlobalAvanzado;

    ResultadoComponente par = procesarComponente("Examen Parcial", pesoParcial, false);
    sumaGlobalPuntos += par.puntosGlobalesGanados;
    sumaGlobalPorcentaje += par.porcentajeGlobalAvanzado;

    ResultadoComponente u2 = procesarComponente("Unidad 2", pesoUnidad2, true);
    sumaGlobalPuntos += u2.puntosGlobalesGanados;
    sumaGlobalPorcentaje += u2.porcentajeGlobalAvanzado;

    ResultadoComponente fin = procesarComponente("Examen Final", pesoFinal, false);
    sumaGlobalPuntos += fin.puntosGlobalesGanados;
    sumaGlobalPorcentaje += fin.porcentajeGlobalAvanzado;

    // --- REPORTE FINAL ---
    encabezado("RESUMEN: " + nombreCurso);
    
    cout << fixed << setprecision(2); // Fijar decimales para la tabla

    // Barra de progreso visual
    cout << "Progreso del curso: [";
    int barras = (int)(sumaGlobalPorcentaje / 5); // 20 barras max
    color(sumaGlobalPorcentaje == 100 ? 10 : 14); // Verde si completo, Amarillo si no
    for(int i=0; i<20; i++) {
        if(i < barras) cout << "#";
        else cout << ".";
    }
    color(7);
    cout << "] " << sumaGlobalPorcentaje << "%" << endl;
    cout << endl;
    
    linea();
    color(11); cout << " Nota Acumulada (Puntos firmes): " << sumaGlobalPuntos << " / 20" << endl; color(7);
    linea();

    string mensajeFinal = ""; // Para el MessageBox
    string tituloFinal = "";
    int icono = 0;

    if (sumaGlobalPorcentaje < 100) {
        float porcentajeRestante = 100 - sumaGlobalPorcentaje;
        float factorRestante = porcentajeRestante / 100.0;
        
        cout << "\n   --> Te falta completar el " << porcentajeRestante << "% del curso." << endl;
        
        color(15);
        cout << "\n   METAS PARA APROBAR (En lo que falta):" << endl;
        linea(40);
        cout << "   Meta (Promedio) | Necesitas sacar:" << endl;
        linea(40);

        vector<float> metas = {10.5, 12, 14, 15, 16, 17, 18, 19, 20};

        bool posibleAprobar = false;

        for (float meta : metas) {
            float puntosFaltantes = meta - sumaGlobalPuntos;
            
            cout << "        " << setw(4) << meta << "       |   ";

            if (puntosFaltantes <= 0) {
                color(10); cout << "YA LA TIENES!" << endl; color(7);
                posibleAprobar = true;
            } else {
                float necesaria = puntosFaltantes / factorRestante;
                if (necesaria > 20) {
                   color(12); cout << "IMPOSIBLE (>20)" << endl; color(7);
                } else {
                   color(14); cout << necesaria << endl; color(7);
                   posibleAprobar = true;
                }
            }
        }
        cout << endl;

        // Preparamos mensaje para ventana emergente
        tituloFinal = "Analisis de " + nombreCurso;
        icono = MB_ICONWARNING;
        mensajeFinal = "Llevas " + to_string(sumaGlobalPuntos) + " puntos acumulados.\n";
        if(posibleAprobar) mensajeFinal += "Aun puedes aprobar. Revisa la consola para ver cuanto necesitas sacar.";
        else mensajeFinal += "Matematicamente ya es muy dificil aprobar. A luchar hasta el final!";

    } else {
        cout << "\n";
        linea();
        if (sumaGlobalPuntos >= NOTA_MINIMA) {
             color(10); 
             cout << "             APROBADO FELICIDADES :D" << endl;
             cout << "             Nota Final: " << sumaGlobalPuntos << endl;
             mensajeFinal = "Felicidades! Aprobaste el curso con " + to_string(sumaGlobalPuntos);
             tituloFinal = "APROBADO";
             icono = MB_ICONINFORMATION;
        } else {
             color(12); 
             cout << "             REPROBADO :c" << endl;
             cout << "             Nota Final: " << sumaGlobalPuntos << endl;
             mensajeFinal = "Lo siento, terminaste el curso con " + to_string(sumaGlobalPuntos) + ". A intentarlo de nuevo!";
             tituloFinal = "REPROBADO";
             icono = MB_ICONERROR;
        }
        color(7);
        linea();
    }

    // --- VENTANA EMERGENTE (Pop-up) ---
    cout << endl;
    color(14); cout << "Que deseas hacer ahora?" << endl;
    color(11); cout << "  1. "; color(7); cout << "Volver al menu principal" << endl;
    color(11); cout << "  2. "; color(7); cout << "Salir del programa" << endl;
    linea();
    color(14); cout << "  Elige una opcion (1-2): ";
    cout << flush;
    
    string msgCopia = mensajeFinal;
    string titCopia = tituloFinal;
    int iconoCopia = icono;
    std::thread([msgCopia, titCopia, iconoCopia]() {
        MessageBoxA(NULL, msgCopia.c_str(), titCopia.c_str(), MB_OK | iconoCopia | MB_TOPMOST);
    }).detach();

    int eleccion = 0;
    while (true) {
        color(15); cin >> eleccion;
        if(cin.fail()){ cin.clear(); cin.ignore(10000, '\n'); eleccion = 0; }
        else { cin.ignore(10000, '\n'); }

        if (eleccion == 1) {
            return;
        } else if (eleccion == 2) {
            color(10); cout << endl << "Gracias por usar la calculadora! Saliendo..." << endl; color(7);
            exit(0);
        } else {
            color(12); cout << "Opcion no valida. Intenta de nuevo: "; color(7);
        }
    }
}

void calcularPGA() {
    encabezado("CALCULADORA DE PGA (Promedio General Acumulado)");
    
    int cantidadCursos = 0;
    color(14); cout << "Cuantos cursos vas a ingresar? ";
    color(15); cin >> cantidadCursos;
    if(cin.fail()){ cin.clear(); cin.ignore(10000, '\n'); }
    cout << endl;

    if (cantidadCursos <= 0) {
        color(12); cout << "La cantidad debe ser mayor a 0." << endl; color(7);
        Sleep(1500);
        return;
    }

    float sumaProductos = 0;
    int sumaCreditos = 0;

    // Limpiamos el buffer si quedo algun salto de linea de 'cantidadCursos'
    cin.ignore(10000, '\n');

    linea();
    for (int i = 0; i < cantidadCursos; i++) {
        string nombre;
        float nota = 0;
        int creditos = 0;

        color(11); cout << "  [ Curso " << (i + 1) << " ]" << endl;
        color(7); cout << "    > Nombre del curso: "; 
        getline(cin, nombre);
        
        cout << "    > Nota Final: ";
        cin >> nota;
        if(cin.fail()){ cin.clear(); cin.ignore(10000, '\n'); }
        
        cout << "    > Creditos (peso): ";
        cin >> creditos;
        if(cin.fail()){ cin.clear(); cin.ignore(10000, '\n'); }

        // Como a la siguiente iteracion usamos getline, limpiamos el '\n' dejado por creditos
        cin.ignore(10000, '\n');
        
        cout << endl;

        sumaProductos += (nota * creditos);
        sumaCreditos += creditos;
    }

    if (sumaCreditos > 0) {
        float pga = sumaProductos / sumaCreditos;
        encabezado("RESULTADO DEL PGA");
        linea();
        color(10);
        cout << "   Tu Promedio General Acumulado (PGA) es: " << fixed << setprecision(2) << pga << endl;
        color(7);
        linea();
        
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Tu PGA es: %.2f", pga);

        cout << endl;
        color(14); cout << "Que deseas hacer ahora?" << endl;
        color(11); cout << "  1. "; color(7); cout << "Volver al menu principal" << endl;
        color(11); cout << "  2. "; color(7); cout << "Salir del programa" << endl;
        linea();
        color(14); cout << "  Elige una opcion (1-2): ";
        cout << flush;

        string resultadoMensaje = buffer;
        std::thread([resultadoMensaje]() {
            MessageBoxA(NULL, resultadoMensaje.c_str(), "Resultado PGA", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
        }).detach();
    } else {
        color(12); cout << "   No se ingresaron creditos validos." << endl; color(7);
        return;
    }
    
    int eleccion = 0;
    while (true) {
        color(15); cin >> eleccion;
        if(cin.fail()){ cin.clear(); cin.ignore(10000, '\n'); eleccion = 0; }
        else { cin.ignore(10000, '\n'); }

        if (eleccion == 1) {
            return;
        } else if (eleccion == 2) {
            color(10); cout << endl << "Gracias por usar la calculadora! Saliendo..." << endl; color(7);
            exit(0);
        } else {
            color(12); cout << "Opcion no valida. Intenta de nuevo: "; color(7);
        }
    }
}

int main() {
    // Configuracion de consola para aceptar caracteres especiales si es posible
    system("chcp 65001 > nul"); 
    
    int opcion = 0;
    
    do {
        encabezado("MENU PRINCIPAL");
        color(11); cout << "  1. "; color(7); cout << "Calcular como va un curso (Notas Parciales)" << endl;
        color(11); cout << "  2. "; color(7); cout << "Calcular Promedio General Acumulado (PGA)" << endl;
        color(11); cout << "  3. "; color(7); cout << "Salir" << endl;
        linea();
        
        color(14); cout << "  Elige una opcion (1-3): ";
        color(15); cin >> opcion;
        cout << endl;

        switch (opcion) {
            case 1:
                calcularCurso();
                break;
            case 2:
                calcularPGA();
                break;
            case 3:
                color(10); cout << "Gracias por usar la calculadora!" << endl; color(7);
                break;
            default:
                color(12); cout << "Opcion no valida." << endl; color(7);
                Sleep(1500);
                // Limpiar error stream por si meten un char
                cin.clear();
                cin.ignore(10000, '\n');
                break;
        }
        
    } while(opcion != 3);
    
    return 0;
}
