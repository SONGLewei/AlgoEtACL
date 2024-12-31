#pragma once

#include <string>
#include <iostream>
#include <type_traits>
#include "AElement.h"

/**
 * @brief GElement<T> : un élément générique du graphe, dérivé de AElement.
 *        Il stocke une valeur de type T (par ex. Ville, std::string, int, etc.)
 */
template <class T>
class GElement : public AElement {
public:
    T v; // Contenu de l'élément

    GElement(T val)
        : AElement(), v(val) {}

    /**
     * @brief valueToString : convertit la valeur v en std::string
     */
    std::string valueToString() const {
        // Si T est std::string, on le retourne directement
        if constexpr (std::is_same_v<T, std::string>) {
            return v;
        } else {
            // Autres cas：假设 T 提供了 operator std::string()
            // (例如 Ville)
            return static_cast<std::string>(v);
        }
    }

    /**
     * @brief Conversion en std::string
     */
    operator std::string() const {
        // AElement::operator std::string() 返回 "AElement: clef"
        // valueToString() 返回 "Ville( ... )" 或 autre
        return "GElement: " + AElement::operator std::string() 
               + ", Value: " + valueToString();
    }

    /**
     * @brief Redéfinition de l'opérateur << pour affichage
     */
    friend std::ostream& operator<<(std::ostream& os, const GElement& element) {
        return os << static_cast<std::string>(element);
    }
};
