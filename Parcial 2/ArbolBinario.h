#ifndef U05_ARBOL_ARBOL_ARBOLBINARIO_H_
#define U05_ARBOL_ARBOL_ARBOLBINARIO_H_

#include "NodoArbol.h"

/**
 * Arbol Binario simple compatible con estructuras del proyecto
 */
template <class T>
class ArbolBinario {
private:
    NodoArbolBinario<T>* root;

public:
    ArbolBinario();

    bool esVacio();

    void put(T dato);

    NodoArbolBinario<T>* getRaiz();

    NodoArbolBinario<T>*& getRaizRef();

private:
    NodoArbolBinario<T>* insertarRecursivo(NodoArbolBinario<T>*& nodo, T dato);
};

/** Constructor */
template <class T>
ArbolBinario<T>::ArbolBinario() {
    root = nullptr;
}

/** Verifica si el arbol esta vacio */
template <class T>
bool ArbolBinario<T>::esVacio() {
    return root == nullptr;
}

/** Devuelve el nodo raiz */
template <class T>
NodoArbolBinario<T>* ArbolBinario<T>::getRaiz() {
    return root;
}

/** Referencia directa a la raiz */
template <class T>
NodoArbolBinario<T>*& ArbolBinario<T>::getRaizRef() {
    return root;
}

/** Inserta un dato de forma ordenada */
template <class T>
void ArbolBinario<T>::put(T dato) {
    insertarRecursivo(root, dato);
}

/** Metodo recursivo para insertar ordenadamente */
template <class T>
NodoArbolBinario<T>* ArbolBinario<T>::insertarRecursivo(NodoArbolBinario<T>*& nodo, T dato) {
    if (nodo == nullptr) {
        nodo = new NodoArbolBinario<T>(dato);
    } else {
        if (dato->getMontoTotal() <= nodo->getDato()->getMontoTotal()) {
            insertarRecursivo(nodo->getIzquierdoRef(), dato);
        } else {
            insertarRecursivo(nodo->getDerechoRef(), dato);
        }
    }
    return nodo;
}

#endif // U05_ARBOL_ARBOL_ARBOLBINARIO_H_


