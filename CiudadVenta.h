#ifndef CIUDAD_VENTA_H
#define CIUDAD_VENTA_H

#include <string>

class CiudadVenta {
private:
    std::string pais;
    std::string ciudad;
    float montoTotal;

public:
    CiudadVenta(std::string pais, std::string ciudad, float monto)
        : pais(pais), ciudad(ciudad), montoTotal(monto) {}

    std::string getPais() const { return pais; }
    std::string getCiudad() const { return ciudad; }
    float getMontoTotal() const { return montoTotal; }

    void acumular(float monto) {
        montoTotal += monto;
    }

    bool operator<(const CiudadVenta& other) const {
        return montoTotal < other.montoTotal;
    }
};

#endif // CIUDAD_VENTA_H
