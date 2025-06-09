#ifndef VENTA_H
#define VENTA_H

#include <string>
#include <iomanip>
using namespace std;

class Venta {
private:
    string idVenta, fecha, pais, ciudad, cliente, producto, categoria;
    int cantidad;
    float precioUnitario, montoTotal;
    string medioEnvio, estadoEnvio;

public:
    Venta(string idVenta, string fecha, string pais, string ciudad,
          string cliente, string producto, string categoria,
          int cantidad, float precioUnitario, float montoTotal,
          string medioEnvio, string estadoEnvio)
        : idVenta(idVenta), fecha(fecha), pais(pais), ciudad(ciudad),
          cliente(cliente), producto(producto), categoria(categoria),
          cantidad(cantidad), precioUnitario(precioUnitario),
          montoTotal(montoTotal), medioEnvio(medioEnvio),
          estadoEnvio(estadoEnvio) {}

    string getIdVenta() const { return idVenta; }
    string getFecha() const { return fecha; }
    string getPais() const { return pais; }
    string getCiudad() const { return ciudad; }
    string getCliente() const { return cliente; }
    string getProducto() const { return producto; }
    int getCantidad() const { return cantidad; }
    float getPrecioUnitario() const { return precioUnitario; }
    float getMontoTotal() const { return montoTotal; }
    string getCategoria() const { return categoria; }
    string getMedioEnvio() const { return medioEnvio; }
    string getEstadoEnvio() const { return estadoEnvio; }


    void setIdVenta(string id) { idVenta = id; }
    void setFecha(string f) { fecha = f; }
    void setPais(string p) { pais = p; }
    void setCiudad(string c) { ciudad = c; }
    void setCliente(string c) { cliente = c; }
    void setProducto(string p) { producto = p; }
    void setCategoria(string c) { categoria = c; }
    void setCantidad(int c) { cantidad = c; }
    void setPrecioUnitario(float p) { precioUnitario = p; }
    void setMontoTotal(float m) { montoTotal = m; }
    void setMedioEnvio(string m) { medioEnvio = m; }
    void setEstadoEnvio(string e) { estadoEnvio = e; }
};


#endif
