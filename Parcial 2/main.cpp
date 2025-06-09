#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
#include "Venta.h"
#include "CiudadVenta.h"
#include "ProductoVenta.h"
#include "Lista.h"
#include "ArbolBinario.h"
#include "Pila.h"
#include "NodoArbol.h"
#include "menu.h"

using namespace std;

struct ProductoCantidad {
    string producto;
    int totalVendidas;
};

struct PromedioCatPais {
    string pais;
    string categoria;
    float suma;
    int cantidad;
};

struct MedioPais {
    string pais;
    string medio;
    int contador;
};

struct MedioCategoria {
    string categoria;
    string medio;
    int contador;
};

struct FechaVenta {
    string fecha;
    float monto;
};

struct EstadoPais {
    string pais;
    string estado;
    int contador;
};

Lista<Venta*> listaVentas;
Lista<string> paisesUnicos;
Lista<CiudadVenta*> ventasPorCiudad;
Lista<ProductoVenta*> ventasPorProducto;
Lista<PromedioCatPais*> promCatPais;
Lista<MedioPais*> mediosPais;
Lista<MedioCategoria*> mediosCategoria;
Lista<FechaVenta*> ventasPorFecha;
Lista<EstadoPais*> estadosPorPais;
Lista<ProductoCantidad*> cantidadPorProducto;

// Funcion que recorre el arbol y muestra el Top 5 ciudades
void mostrarTop5DesdeArbol(NodoArbolBinario<CiudadVenta*>* nodo, int& contador) {
    if (nodo == nullptr || contador >= 5) return;
    mostrarTop5DesdeArbol(nodo->getHijoDerecho(), contador);
    if (contador < 5) {
        cout << contador + 1 << ". " << nodo->getDato()->getCiudad() << " - $" << nodo->getDato()->getMontoTotal() << endl;
        contador++;
    }
    mostrarTop5DesdeArbol(nodo->getHijoIzquierdo(), contador);
}

void clearAggregateData() {
    for (int i = 0; i < ventasPorCiudad.getTamanio(); ++i) {
        delete ventasPorCiudad.getDato(i);
    }
    ventasPorCiudad.vaciar();

    for (int i = 0; i < ventasPorProducto.getTamanio(); ++i) {
        delete ventasPorProducto.getDato(i);
    }
    ventasPorProducto.vaciar();

    for (int i = 0; i < promCatPais.getTamanio(); ++i) {
        delete promCatPais.getDato(i);
    }
    promCatPais.vaciar();

    for (int i = 0; i < mediosPais.getTamanio(); ++i) {
        delete mediosPais.getDato(i);
    }
    mediosPais.vaciar();

    for (int i = 0; i < mediosCategoria.getTamanio(); ++i) {
        delete mediosCategoria.getDato(i);
    }
    mediosCategoria.vaciar();

    for (int i = 0; i < ventasPorFecha.getTamanio(); ++i) {
        delete ventasPorFecha.getDato(i);
    }
    ventasPorFecha.vaciar();

    for (int i = 0; i < estadosPorPais.getTamanio(); ++i) {
        delete estadosPorPais.getDato(i);
    }
    estadosPorPais.vaciar();

    for (int i = 0; i < cantidadPorProducto.getTamanio(); ++i) {
        delete cantidadPorProducto.getDato(i);
    }
    cantidadPorProducto.vaciar();

    paisesUnicos.vaciar();
}

