#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <set>
#include <stack>
#include <unordered_map>
#include "../includeGraphe/GrapheAll.h"
#include "../includeAlgo/Kmeans.h"



// Function to find a vertex in the graph by its key
// 这个是通过clef来找sommet的valuer的值。其实写在graph.h里也可以，但gpt就加在这里了直接用了
Sommet<Ville>* getSommetParClef(const Graphe<double, Ville>& graphe, int clef) {
    for (auto it = graphe.lSommets; it; it = it->suivant) {
        if (it->valeur->clef == clef) {
            return it->valeur;
        }
    }
    return nullptr; // 如果没有找到对应的顶点，返回空指针。
}



// Helper function to construct an Eulerian circuit
// 用于将欧拉回路选取的所有边绘制成欧拉回路，用于后边的第五步
void constructEulerianCircuit(const Graphe<double, Ville>& graphe, int start, std::vector<int>& circuit, std::set<std::pair<int, int>>& edges) {
    std::stack<int> stack;
    std::unordered_map<int, std::vector<int>> adjList;

    // 构建邻接表
    for (const auto& edge : edges) {
        adjList[edge.first].push_back(edge.second);
        adjList[edge.second].push_back(edge.first);
    }

    stack.push(start);

    while (!stack.empty()) {
        int current = stack.top();

        if (!adjList[current].empty()) {
            int next = adjList[current].back();
            adjList[current].pop_back();
            //adjList[next].erase(std::find(adjList[next].begin(), adjList[next].end(), current));
            auto it2 = std::find(adjList[next].begin(), adjList[next].end(), current);
            if (it2 != adjList[next].end()) {
                adjList[next].erase(it2);
            }
            stack.push(next);
        } else {
            circuit.push_back(current);
            stack.pop();
        }
    }
}

// Helper function to construct a Hamiltonian circuit from an Eulerian circuit
// 把欧拉回路传化成密哈顿回路，其实就是顶点查重，用于后边的第五步
void constructHamiltonianCircuit(const std::vector<int>& eulerCircuit, std::vector<int>& hamiltonianCircuit) {
    std::unordered_set<int> visited;

    for (int vertex : eulerCircuit) {
        if (visited.find(vertex) == visited.end()) {
            hamiltonianCircuit.push_back(vertex);
            visited.insert(vertex);
        }
    }
}

// Function to find the minimum spanning tree (MST) for a given cluster
// 对其中的一个簇进行计算，返回的是最小生成树的权重
// 寻找结果就是保证给每个点都都选择与他相连的所有边中权重最小的，但选边有两个限制条件
// 1.不能选连接已经遍历过的点
// 2.连接后不能出现环
double calculateMST(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster) {
    double totalWeight = 0.0;
    //if (cluster.size() < 2) return 0.0;
    std::vector<bool> visited(cluster.size(), false);//用于记录每个顶点有没有被遍历
    std::vector<double> minEdge(cluster.size(), std::numeric_limits<double>::max());//用于记录每个顶点所连接的最小权重的边
    std::set<std::pair<int, int>> selectedEdges; // 存储已选中的边避免在下一个函数中的第三步算奇数顶点边的连接时重复一条边两次

    minEdge[0] = 0.0; // 从第一个节点开始。

    for (size_t i = 0; i < cluster.size(); i++) {
        int u = -1;

        // 找未遍历点中，连接权重最小的点。
        for (size_t j = 0; j < cluster.size(); j++) {
            if (!visited[j] && (u == -1 || minEdge[j] < minEdge[u])) {
                u = j;
            }
        }

        visited[u] = true;
        totalWeight += minEdge[u];// 找到后加到总和里

        // 更新与当前点连接的其他点的最小边权重。
        // 其实就是为了保证不出现环，不让往回连
       for (size_t v = 0; v < cluster.size(); v++) {
            if (!visited[v]) {
                double weight = distanceCityToCity(graphe, 
                    getSommetParClef(graphe, cluster[u]), 
                    getSommetParClef(graphe, cluster[v]));//算边长
                if (weight < minEdge[v]) {
                    minEdge[v] = weight;
                    selectedEdges.insert({std::min(cluster[u], cluster[v]), std::max(cluster[u], cluster[v])});//记录已选择的边
                }
            }
        }
    }

    return totalWeight;
}

