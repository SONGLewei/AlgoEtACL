#pragma once

#include <string>
#include "../json.hpp"
#include <fstream>
#include "Graphe.h"
#include "Ville.h"

class GrapheExporter {
public:
    /**
     * 导出图结构为 JSON 文件。
     * @param graphe   : 要导出的图
     * @param filename : 导出的 JSON 文件路径
     */
    static void exporter(const Graphe<double, Ville>& graphe, const std::string& filename);
};