void processAndDisplayData() {
    clearAggregateData();

    for (int i = 0; i < listaVentas.getTamanio(); ++i) {
        string pais = listaVentas.getDato(i)->getPais();
        bool encontradoPais = false;
        for (int j = 0; j < paisesUnicos.getTamanio(); ++j) {
            if (paisesUnicos.getDato(j) == pais) {
                encontradoPais = true;
                break;
            }
        }
        if (!encontradoPais) {
            paisesUnicos.insertarUltimo(pais);
        }
    }

    for (int k = 0; k < listaVentas.getTamanio(); ++k) {
        Venta* venta = listaVentas.getDato(k);

        string pais = venta->getPais();
        string ciudad = venta->getCiudad();
        string producto = venta->getProducto();
        string categoria = venta->getCategoria();
        int cantidad = venta->getCantidad();
        float monto = venta->getMontoTotal();
        string medio = venta->getMedioEnvio();
        string fecha = venta->getFecha();
        string estado = venta->getEstadoEnvio();

        // 5.1 - Top 5 ciudades con mayor monto de ventas por país
        bool encontradaCiudadVenta = false;
        for (int i = 0; i < ventasPorCiudad.getTamanio(); i++) {
            CiudadVenta* cv = ventasPorCiudad.getDato(i);
            if (cv->getCiudad() == ciudad && cv->getPais() == pais) {
                cv->acumular(monto);
                encontradaCiudadVenta = true;
                break;
            }
        }
        if (!encontradaCiudadVenta) {
            ventasPorCiudad.insertarUltimo(new CiudadVenta(pais, ciudad, monto));
        }

        // 5.2 - Monto total vendido por producto, discriminado por país
        bool encontradoProdVenta = false;
        for (int i = 0; i < ventasPorProducto.getTamanio(); i++) {
            ProductoVenta* pv = ventasPorProducto.getDato(i);
            if (pv->getProducto() == producto && pv->getPais() == pais) {
                pv->acumular(monto);
                encontradoProdVenta = true;
                break;
            }
        }
        if (!encontradoProdVenta) {
            ventasPorProducto.insertarUltimo(new ProductoVenta(producto, pais, monto));
        }

        // 5.3 - Promedio de ventas por categoría y país
        bool pcEncontrado = false;
        for (int i = 0; i < promCatPais.getTamanio(); i++) {
            auto pcp = promCatPais.getDato(i);
            if (pcp->pais == pais && pcp->categoria == categoria) {
                pcp->suma += monto;
                pcp->cantidad++;
                pcEncontrado = true;
                break;
            }
        }
        if (!pcEncontrado) {
            promCatPais.insertarUltimo(new PromedioCatPais{pais, categoria, monto, 1});
        }

        // 5.4 - Medio de envío más usado por país
        bool mpEncontrado = false;
        for (int i = 0; i < mediosPais.getTamanio(); i++) {
            auto mp = mediosPais.getDato(i);
            if (mp->pais == pais && mp->medio == medio) {
                mp->contador++;
                mpEncontrado = true;
                break;
            }
        }
        if (!mpEncontrado) {
            mediosPais.insertarUltimo(new MedioPais{pais, medio, 1});
        }

        // 5.5 - Medio de envío más usado por categoría
        bool mcEncontrado = false;
        for (int i = 0; i < mediosCategoria.getTamanio(); i++) {
            auto mc = mediosCategoria.getDato(i);
            if (mc->categoria == categoria && mc->medio == medio) {
                mc->contador++;
                mcEncontrado = true;
                break;
            }
        }
        if (!mcEncontrado) {
            mediosCategoria.insertarUltimo(new MedioCategoria{categoria, medio, 1});
        }

        // 5.6 - Día con mayor venta
        bool fechaEncontrada = false;
        for (int i = 0; i < ventasPorFecha.getTamanio(); i++) {
            auto vf = ventasPorFecha.getDato(i);
            if (vf->fecha == fecha) {
                vf->monto += monto;
                fechaEncontrada = true;
                break;
            }
        }
        if (!fechaEncontrada) {
            ventasPorFecha.insertarUltimo(new FechaVenta{fecha, monto});
        }

        // 5.7 - Estado de envío más frecuente por país
        bool epEncontrado = false;
        for (int i = 0; i < estadosPorPais.getTamanio(); i++) {
            auto ep = estadosPorPais.getDato(i);
            if (ep->pais == pais && ep->estado == estado) {
                ep->contador++;
                epEncontrado = true;
                break;
            }
        }
        if (!epEncontrado) {
            estadosPorPais.insertarUltimo(new EstadoPais{pais, estado, 1});
        }

        // 5.8 y 5.9 - Producto más y menos vendido en unidades
        bool prodEncontrado = false;
        for (int i = 0; i < cantidadPorProducto.getTamanio(); i++) {
            auto p = cantidadPorProducto.getDato(i);
            if (p->producto == producto) {
                p->totalVendidas += cantidad;
                prodEncontrado = true;
                break;
            }
        }
        if (!prodEncontrado) {
            cantidadPorProducto.insertarUltimo(new ProductoCantidad{producto, cantidad});
        }
    }


    cout << "\n================== RESULTADOS DEL PROCESAMIENTO DE DATOS ==================\n";

    // 5.1 - top 5 de ciudades con mayor monto de ventas por país.
    cout << "\n Top 5 Ciudades con Mayor Monto de Ventas por Pais\n";
    for (int p = 0; p < paisesUnicos.getTamanio(); p++) {
        string pais = paisesUnicos.getDato(p);
        cout << "\n Pais: " << pais << endl;
        ArbolBinario<CiudadVenta*> arbol;
        for (int i = 0; i < ventasPorCiudad.getTamanio(); i++) {
            CiudadVenta* cv = ventasPorCiudad.getDato(i);
            if (cv->getPais() == pais) arbol.put(cv);
        }
        int contador = 0;
        mostrarTop5DesdeArbol(arbol.getRaiz(), contador);
    }

    // 5.2 - Monto total vendido por producto, discriminado por país
    cout << "\n Monto Total Vendido por Producto (por Pais)\n";
    for (int i = 0; i < ventasPorProducto.getTamanio(); i++) {
        ProductoVenta* pv = ventasPorProducto.getDato(i);
        cout << "Producto: " << pv->getProducto() << " | Pais: " << pv->getPais() << " | Monto: $" << fixed << setprecision(2) << pv->getMontoTotal() << endl;
    }

    // 5.3 - Promedio de ventas por categoría y país
    cout << "\n Promedio de Ventas por Categoria y Pais\n";
    for (int i = 0; i < promCatPais.getTamanio(); i++) {
        auto pcp = promCatPais.getDato(i);
        float promedio = (pcp->cantidad > 0) ? (pcp->suma / pcp->cantidad) : 0.0f; // Prevent division by zero
        cout << "Pais: " << pcp->pais << " | Categoria: " << pcp->categoria << " | Promedio: $" << fixed << setprecision(2) << promedio << endl;
    }

    // 5.4 - Medio de envío más usado por país
    cout << "\n Medio de Envio mas utilizado por Pais\n";
    for (int i = 0; i < paisesUnicos.getTamanio(); i++) {
        string pais = paisesUnicos.getDato(i);
        string medioMasUsado = "N/A";
        int maxUsos = 0;
        for (int j = 0; j < mediosPais.getTamanio(); j++) {
            auto mp = mediosPais.getDato(j);
            if (mp->pais == pais && mp->contador > maxUsos) {
                maxUsos = mp->contador;
                medioMasUsado = mp->medio;
            }
        }
        cout << "Pais: " << pais << " | Medio mas usado: " << medioMasUsado << " (" << maxUsos << " envios)" << endl;
    }

    // 5.5 - Medio de envío más usado por categoría
    cout << "\n Medio de Envio mas Utilizado por Categoria\n";
    Lista<string> categoriasProcesadas;
    for (int i = 0; i < mediosCategoria.getTamanio(); i++) {
        string cat = mediosCategoria.getDato(i)->categoria;
        bool yaMostrado = false;
        for (int j = 0; j < categoriasProcesadas.getTamanio(); j++) {
            if (categoriasProcesadas.getDato(j) == cat) {
                yaMostrado = true;
                break;
            }
        }
        if (yaMostrado) continue;

        categoriasProcesadas.insertarUltimo(cat);
        string medioMax = "N/A";
        int max = 0;
        for (int j = 0; j < mediosCategoria.getTamanio(); j++) {
            auto mc = mediosCategoria.getDato(j);
            if (mc->categoria == cat && mc->contador > max) {
                max = mc->contador;
                medioMax = mc->medio;
            }
        }
        cout << "Categoria: " << cat << " | Medio mas usado: " << medioMax << " (" << max << " envios)" << endl;
    }

    // 5.6 - Dia con mayor venta
    cout << "\n Dia con Mayor Monto Total de Ventas\n";
    string mejorDia = "N/A";
    float montoMax = 0;
    if (ventasPorFecha.getTamanio() > 0) { // Handle case where list is empty
        mejorDia = ventasPorFecha.getDato(0)->fecha;
        montoMax = ventasPorFecha.getDato(0)->monto;
    }
    for (int i = 0; i < ventasPorFecha.getTamanio(); i++) {
        auto vf = ventasPorFecha.getDato(i);
        if (vf->monto > montoMax) {
            montoMax = vf->monto;
            mejorDia = vf->fecha;
        }
    }
    cout << "Dia con mayor monto total de ventas: " << mejorDia << " ($" << fixed << setprecision(2) << montoMax << ")" << endl;

    // 5.7 - Estado de envío más frecuente por país
    cout << "\n Estado de Envio mas Frecuente por Pais\n";
    for (int i = 0; i < paisesUnicos.getTamanio(); i++) {
        string pais = paisesUnicos.getDato(i);
        string estadoFrecuente = "N/A";
        int max = 0;
        for (int j = 0; j < estadosPorPais.getTamanio(); j++) {
            auto ep = estadosPorPais.getDato(j);
            if (ep->pais == pais && ep->contador > max) {
                max = ep->contador;
                estadoFrecuente = ep->estado;
            }
        }
        cout << "Pais: " << pais << " | Estado mas frecuente: " << estadoFrecuente << " (" << max << " veces)" << endl;
    }

    // 5.8 y 5.9 - Producto más y menos vendido en unidades usando pila
    cout << "\n Producto mas y menos vendido en cantidad total (en unidades)\n";

    if (cantidadPorProducto.esVacia()) {
        cout << "No hay productos vendidos para analizar." << endl;
    } else {
        Lista<ProductoCantidad*> ordenados;
        for (int i = 0; i < cantidadPorProducto.getTamanio(); i++) {
            ordenados.insertarUltimo(cantidadPorProducto.getDato(i));
        }

        for (int i = 0; i < ordenados.getTamanio() - 1; i++) {
            for (int j = 0; j < ordenados.getTamanio() - i - 1; j++) {
                if (ordenados.getDato(j)->totalVendidas < ordenados.getDato(j + 1)->totalVendidas) {
                    ProductoCantidad* aux = ordenados.getDato(j);
                    ordenados.reemplazar(j, ordenados.getDato(j + 1));
                    ordenados.reemplazar(j + 1, aux);
                }
            }
        }
        Pila<ProductoCantidad*> pilaProductos;
        for (int i = 0; i < ordenados.getTamanio(); i++) {
            pilaProductos.push(ordenados.getDato(i));
        }
        ProductoCantidad* masVendido = pilaProductos.topeValor();
        ProductoCantidad* menosVendido = NULL;
        while (!pilaProductos.esVacia()) menosVendido = pilaProductos.pop();

        cout << "Producto mas vendido: " << masVendido->producto << " (" << masVendido->totalVendidas << " unidades)" << endl;
        cout << "Producto menos vendido: " << menosVendido->producto << " (" << menosVendido->totalVendidas << " unidades)" << endl;
    }
}