// Function to apply Christofides algorithm on a given cluster
// 用 Christofides 算法为一个簇解决旅行商问题（TSP）。
double calculateChristofides(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster) {

    /*if (cluster.size() < 2) {
        return 0.0;
    }*/
    std::set<std::pair<int,int>> mstEdges;//////////////
    // Step 1: Calculate the MST
    // 第一步：计算最小生成树（MST）的权重。就是上边那个方法
    double mstWeight = calculateMST(graphe, cluster);

    // Step 2: Find odd-degree vertices in MST
    // 第二步：找出 MST 中度数为奇数的顶点。
    std::vector<int> oddVertices;//用于存储所有奇数边的顶点

    // 用Sommet类里的int degre看它是不是奇数顶点
    for (int id : cluster) {
        Sommet<Ville>* sommet = getSommetParClef(graphe, id);
        if (sommet && sommet->degre % 2 != 0) {
            oddVertices.push_back(id);
        }
    }


    // Step 3: Perform minimum weight perfect matching on odd vertices
    // 第三步：对奇度顶点进行最小权完美匹配。
    double matchingWeight = 0.0;
    std::set<std::pair<int, int>> matchedEdges; // 提取在第一步中MST树里选择过的边，防止计算两次

    while (true) {
        int u = -1, v = -1;
        double minWeight = std::numeric_limits<double>::max();

        // 找到当前未匹配顶点中权重最小的边。
        for (size_t i = 0; i < oddVertices.size(); i++) {
            for (size_t j = i + 1; j < oddVertices.size(); j++) {
                std::pair<int, int> edge = {std::min(oddVertices[i], oddVertices[j]), std::max(oddVertices[i], oddVertices[j])};
                if (matchedEdges.find(edge) != matchedEdges.end()) continue;//如果这条边之前没选择过就继续，也就是计算他的权重并添加到总和里

                double weight = distanceCityToCity(graphe, 
                    getSommetParClef(graphe, oddVertices[i]), 
                    getSommetParClef(graphe, oddVertices[j]));//算边长

                if (weight < minWeight) {
                    minWeight = weight;
                    u = i;
                    v = j;
                }
            }
        }

        if (u == -1 || v == -1) break;

        matchedEdges.insert({std::min(oddVertices[u], oddVertices[v]), std::max(oddVertices[u], oddVertices[v])});//记录选择过的边
        matchingWeight += minWeight;
    }

    // Step 4: Combine MST and matching to form an Eulerian graph
    // 第四步：将 MST 和匹配边合并，形成欧拉图。把两个集合的选取的边加起来
    double eulerianGraphWeight = mstWeight + matchingWeight;

    // Step 5: Construct Eulerian and Hamiltonian circuits
    // 第五步：构建密哈顿回路并计算结果
    std::vector<int> eulerCircuit;
        if (cluster.empty()) {
        // 可能直接返回 0 或者返回一个特殊值
        return 0.0;
        }
        std::set<std::pair<int,int>> eulerEdges = mstEdges;
            for (auto &me : matchedEdges) {
            eulerEdges.insert(me);
        }   
    constructEulerianCircuit(graphe, cluster[0], eulerCircuit, /*matchedEdges*/eulerEdges);

    std::vector<int> hamiltonianCircuit;
    constructHamiltonianCircuit(eulerCircuit, hamiltonianCircuit);

    // Calculate Hamiltonian circuit weight
    //通过欧拉回路的结果转化成密哈顿回路计算最终结果
    double hamiltonianWeight = 0.0;
    for (size_t i = 1; i < hamiltonianCircuit.size(); i++) {
        hamiltonianWeight += distanceCityToCity(graphe, 
            getSommetParClef(graphe, hamiltonianCircuit[i - 1]), 
            getSommetParClef(graphe, hamiltonianCircuit[i]));
    }

    return hamiltonianWeight;
}

std::pair<double, std::vector<int>> calculateChristofidesWithPath(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster) {
    std::set<std::pair<int, int>> mstEdges;
    double mstWeight = calculateMST(graphe, cluster);

    std::vector<int> oddVertices;
    for (int id : cluster) {
        Sommet<Ville>* sommet = getSommetParClef(graphe, id);
        if (sommet && sommet->degre % 2 != 0) {
            oddVertices.push_back(id);
        }
    }

    double matchingWeight = 0.0;
    std::set<std::pair<int, int>> matchedEdges;
    while (true) {
        int u = -1, v = -1;
        double minWeight = std::numeric_limits<double>::max();

        for (size_t i = 0; i < oddVertices.size(); i++) {
            for (size_t j = i + 1; j < oddVertices.size(); j++) {
                std::pair<int, int> edge = {std::min(oddVertices[i], oddVertices[j]), std::max(oddVertices[i], oddVertices[j])};
                if (matchedEdges.find(edge) != matchedEdges.end()) continue;

                double weight = distanceCityToCity(graphe, 
                    getSommetParClef(graphe, oddVertices[i]), 
                    getSommetParClef(graphe, oddVertices[j]));
                if (weight < minWeight) {
                    minWeight = weight;
                    u = i;
                    v = j;
                }
            }
        }

        if (u == -1 || v == -1) break;

        matchedEdges.insert({std::min(oddVertices[u], oddVertices[v]), std::max(oddVertices[u], oddVertices[v])});
        matchingWeight += minWeight;
    }

    std::set<std::pair<int, int>> eulerEdges = mstEdges;
    for (auto &me : matchedEdges) {
        eulerEdges.insert(me);
    }

    std::vector<int> eulerCircuit;
    if (!cluster.empty()) {
        constructEulerianCircuit(graphe, cluster[0], eulerCircuit, eulerEdges);
    }

    std::vector<int> hamiltonianCircuit;
    constructHamiltonianCircuit(eulerCircuit, hamiltonianCircuit);

    double hamiltonianWeight = 0.0;
    for (size_t i = 1; i < hamiltonianCircuit.size(); i++) {
        hamiltonianWeight += distanceCityToCity(graphe, 
            getSommetParClef(graphe, hamiltonianCircuit[i - 1]), 
            getSommetParClef(graphe, hamiltonianCircuit[i]));
    }

    return {hamiltonianWeight, hamiltonianCircuit};
}

