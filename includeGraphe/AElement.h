#pragma once
#include <string>
#include <iostream>

/**
 * @brief AElement : classe de base pour gérer un identifiant unique (clef).
 */
class AElement {
private:
    // 静态成员变量的声明（不在这里初始化）
    static int nextKey;

public:
    int clef; // Identifiant unique

    AElement()
        : clef(nextKey++) {
    }

    // 转为字符串时，返回类似 "AElement: 3"
    operator std::string() const {
        return "AElement: " + std::to_string(clef);
    }

    friend std::ostream& operator<<(std::ostream& os, const AElement& element) {
        return os << static_cast<std::string>(element);
    }
};