tuple<int, int, int> parseDate(const string& dateStr) {
    int day, month, year;
    sscanf(dateStr.c_str(), "%d/%d/%d", &day, &month, &year);
    return make_tuple(day, month, year);
}

bool isDateLessEqual(const string& date1Str, const string& date2Str) {
    auto [d1, m1, y1] = parseDate(date1Str);
    auto [d2, m2, y2] = parseDate(date2Str);

    if (y1 != y2) return y1 < y2;
    if (m1 != m2) return m1 < m2;
    return d1 <= d2;
}


// 7.1 - Listado de ventas realizadas en una ciudad específica
void queryVentasPorCiudad() {
    cout << "\n--- VENTAS EN CIUDAD ESPECIFICA ---\n";
    cout << "Ingrese la ciudad: ";
    string ciudadBuscar;
    getline(cin, ciudadBuscar);

    cout << "\n Listado de ventas en " << ciudadBuscar << endl;
    int count = 0;
    for (int i = 0; i < listaVentas.getTamanio(); ++i) {
        Venta* v = listaVentas.getDato(i);
        if (v->getCiudad() == ciudadBuscar) {
            cout << "ID: " << v->getIdVenta() << ", Fecha: " << v->getFecha()
                 << ", Pais: " << v->getPais() << ", Producto: " << v->getProducto()
                 << ", Monto: $" << fixed << setprecision(2) << v->getMontoTotal() << endl;
            count++;
        }
    }
    if (count == 0) {
        cout << "No se encontraron ventas para la ciudad: " << ciudadBuscar << endl;
    }
}

