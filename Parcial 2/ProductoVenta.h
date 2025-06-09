#ifndef PRODUCTO_VENTA_H
#define PRODUCTO_VENTA_H

#include <string>

class ProductoVenta {
private:
    std::string producto;
    std::string pais;
    float montoTotal;

public:
    ProductoVenta(std::string producto, std::string pais, float monto)
        : producto(producto), pais(pais), montoTotal(monto) {}

    std::string getProducto() const { return producto; }
    std::string getPais() const { return pais; }
    float getMontoTotal() const { return montoTotal; }

    void acumular(float monto) {
        montoTotal += monto;
    }
};

#endif // PRODUCTO_VENTA_H
