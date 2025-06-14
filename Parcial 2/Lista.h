
#ifndef U02_LISTAS_LISTA_LISTA_H_
#define U02_LISTAS_LISTA_LISTA_H_
#include "nodo.h"
#include <iostream>

/**
 * Clase que implementa una Lista Enlasada generica, ya que puede
 * almacenar cualquier tipo de dato T
 * @tparam T cualquier tipo de dato
 */
template <class T> class Lista {
private:
  Nodo<T> *inicio;

public:
  Lista();

  Lista(const Lista<T> &li);

  ~Lista();

  bool esVacia();

  int getTamanio();

  void insertar(int pos, T dato);

  void insertarPrimero(T dato);

  void insertarUltimo(T dato);

  void remover(int pos);


  T& getDato(int pos);

  const T& getDato(int pos) const;

  void reemplazar(int pos, T dato);

  void vaciar();

  void print();
};

/**
 * Constructor de la clase Lista
 * @tparam T
 */
template <class T> Lista<T>::Lista() { inicio = nullptr; }

/**
 * Constructor por copia de la clase Lista
 * @tparam T
 * @param li
 */
template <class T> Lista<T>::Lista(const Lista<T> &li) {

  inicio = li.inicio;
}

/**
 * Destructor de la clase Lista, se encarga de liberar la memoria de todos los
 * nodos utilizados en la lista
 * @tparam T
 */
template <class T> Lista<T>::~Lista() { vaciar(); }

/**
 * Metodo para saber si la lista esta vacia
 * @tparam T
 * @return true si la lista esta vacia, sino false
 */
template <class T> bool Lista<T>::esVacia() { return inicio == nullptr; }

/**
 * Metodo para obtener la cantidad de nodos de la lista
 * @tparam T
 * @return la cantidad de nodos de la lista
 */
template <class T> int Lista<T>::getTamanio() {
  Nodo<T> *aux = inicio;
  int size = 0;

  while (aux != nullptr) {
    aux = aux->getSiguiente();
    size++;
  }

  return size;
}

/**
 * Inserta un nodo con el dato en la posicion pos
 * @tparam T
 * @param pos lugar donde ser� insertado el dato
 * @param dato  dato a insertar
 */
template <class T> void Lista<T>::insertar(int pos, T dato) {
  int posActual = 0;
  Nodo<T> *aux = inicio, *nuevo;
  nuevo = new Nodo<T>;
  nuevo->setDato(dato);

  if (pos == 0) {
    nuevo->setSiguiente(inicio);
    inicio = nuevo;
    return;
  }

  if (pos < 0 || (pos > getTamanio() && inicio != nullptr)) {
      delete nuevo;
      throw std::out_of_range("Lista::insertar() - Posicion fuera de rango");
  }


  while (aux != nullptr && posActual < pos - 1) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) {
      delete nuevo;
      throw std::runtime_error("Error inesperado en Lista::insertar()");
  }

  nuevo->setSiguiente(aux->getSiguiente());
  aux->setSiguiente(nuevo);
}

/**
 * Inserta un nodo con el dato en la primera posicion
 * @tparam T
 * @param dato dato a insertar
 */
template <class T> void Lista<T>::insertarPrimero(T dato) { insertar(0, dato); }

/**
 * Inserta un nodo con el dato en la ultima posicion
 * @tparam T
 * @param dato dato a insertar
 */
template <class T> void Lista<T>::insertarUltimo(T dato) {
  Nodo<T> *aux = inicio, *nuevo;
  nuevo = new Nodo<T>;
  nuevo->setDato(dato);

  if (aux == nullptr) {
    nuevo->setSiguiente(nullptr);
    inicio = nuevo;
    return;
  }

  while (aux->getSiguiente() != nullptr) {
    aux = aux->getSiguiente();
  }

  nuevo->setSiguiente(aux->getSiguiente());
  aux->setSiguiente(nuevo);
}

/**
 * Elimina el nodo en la posicion 'pos' de la lista enlasada, reenlazando los nodos
 * adecuadamente.
 * @tparam T
 * @param pos posicion del nodo a eliminar
 */
template <class T> void Lista<T>::remover(int pos) {
  Nodo<T> *aux = inicio, *aBorrar;
  int posActual = 0;

  if (inicio == nullptr) {
      throw std::out_of_range("Lista::remover() - La lista esta vacia");
  }
  if (pos < 0 || pos >= getTamanio()) {
      throw std::out_of_range("Lista::remover() - Posicion fuera de rango");
  }


  if (pos == 0) {
    inicio = inicio->getSiguiente();
    delete aux;
    return;
  }

  while (aux != nullptr && posActual < pos - 1) {
    aux = aux->getSiguiente();
    posActual++;
  }

  aBorrar = aux->getSiguiente();
  aux->setSiguiente(aBorrar->getSiguiente());

  delete aBorrar;
}

/**
 * Obtener el dato del nodo en la posicion pos
 * @tparam T
 * @param pos posicion del dato
 * @return dato almacenado en el nodo (by reference)
 */
template <class T>
T& Lista<T>::getDato(int pos) {
  Nodo<T> *aux = inicio;
  int posActual = 0;

  if (pos < 0 || pos >= getTamanio()) {
      throw std::out_of_range("Lista::getDato() - Posicion fuera de rango");
  }

  while (aux != nullptr && posActual < pos) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) {
    throw std::runtime_error("Error logico en Lista::getDato() - Nodo no encontrado");
  }

  return aux->getDato();
}

/**
 * Obtener el dato del nodo en la posicion pos (const version)
 * @tparam T
 * @param pos posicion del dato
 * @return dato almacenado en el nodo (const reference)
 */
template <class T>
const T& Lista<T>::getDato(int pos) const {
  Nodo<T> *aux = inicio;
  int posActual = 0;

  if (pos < 0 || pos >= getTamanio()) {
      throw std::out_of_range("Lista::getDato() - Posicion fuera de rango (const)");
  }

  while (aux != nullptr && posActual < pos) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) {
    throw std::runtime_error("Error logico en Lista::getDato() const - Nodo no encontrado");
  }

  return aux->getDato();
}


/**
 * Reemplaza el dato almacenado en un nodo por este otro
 * @tparam T
 * @param pos posicion donde se desea reemplazar
 * @param dato nuevo dato a almacenar
 */
template <class T> void Lista<T>::reemplazar(int pos, T dato) {
  Nodo<T> *aux = inicio;
  int posActual = 0;

  if (pos < 0 || pos >= getTamanio()) {
      throw std::out_of_range("Lista::reemplazar() - Posicion fuera de rango");
  }

  while (aux != nullptr && posActual < pos) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) {
    throw std::runtime_error("Error logico en Lista::reemplazar() - Nodo no encontrado");
  }

  aux->setDato( dato );
}

/**
 * Funci�n que vacia la lista enlazada
 * @tparam T
 */
template <class T> void Lista<T>::vaciar() {
  Nodo<T> *aux = inicio, *aBorrar;

  while( aux != nullptr ){
   aBorrar = aux;
   aux = aux->getSiguiente();
   delete aBorrar;
  }

  inicio = nullptr;
}

template<class T>
void Lista<T>::print() {
    Nodo<T> *aux = inicio;

    while (aux != nullptr) {
        std::cout << aux->getDato() << "->";
        aux = aux->getSiguiente();
    }
    std::cout << "NULL" << std::endl;
}

#endif // U02_LISTAS_LISTA_LISTA_H_
