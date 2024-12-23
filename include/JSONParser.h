#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include "Graphe.h"
#include "Ville.h"

using namespace std;

class JSONParser {
  public:
    Graphe<Ville, string> parse(const string& filename) {
      ifstream file(filename);
      stringstream buffer;
      buffer << file.rdbuf();
      string content = buffer.str();

      Graphe<Ville, string> graphe;
      parseJSON(content, graphe);
      return graphe;
    }

  private:
    void parseJSON(const string& content, Graphe<Ville, string>& graphe) {
      size_t pos = 0;
      while ((pos = content.find("{", pos)) != string::npos) {
        size_t endPos = content.find("}", pos);
        if (endPos == string::npos) break;

        string villeData = content.substr(pos, endPos - pos + 1);
        Ville ville = parseVille(villeData);
        graphe.creerSommet(ville);

        pos = endPos + 1;
      }
    }

    Ville parseVille(const string& villeData) {
        string nom = extractValue(villeData, "ville");
        double latitude = stod(extractValue(villeData, "latitude"));
        double longitude = stod(extractValue(villeData, "longitude"));
        return Ville(nom, latitude, longitude);
    }

    string extractValue(const string& data, const string& key) {
        size_t keyPos = data.find("\"" + key + "\"");
        size_t colonPos = data.find(":", keyPos);
        size_t commaPos = data.find(",", colonPos);
        size_t endPos = data.find("}", colonPos);

        size_t valuePos = colonPos + 1;
        size_t valueEnd = (commaPos < endPos) ? commaPos : endPos;

        string value = data.substr(valuePos, valueEnd - valuePos);
        value.erase(remove(value.begin(), value.end(), '\"'), value.end());
        value.erase(remove(value.begin(), value.end(), ' '), value.end());
        return value;
    }
};