// 7.2 - Listado de ventas realizadas en un rango de fechas por país
void queryVentasPorRangoFechaPais() {
    cout << "\n--- VENTAS EN RANGO DE FECHAS POR PAIS ---\n";
    cout << "Ingrese el pais: ";
    string paisBuscar;
    getline(cin, paisBuscar);
    cout << "Ingrese la fecha de inicio (DD/MM/YYYY): ";
    string fechaInicio;
    getline(cin, fechaInicio);
    cout << "Ingrese la fecha de fin (DD/MM/YYYY): ";
    string fechaFin;
    getline(cin, fechaFin);

    cout << "\n Listado de ventas por rango de fechas en " << paisBuscar << endl;
    int count = 0;
    for (int i = 0; i < listaVentas.getTamanio(); ++i) {
        Venta* v = listaVentas.getDato(i);
        if (v->getPais() == paisBuscar &&
            isDateLessEqual(fechaInicio, v->getFecha()) &&
            isDateLessEqual(v->getFecha(), fechaFin)) {
            cout << "ID: " << v->getIdVenta() << ", Fecha: " << v->getFecha()
                 << ", Ciudad: " << v->getCiudad() << ", Producto: " << v->getProducto()
                 << ", Monto: $" << fixed << setprecision(2) << v->getMontoTotal() << endl;
            count++;
        }
    }
    if (count == 0) {
        cout << "No se encontraron ventas para " << paisBuscar
             << " en el rango " << fechaInicio << " a " << fechaFin << endl;
    }
}

