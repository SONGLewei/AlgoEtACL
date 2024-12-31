#pragma once
#include <string>
#include <sstream>

/**
 * @brief Ville : représente une ville avec un nom et des coordonnées (latitude, longitude).
 */
class Ville {
public:
    std::string name;
    double latitude;
    double longitude;

    Ville() 
        : name(""), latitude(0.0), longitude(0.0) 
    {}

    Ville(const std::string& n, double lat, double lon)
        : name(n), latitude(lat), longitude(lon) 
    {}

    // 在类内部实现 operator std::string()，可自定义输出格式
    operator std::string() const {
        std::ostringstream oss;
        oss << "Ville(" << name 
            << ", lat=" << latitude 
            << ", lon=" << longitude << ")";
        return oss.str();
    }
};
