//
// Created by Patrick on 16/09/2024.
//

#ifndef SEMANA_5_HASH_TABLE_OPEN_ADDRESING_H
#define SEMANA_5_HASH_TABLE_OPEN_ADDRESING_H

//Este trabajará con el modelo de open addresing, donde se cambiará el valor de donde se encuentra el


#include <random>
#include <ctime>

template<typename K, typename V>
struct Par {
    K first;
    V second;

    Par() : first(K()), second(V()) {}

    Par(const K &llave, const V &valor) : first(llave), second(valor) {}
};

template<typename K, typename V>
class Hash_table_open_addresing {
    Par<K, V> *table;
    int size = 7;
    int a, b, p;
    int count = 0;
    double MaxFillFactor = 0.7;

    std::mt19937 rng;  // Generador de números aleatorios

    int hashFunction(const K& key) {
        if constexpr (std::is_same<K, std::string>::value) {
            const int pp = 73;  // Base
            int hashValue = 0;
            int p_pow = 1;

            for (char ch : key) {
                hashValue = (hashValue + (ch - 'a' + 1) * p_pow) % size;
                p_pow = (p_pow * pp) % size;
            }
            return hashValue;
        } else {
            // Si no es un std::string, usamos la fórmula hash genérica
            return ((a * key + b) % p) % size;
        }
    }

public:
    Hash_table_open_addresing() {
        table = new Par<K, V>[size];
        p = 10007;
        std::uniform_int_distribution<int> dist_a(1, 20);
        std::uniform_int_distribution<int> dist_b(0, 20);
        a = dist_a(rng);
        b = dist_b(rng);
    }

    void insert(const K &key, const V &value) {
        if (static_cast<double>(count) / size >= MaxFillFactor) {
            resize();
        }

        int index = hashFunction(key);  // Hash original

        //Mientras que el valor en el índice no sea vacío o sea distinto de key (para reemplazarlo), sigue bucle
        while ((table + index)->first != K() && (table + index)->first != key) {
            index = (index + 1) % size;  // Linear probing, moverse al siguiente índice
        }

        *(table + index) = Par<K, V>(key, value);  // Insertar el par
        count++;
    }


    V search(const K &key) const{
        int index = hashFunction(key);
        int originalIndex = index;

        while ((table + index)->first != key) {
            index = (index + 1) % size;

            if (index == originalIndex) {
                throw std::out_of_range("Clave no encontrada");
            }
        }
        return (table + index)->second;
    }

    V& search(const K &key) {
        int index = hashFunction(key);
        int originalIndex = index;

        while ((table + index)->first != key) {
            index = (index + 1) % size;

            if (index == originalIndex) {
                throw std::out_of_range("Clave no encontrada");
            }
        }
        return (table + index)->second;
    }

    void remove(const K &key) {
        int index = hashFunction(key);
        int originalIndex = index;

        while ((table + index)->first != key) {
            index = (index + 1) % size;

            if (index == originalIndex) {
                throw std::out_of_range("Clave no encontrada");
            }
        }

        // Marcar como borrado o eliminar el par
        (table + index)->first = Par<K, V>();  // Vaciar la celda
        count--;
    }


    void resize() {
        int newSize = size * 2;  // Tamaño nuevo
        auto *newTable = new Par<K, V>[newSize];

        // Reinsertar todos los elementos de la tabla original
        for (int i = 0; i < size; ++i) {
            if ((table+i)->first != K()) {
                int newIndex = hashFunction((table+i)->first) % newSize;
                (newTable+newIndex)->first = (table+i)->first;
                (newTable+newIndex)->second = (table+i)->second;
            }
        }

        delete[] table;  // Liberar la tabla vieja
        table = newTable;
        size = newSize;
    }

};

#endif //SEMANA_5_HASH_TABLE_OPEN_ADDRESING_H