// 7.3 - Comparación entre dos países
void queryComparacionPaises() {
    cout << "\n--- COMPARACION ENTRE DOS PAISES ---\n";
    cout << "Ingrese el primer pais: ";
    string pais1;
    getline(cin, pais1);
    cout << "Ingrese el segundo pais: ";
    string pais2;
    getline(cin, pais2);

    cout << "\n Comparacion entre " << pais1 << " y " << pais2 << endl;

    float monto1 = 0.0f;
    float monto2 = 0.0f;
    Lista<string> productosVendidos1;
    Lista<string> productosVendidos2;
    Lista<pair<string, int>> mediosEnvio1;
    Lista<pair<string, int>> mediosEnvio2;


    for (int i = 0; i < listaVentas.getTamanio(); ++i) {
        Venta* v = listaVentas.getDato(i);
        if (v->getPais() == pais1) {
            monto1 += v->getMontoTotal();
            productosVendidos1.insertarUltimo(v->getProducto());

            bool foundMedio = false;
            for (int j = 0; j < mediosEnvio1.getTamanio(); ++j) {
                if (mediosEnvio1.getDato(j).first == v->getMedioEnvio()) {
                    mediosEnvio1.getDato(j).second++;
                    foundMedio = true;
                    break;
                }
            }
            if (!foundMedio) mediosEnvio1.insertarUltimo({v->getMedioEnvio(), 1});

        } else if (v->getPais() == pais2) {
            monto2 += v->getMontoTotal();
            productosVendidos2.insertarUltimo(v->getProducto());

            bool foundMedio = false;
            for (int j = 0; j < mediosEnvio2.getTamanio(); ++j) {
                if (mediosEnvio2.getDato(j).first == v->getMedioEnvio()) {
                    mediosEnvio2.getDato(j).second++;
                    foundMedio = true;
                    break;
                }
            }
            if (!foundMedio) mediosEnvio2.insertarUltimo({v->getMedioEnvio(), 1});
        }
    }

    // a. Monto total de ventas
    cout << "\n--- Monto Total de Ventas ---\n";
    cout << pais1 << ": $" << fixed << setprecision(2) << monto1 << endl;
    cout << pais2 << ": $" << fixed << setprecision(2) << monto2 << endl;

    // b. Productos más vendidos (simple count for now)
    cout << "\n--- Productos Vendidos ---\n";
    cout << pais1 << " productos: ";
    if (productosVendidos1.esVacia()) cout << "N/A";
    else {
        for (int i = 0; i < productosVendidos1.getTamanio(); ++i) {
            cout << productosVendidos1.getDato(i) << ((i == productosVendidos1.getTamanio() - 1) ? "" : ", ");
        }
    }
    cout << endl;
    cout << pais2 << " productos: ";
    if (productosVendidos2.esVacia()) cout << "N/A";
    else {
        for (int i = 0; i < productosVendidos2.getTamanio(); ++i) {
            cout << productosVendidos2.getDato(i) << ((i == productosVendidos2.getTamanio() - 1) ? "" : ", ");
        }
    }
    cout << endl;

    // c. Medio de envío más usado
    cout << "\n--- Medio de Envio mas Usado ---\n";
    string medio1_max = "N/A";
    int count1_max = 0;
    for (int i = 0; i < mediosEnvio1.getTamanio(); ++i) {
        if (mediosEnvio1.getDato(i).second > count1_max) {
            count1_max = mediosEnvio1.getDato(i).second;
            medio1_max = mediosEnvio1.getDato(i).first;
        }
    }
    cout << pais1 << ": " << medio1_max << " (" << count1_max << " envios)" << endl;

    string medio2_max = "N/A";
    int count2_max = 0;
    for (int i = 0; i < mediosEnvio2.getTamanio(); ++i) {
        if (mediosEnvio2.getDato(i).second > count2_max) {
            count2_max = mediosEnvio2.getDato(i).second;
            medio2_max = mediosEnvio2.getDato(i).first;
        }
    }
    cout << pais2 << ": " << medio2_max << " (" << count2_max << " envios)" << endl;
}


// 7.4 - Comparación entre dos productos discriminado por todos los paises
void queryComparacionProductosPorPais() {
    cout << "\n--- COMPARACION ENTRE DOS PRODUCTOS POR PAIS ---\n";
    cout << "Ingrese el primer producto: ";
    string prod1;
    getline(cin, prod1);
    cout << "Ingrese el segundo producto: ";
    string prod2;
    getline(cin, prod2);

    cout << "\n Comparacion de productos por pais\n";

    struct ProdCompData {
        string pais;
        int cantidad1 = 0;
        float monto1 = 0.0f;
        int cantidad2 = 0;
        float monto2 = 0.0f;
    };
    Lista<ProdCompData*> compDataList;

    for (int i = 0; i < listaVentas.getTamanio(); ++i) {
        Venta* v = listaVentas.getDato(i);
        bool found = false;
        for (int j = 0; j < compDataList.getTamanio(); ++j) {
            ProdCompData* data = compDataList.getDato(j);
            if (data->pais == v->getPais()) {
                if (v->getProducto() == prod1) {
                    data->cantidad1 += v->getCantidad();
                    data->monto1 += v->getMontoTotal();
                } else if (v->getProducto() == prod2) {
                    data->cantidad2 += v->getCantidad();
                    data->monto2 += v->getMontoTotal();
                }
                found = true;
                break;
            }
        }
        if (!found) {
            ProdCompData* newData = new ProdCompData();
            newData->pais = v->getPais();
            if (v->getProducto() == prod1) {
                newData->cantidad1 += v->getCantidad();
                newData->monto1 += v->getMontoTotal();
            } else if (v->getProducto() == prod2) {
                newData->cantidad2 += v->getCantidad();
                newData->monto2 += v->getMontoTotal();
            }
            compDataList.insertarUltimo(newData);
        }
    }

    if (compDataList.esVacia()) {
        cout << "No se encontraron ventas para los productos especificados." << endl;
    } else {
        for (int i = 0; i < compDataList.getTamanio(); ++i) {
            ProdCompData* data = compDataList.getDato(i);
            cout << "\n--- Pais: " << data->pais << " ---\n";
            cout << "  " << prod1 << ": Cantidad = " << data->cantidad1
                 << " unidades, Monto = $" << fixed << setprecision(2) << data->monto1 << endl;
            cout << "  " << prod2 << ": Cantidad = " << data->cantidad2
                 << " unidades, Monto = $" << fixed << setprecision(2) << data->monto2 << endl;
        }
    }

    for (int i = 0; i < compDataList.getTamanio(); ++i) {
        delete compDataList.getDato(i);
    }
    compDataList.vaciar();
}

