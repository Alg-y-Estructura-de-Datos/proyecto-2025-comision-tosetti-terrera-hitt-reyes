#include <iostream>

void mostrarMenu() {
    std::cout << "\n==================== MENU PRINCIPAL ====================\n";
    std::cout << "1. Agregar una nueva venta\n";
    std::cout << "2. Eliminar una venta\n";
    std::cout << "3. Modificar una venta\n";
    std::cout << "4. Ver Estadisticas (Reprocesar datos)\n";
    std::cout << "5. Consultas Dinamicas\n";
    std::cout << "6. Salir\n";
}

void mostrarMenuConsultas() {
    std::cout << "\n================= CONSULTAS DINAMICAS ================\n";
    std::cout << "1. Ventas realizadas en una ciudad especifica\n";
    std::cout << "2. Ventas realizadas en un rango de fechas por pais\n";
    std::cout << "3. Comparacion entre dos paises\n";
    std::cout << "4. Comparacion entre dos productos (por pais)\n";
    std::cout << "5. Buscar productos por debajo de un umbral de monto promedio (por pais)\n";
    std::cout << "6. Buscar productos por encima de un umbral de monto promedio (global)\n";
    std::cout << "7. Volver al Menu Principal\n";

}