std::vector<std::pair<double, std::vector<int>>> getAllClusterRoutes(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters) {
    std::vector<std::pair<double, std::vector<int>>> allRoutes;

    for (size_t i = 0; i < clusters.size(); i++) {
        auto [cMax, path] = calculateChristofidesWithPath(graphe, clusters[i]);
        allRoutes.push_back({cMax, path});

        // 打印每个簇的 Cmax 和路径
        std::cout << "Cluster " << i << " Cmax: " << cMax << "\nPath: ";
        for (int vertex : path) {
            Sommet<Ville>* sommet = getSommetParClef(graphe, vertex);
            if (sommet) {
                std::cout << sommet->v.name << " ";
            }
        }
        std::cout << std::endl;
    }

    return allRoutes;
}



std::pair<double, std::vector<int>> getMaxCmaxWithPath(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters) {
    double maxCmax = 0.0;
    std::vector<int> cmaxPath;

    for (size_t i = 0; i < clusters.size(); i++) {
        auto [cMax, path] = calculateChristofidesWithPath(graphe, clusters[i]);
        std::cout << "Cluster " << i << " Cmax: " << cMax << std::endl;

        if (cMax > maxCmax) {
            maxCmax = cMax;
            cmaxPath = path; // 更新对应路径
        }
    }

    return {maxCmax, cmaxPath};
}


// Function to calculate and return the maximum Cmax among all clusters
// 计算所有簇的Cmax进行比较并返回最大的 Cmax。
double getMaxCmax(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters) {
    double maxCmax = 0.0;
    for (size_t i = 0; i < clusters.size(); i++) {
        double cMax = calculateChristofides(graphe, clusters[i]);
        std::cout << "Cluster " << i << " Cmax: " << cMax << std::endl;
        if (cMax > maxCmax) {
            maxCmax = cMax;
        }
    }
    return maxCmax;
}


std::vector<std::string> getTruckRouteForCluster(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster) {
    std::vector<int> eulerCircuit;
    std::vector<int> hamiltonianCircuit;

    if (cluster.empty()) {
        return {}; // 如果簇为空，返回空路线
    }

    // Step 1: 计算最小生成树（MST）并找到所有奇度顶点
    calculateMST(graphe, cluster);

    std::vector<int> oddVertices;
    for (int id : cluster) {
        Sommet<Ville>* sommet = getSommetParClef(graphe, id);
        if (sommet && sommet->degre % 2 != 0) {
            oddVertices.push_back(id);
        }
    }

    // Step 2: 对奇度顶点进行最小权匹配，形成欧拉图
    std::set<std::pair<int, int>> mstEdges;
    std::set<std::pair<int, int>> matchedEdges;

    // 执行最小权匹配，计算 matchedEdges
    for (size_t i = 0; i < oddVertices.size(); i++) {
        for (size_t j = i + 1; j < oddVertices.size(); j++) {
            matchedEdges.insert({std::min(oddVertices[i], oddVertices[j]), std::max(oddVertices[i], oddVertices[j])});
        }
    }

    // 合并 MST 和匹配的边，形成欧拉图
    std::set<std::pair<int, int>> eulerEdges = mstEdges;
    for (auto& edge : matchedEdges) {
        eulerEdges.insert(edge);
    }

    // Step 3: 构造欧拉回路
    constructEulerianCircuit(graphe, cluster[0], eulerCircuit, eulerEdges);

    // Step 4: 构造哈密顿回路（去重）
    constructHamiltonianCircuit(eulerCircuit, hamiltonianCircuit);

    // Step 5: 将哈密顿回路的 ID 转换为城市名字
    std::vector<std::string> truckRoute;
    for (int id : hamiltonianCircuit) {
        Sommet<Ville>* sommet = getSommetParClef(graphe, id);
        if (sommet) {
            truckRoute.push_back(sommet->v.name); // 获取城市名字
        }
    }

    return truckRoute; // 返回城市名字列表，表示卡车的路线
}


std::vector<std::vector<std::string>> getAllTruckRoutes(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters) {
    std::vector<std::vector<std::string>> allRoutes;

    for (const auto& cluster : clusters) {
        std::vector<std::string> route = getTruckRouteForCluster(graphe, cluster);
        allRoutes.push_back(route);

        // 打印每个簇的路线
        std::cout << "Truck Route for Cluster: ";
        for (const auto& cityName : route) {
            std::cout << cityName << " ";
        }
        std::cout << std::endl;
    }

    return allRoutes;
}