// 7.5 & 7.6 - Buscar productos vendidos en promedio por debajo/encima de un monto total especificado por el usuario (umbral)
void queryProductosPorUmbralMontoPromedio(bool findBelowUmbral) {
    string criterio = findBelowUmbral ? "por debajo de" : "por encima de";
    cout << "\n--- BUSCAR PRODUCTOS " << (findBelowUmbral ? "CON MONTO PROMEDIO MENOR A" : "CON MONTO PROMEDIO MAYOR A") << " UN UMBRAL ---\n";
    cout << "Ingrese el monto umbral: $";
    float umbral;
    while (!(cin >> umbral)) {
        cout << "Entrada invalida. Ingrese un numero para el umbral: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Ingrese el pais (dejar vacio para todos los paises): ";
    string paisFiltro;
    getline(cin, paisFiltro);

    cout << "\nProceso: Busqueda de productos por umbral de monto promedio " << criterio << (paisFiltro.empty() ? "" : " por pais") << endl;
    cout << "Algoritmo: Agregacion de datos y filtro condicional\n";
    cout << "Estructura: Lista Enlazada (para agregacion)\n";

    struct ProdAvgData {
        string producto;
        string pais;
        float sumaMonto = 0.0f;
        int totalCantidad = 0;
    };
    Lista<ProdAvgData*> avgDataList;

    for (int i = 0; i < listaVentas.getTamanio(); ++i) {
        Venta* v = listaVentas.getDato(i);
        if (!paisFiltro.empty() && v->getPais() != paisFiltro) {
            continue;
        }

        bool found = false;
        for (int j = 0; j < avgDataList.getTamanio(); ++j) {
            ProdAvgData* data = avgDataList.getDato(j);
            if (data->producto == v->getProducto() && (paisFiltro.empty() || data->pais == v->getPais())) {
                data->sumaMonto += v->getMontoTotal();
                data->totalCantidad += v->getCantidad();
                found = true;
                break;
            }
        }
        if (!found) {
            ProdAvgData* newData = new ProdAvgData();
            newData->producto = v->getProducto();
            newData->pais = v->getPais();
            newData->sumaMonto = v->getMontoTotal();
            newData->totalCantidad = v->getCantidad();
            avgDataList.insertarUltimo(newData);
        }
    }

    Lista<string> matchingProducts;
    for (int i = 0; i < avgDataList.getTamanio(); ++i) {
        ProdAvgData* data = avgDataList.getDato(i);
        if (data->totalCantidad > 0) {
            float promedioUnidad = data->sumaMonto / data->totalCantidad;
            if ((findBelowUmbral && promedioUnidad < umbral) || (!findBelowUmbral && promedioUnidad > umbral)) {
                matchingProducts.insertarUltimo(data->producto + " (Pais: " + data->pais + ", Promedio: $" + to_string(promedioUnidad) + ")");
            }
        }
    }

    if (matchingProducts.esVacia()) {
        cout << "No se encontraron productos " << criterio << " $" << umbral << (paisFiltro.empty() ? " en ningun pais." : " en " + paisFiltro + ".") << endl;
    } else {
        cout << "\nProductos con monto promedio " << criterio << " $" << umbral << (paisFiltro.empty() ? "" : " en " + paisFiltro) << ": ";
        for (int i = 0; i < matchingProducts.getTamanio(); ++i) {
            cout << matchingProducts.getDato(i) << ((i == matchingProducts.getTamanio() - 1) ? "" : "; ");
        }
        cout << "." << endl;
    }

    for (int i = 0; i < avgDataList.getTamanio(); ++i) {
        delete avgDataList.getDato(i);
    }
    avgDataList.vaciar();
}


void handleDynamicQueries() {
    int opcionConsulta;
    string inputStr;
    do {
        mostrarMenuConsultas();
        cout << "Ingrese su opcion: ";
        cin >> opcionConsulta;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcionConsulta) {
            case 1: queryVentasPorCiudad(); break;
            case 2: queryVentasPorRangoFechaPais(); break;
            case 3: queryComparacionPaises(); break;
            case 4: queryComparacionProductosPorPais(); break;
            case 5: queryProductosPorUmbralMontoPromedio(true); break;
            case 6: queryProductosPorUmbralMontoPromedio(false); break;
            case 7: cout << "Volviendo al menu principal.\n"; break;
            default: cout << "Opcion invalida. Por favor, intente de nuevo.\n"; break;
        }
        if (opcionConsulta != 7) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (opcionConsulta != 7);
}


int main() {
    ifstream archivo("./ventas_sudamerica.csv");
    string linea;

    if (!archivo) {
        cerr << "El archivo no existe o no se pudo abrir." << endl;
        return 1;
    }

    char bom[3];
    archivo.read(bom, 3);
    if (!(bom[0] == char(0xEF) && bom[1] == char(0xBB) && bom[2] == char(0xBF))) {
        archivo.seekg(0);
    }

    getline(archivo, linea);

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string dato;
        vector<string> fila;

        while (getline(ss, dato, ',')) fila.push_back(dato);

        if (fila.size() != 12) continue;

        try {
            string id = fila[0];
            string fecha = fila[1];
            string pais = fila[2];
            string ciudad = fila[3];
            string cliente = fila[4];
            string producto = fila[5];
            string categoria = fila[6];
            int cantidad = stoi(fila[7]);
            float precio = stof(fila[8]);
            float monto = stof(fila[9]);
            string medio = fila[10];
            string estado = fila[11];

            Venta* venta = new Venta(
                id, fecha, pais, ciudad, cliente, producto,
                categoria, cantidad, precio, monto, medio, estado
            );
            listaVentas.insertarUltimo(venta);
        } catch (const std::invalid_argument& e) {
            cerr << "Error de conversion en linea: " << linea << " - " << e.what() << endl;
        } catch (const std::out_of_range& e) {
            cerr << "Valor fuera de rango en linea: " << linea << " - " << e.what() << endl;
        }
    }
    archivo.close();

    processAndDisplayData();

    int opcion;
    string inputStr;

    do {
        mostrarMenu();
        cout << "Ingrese su opcion: ";
        cin >> opcion;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1: {
                cout << "\n--- AGREGAR NUEVA VENTA ---\n";
                string id, fecha, pais, ciudad, cliente, producto, categoria, medio, estado;
                int cantidad;
                float precio, monto;

                cout << "ID de Venta: "; getline(cin, id);
                cout << "Fecha (DD/MM/YYYY): "; getline(cin, fecha);
                cout << "Pais: "; getline(cin, pais);
                cout << "Ciudad: "; getline(cin, ciudad);
                cout << "Cliente: "; getline(cin, cliente);
                cout << "Producto: "; getline(cin, producto);
                cout << "Categoria: "; getline(cin, categoria);

                cout << "Cantidad: ";
                while (!(cin >> cantidad)) {
                    cout << "Entrada invalida. Ingrese un numero para Cantidad: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Precio Unitario: ";
                while (!(cin >> precio)) {
                    cout << "Entrada invalida. Ingrese un numero para Precio Unitario: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                monto = cantidad * precio;

                cout << "Medio de Envio: "; getline(cin, medio);
                cout << "Estado de Envio: "; getline(cin, estado);

                Venta* nuevaVenta = new Venta(
                    id, fecha, pais, ciudad, cliente, producto, categoria,
                    cantidad, precio, monto, medio, estado
                );
                listaVentas.insertarUltimo(nuevaVenta);
                cout << "\nVenta agregada exitosamente. Reprocesando datos...\n";
                processAndDisplayData();
                break;
            }
            case 2: {
                cout << "\n--- ELIMINAR VENTA ---\n";
                cout << "Desea eliminar por Pais o por Ciudad? (Ingrese 'Pais' o 'Ciudad'): ";
                string filtroTipo;
                getline(cin, filtroTipo);

                string valorFiltro;
                Lista<int> indicesAEliminar;

                if (filtroTipo == "Pais" || filtroTipo == "pais") {
                    cout << "Ingrese el Pais a filtrar: ";
                    getline(cin, valorFiltro);
                    cout << "\n--- Ventas en " << valorFiltro << " ---\n";
                    int count = 0;
                    for (int i = 0; i < listaVentas.getTamanio(); ++i) {
                        Venta* v = listaVentas.getDato(i);
                        if (v->getPais() == valorFiltro) {
                            cout << i << ". ID: " << v->getIdVenta() << ", Producto: " << v->getProducto() << ", Monto: $" << v->getMontoTotal() << endl;
                            indicesAEliminar.insertarUltimo(i);
                            count++;
                        }
                    }
                    if (count == 0) {
                        cout << "No se encontraron ventas para el pais: " << valorFiltro << endl;
                        break;
                    }
                } else if (filtroTipo == "Ciudad" || filtroTipo == "ciudad") {
                    cout << "Ingrese la Ciudad a filtrar: ";
                    getline(cin, valorFiltro);
                    cout << "\n--- Ventas en " << valorFiltro << " ---\n";
                    int count = 0;
                    for (int i = 0; i < listaVentas.getTamanio(); ++i) {
                        Venta* v = listaVentas.getDato(i);
                        if (v->getCiudad() == valorFiltro) {
                            cout << i << ". ID: " << v->getIdVenta() << ", Producto: " << v->getProducto() << ", Monto: $" << v->getMontoTotal() << endl;
                            indicesAEliminar.insertarUltimo(i);
                            count++;
                        }
                    }
                    if (count == 0) {
                        cout << "No se encontraron ventas para la ciudad: " << valorFiltro << endl;
                        break;
                    }
                } else {
                    cout << "Tipo de filtro invalido. Volviendo al menu." << endl;
                    break;
                }

                if (indicesAEliminar.esVacia()) {
                    cout << "No hay ventas para eliminar con ese filtro." << endl;
                } else {
                    cout << "Ingrese el numero de indice de la venta a eliminar (o -1 para cancelar): ";
                    int idxEliminar;
                    cin >> idxEliminar;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (idxEliminar >= 0 && idxEliminar < listaVentas.getTamanio()) {
                        delete listaVentas.getDato(idxEliminar);
                        listaVentas.remover(idxEliminar);
                        cout << "Venta eliminada exitosamente. Reprocesando datos...\n";
                        processAndDisplayData();
                    } else if (idxEliminar == -1) {
                        cout << "Eliminacion cancelada." << endl;
                    } else {
                        cout << "Indice invalido." << endl;
                    }
                }
                break;
            }
            case 3: {
                cout << "\n--- MODIFICAR VENTA ---\n";
                string idModificar;
                cout << "Ingrese el ID de la venta a modificar: ";
                getline(cin, idModificar);

                Venta* ventaAModificar = nullptr;
                int posVenta = -1;
                for (int i = 0; i < listaVentas.getTamanio(); ++i) {
                    if (listaVentas.getDato(i)->getIdVenta() == idModificar) {
                        ventaAModificar = listaVentas.getDato(i);
                        posVenta = i;
                        break;
                    }
                }

                if (ventaAModificar == nullptr) {
                    cout << "Venta con ID " << idModificar << " no encontrada." << endl;
                    break;
                }

                cout << "Venta encontrada. Que campo desea modificar?" << endl;
                cout << "1. Fecha (" << ventaAModificar->getFecha() << ")\n";
                cout << "2. Pais (" << ventaAModificar->getPais() << ")\n";
                cout << "3. Ciudad (" << ventaAModificar->getCiudad() << ")\n";
                cout << "4. Cliente (" << ventaAModificar->getCliente() << ")\n";
                cout << "5. Producto (" << ventaAModificar->getProducto() << ")\n";
                cout << "6. Categoria (" << ventaAModificar->getCategoria() << ")\n";
                cout << "7. Cantidad (" << ventaAModificar->getCantidad() << ")\n";
                cout << "8. Precio Unitario (" << ventaAModificar->getPrecioUnitario() << ")\n";
                cout << "9. Medio de Envio (" << ventaAModificar->getMedioEnvio() << ")\n";
                cout << "10. Estado de Envio (" << ventaAModificar->getEstadoEnvio() << ")\n";
                cout << "Ingrese el numero del campo a modificar: ";
                int campoOpcion;
                cin >> campoOpcion;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch (campoOpcion) {
                    case 1: { cout << "Nueva Fecha: "; string val; getline(cin, val); ventaAModificar->setFecha(val); break; }
                    case 2: { cout << "Nuevo Pais: "; string val; getline(cin, val); ventaAModificar->setPais(val); break; }
                    case 3: { cout << "Nueva Ciudad: "; string val; getline(cin, val); ventaAModificar->setCiudad(val); break; }
                    case 4: { cout << "Nuevo Cliente: "; string val; getline(cin, val); ventaAModificar->setCliente(val); break; }
                    case 5: { cout << "Nuevo Producto: "; string val; getline(cin, val); ventaAModificar->setProducto(val); break; }
                    case 6: { cout << "Nueva Categoria: "; string val; getline(cin, val); ventaAModificar->setCategoria(val); break; }
                    case 7: {
                        cout << "Nueva Cantidad: ";
                        int val;
                        while (!(cin >> val)) {
                            cout << "Entrada invalida. Ingrese un numero para Cantidad: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        ventaAModificar->setCantidad(val);
                        ventaAModificar->setMontoTotal(ventaAModificar->getCantidad() * ventaAModificar->getPrecioUnitario());
                        break;
                    }
                    case 8: {
                        cout << "Nuevo Precio Unitario: ";
                        float val;
                        while (!(cin >> val)) {
                            cout << "Entrada invalida. Ingrese un numero para Precio Unitario: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        ventaAModificar->setPrecioUnitario(val);
                        ventaAModificar->setMontoTotal(ventaAModificar->getCantidad() * ventaAModificar->getPrecioUnitario());
                        break;
                    }
                    case 9: { cout << "Nuevo Medio de Envio: "; string val; getline(cin, val); ventaAModificar->setMedioEnvio(val); break; }
                    case 10: { cout << "Nuevo Estado de Envio: "; string val; getline(cin, val); ventaAModificar->setEstadoEnvio(val); break; }
                    default:
                        cout << "Opcion de campo invalida." << endl;
                        break;
                }
                cout << "\nVenta modificada exitosamente. Reprocesando datos...\n";
                processAndDisplayData();
                break;
            }
            case 4: {
                cout << "\nRecalculando y mostrando estadisticas...\n";
                processAndDisplayData();
                break;
            }
            case 5: {
                handleDynamicQueries();
                break;
            }
            case 6: {
                cout << "Terminando programa." << endl;
                break;
            }
            default: {
                cout << "Opcion invalida. Por favor, intente de nuevo." << endl;
                break;
            }
        }
        cout << "\nPresione Enter para continuar...";
        cin.get();
        if (opcion != 6) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (opcion != 6);


    for (int i = 0; i < listaVentas.getTamanio(); ++i) {
        delete listaVentas.getDato(i);
    }
    listaVentas.vaciar();

    clearAggregateData();

    return 0;
}
