#ifndef NODOARBOL_H_
#define NODOARBOL_H_

template <class T>
class NodoArbolBinario {
private:
    T dato;
    NodoArbolBinario<T>* hijoIzquierdo;
    NodoArbolBinario<T>* hijoDerecho;

public:
    NodoArbolBinario(T d) {
        dato = d;
        hijoIzquierdo = nullptr;
        hijoDerecho = nullptr;
    }

    T getDato() {
        return dato;
    }

    void setDato(T d) {
        dato = d;
    }

    NodoArbolBinario<T>* getHijoIzquierdo() {
        return hijoIzquierdo;
    }

    NodoArbolBinario<T>* getHijoDerecho() {
        return hijoDerecho;
    }

    void setHijoIzquierdo(NodoArbolBinario<T>* hijo) {
        hijoIzquierdo = hijo;
    }

    void setHijoDerecho(NodoArbolBinario<T>* hijo) {
        hijoDerecho = hijo;
    }

    NodoArbolBinario<T>*& getIzquierdoRef() {
        return hijoIzquierdo;
    }

    NodoArbolBinario<T>*& getDerechoRef() {
        return hijoDerecho;
    }
};

#endif // NODOARBOL_H_

