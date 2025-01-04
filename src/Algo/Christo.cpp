
#include "../../include/Algo/Christo.h"

// Function to find a vertex in the graph by its key
// 这个是通过clef来找sommet的valuer的值。其实写在graph.h里也可以
Sommet<Ville>*getSommetParClef(const Graphe<double, Ville>& graphe, int clef) {
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
std::pair<double, std::set<std::pair<int,int>>> calculateMSTWithEdges(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster)
{
    double totalWeight = 0.0;
    std::set<std::pair<int,int>> selectedEdges; 
    
    // 如果簇内城市数太少，就返回空
    if (cluster.size() < 2) {
        return {0.0, selectedEdges};
    }

    // dist[v]  : cluster[v] 这个城市到“已选集合”的最短距离
    // parent[v]: cluster[v] 的父节点在 MST 中是哪一个 (下标)
    // visited[v] : cluster[v] 是否已经被选入 MST
    std::vector<double> dist(cluster.size(), std::numeric_limits<double>::infinity());
    std::vector<int> parent(cluster.size(), -1);
    std::vector<bool> visited(cluster.size(), false);

    // 从 cluster[0] 开始，令其到已选集合的距离为 0
    dist[0] = 0.0;

    // 重复选择 |cluster.size()| 次
    for (size_t i = 0; i < cluster.size(); i++) {
        // 第一步：在所有未访问的点里，找到 dist[] 值最小的那个
        int u = -1;
        for (size_t j = 0; j < cluster.size(); j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        // 如果找不到任何可用节点(不连通情形)，那就停止
        if (u == -1) break;

        // 把这个点 u 标记为已访问
        visited[u] = true;

        // 如果它的 parent[u] != -1，说明 u 与 parent[u] 之间的那条边就是 MST 新增的一条边，可以加入 selectedEdges 并累加权重
        if (parent[u] != -1) {
            // cluster[u]、cluster[parent[u]] 分别是城市的真实 ID
            int idx1 = cluster[u];
            int idx2 = cluster[parent[u]];
            // 统一一下 (小,大) 顺序，以避免重复
            if (idx1 > idx2) std::swap(idx1, idx2);
            selectedEdges.insert({idx1, idx2});

            // dist[u] 就是这条边的权重
            totalWeight += dist[u];
        }

        // 第二步：用当前选入的点 u，去更新其他未访问顶点的最短距离
        for (size_t v = 0; v < cluster.size(); v++) {
            if (!visited[v]) {
                double w =distanceCityToCity(
                    graphe,
                    getSommetParClef(graphe, cluster[u]),
                    getSommetParClef(graphe, cluster[v])
                );
                // 如果这条 (u->v) 更短，就更新 dist[v] 和 parent[v]
                if (w < dist[v]) {
                    dist[v] = w;
                    parent[v] = static_cast<int>(u);
                }
            }
        }
    }

    return {totalWeight, selectedEdges};
}

// 对奇度顶点做最小权完美匹配
double doMinimumWeightMatching(const Graphe<double, Ville>& graphe, const std::vector<int>& oddVertices, std::set<std::pair<int,int>>& matchedEdges)
{
    double matchingWeight = 0.0;
    if (oddVertices.size() < 2) return 0.0;

    std::vector<bool> used(oddVertices.size(), false);

    // 暴力贪心：每次选所有未匹配的顶点对里最小的边
    while (true) {
        int u = -1, v = -1;
        double minW = std::numeric_limits<double>::max();

        for (size_t i = 0; i < oddVertices.size(); i++) {
            if (used[i]) continue;
            for (size_t j = i + 1; j < oddVertices.size(); j++) {
                if (used[j]) continue;

                double w = distanceCityToCity(
                    graphe, 
                    getSommetParClef(graphe, oddVertices[i]), 
                    getSommetParClef(graphe, oddVertices[j]));
                if (w < minW) {
                    minW = w;
                    u = i; 
                    v = j;
                }
            }
        }

        // 如果找不到可匹配的顶点对，就退出
        if (u == -1 || v == -1) {
            break;
        }

        // 记录此匹配
        int idx1 = std::min(oddVertices[u], oddVertices[v]);
        int idx2 = std::max(oddVertices[u], oddVertices[v]);
        matchedEdges.insert({idx1, idx2});
        matchingWeight += minW;

        // 标记这两个顶点已经被匹配
        used[u] = true;
        used[v] = true;
    }

    return matchingWeight;
}


// Function to apply Christofides algorithm on a given cluster
// 用 Christofides 算法为一个簇解决旅行商问题（TSP）。
double calculateChristofides(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster) {

    if (cluster.size() < 2) {
        return 0.0;
    }
    
    // Step 1: Calculate the MST
    // 第一步：计算最小生成树（MST）的权重。就是上边那个方法
    auto [mstWeight, mstEdges] = calculateMSTWithEdges(graphe, cluster);

    // Step 2: Find odd-degree vertices in MST
    // 第二步：找出 MST 中度数为奇数的顶点。
    //    !!! 不是在原图里直接拿 sommet->degre，
    //    而是要根据 mstEdges 自己数一下度。
    std::unordered_map<int,int> mstDegree;
    for (auto &edge : mstEdges) {
        mstDegree[edge.first]  += 1;
        mstDegree[edge.second] += 1;
    }

    std::vector<int> oddVertices;
    for (int id : cluster) {
        if (mstDegree[id] % 2 != 0) {
            oddVertices.push_back(id);
        }
    }


    // Step 3: Perform minimum weight perfect matching on odd vertices
    // 第三步：对奇度顶点进行最小权完美匹配。
    std::set<std::pair<int,int>> matchedEdges;
    double matchingWeight = doMinimumWeightMatching(graphe, oddVertices, matchedEdges);

    // Step 4: Combine MST and matching to form an Eulerian graph
    // 第四步：将 MST 和匹配边合并，形成欧拉图。把两个集合的选取的边加起来
    std::set<std::pair<int,int>> eulerEdges = mstEdges;
    for (auto &me : matchedEdges) {
        eulerEdges.insert(me);
    }

    // Step 5: Construct Eulerian and Hamiltonian circuits
    // 第五步：构建密哈顿回路并计算结果
    std::vector<int> eulerCircuit;
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

// 版本2：返回 (总权重, 路径)
std::pair<double, std::vector<int>> calculateChristofidesWithPath(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster)
{
    if (cluster.size() < 2) {
        // 如果簇里只有0或1个城市，就返回 0 路径
        return {0.0, cluster};
    }

    // 1. MST
    auto [mstWeight, mstEdges] = calculateMSTWithEdges(graphe, cluster);

    // 2. 统计 MST 中度数，找奇度顶点
    std::unordered_map<int,int> mstDegree;
    for (auto &e : mstEdges) {
        mstDegree[e.first]  += 1;
        mstDegree[e.second] += 1;
    }
    std::vector<int> oddVertices;
    for (int id : cluster) {
        if (mstDegree[id] % 2 != 0) {
            oddVertices.push_back(id);
        }
    }

    // 3. 最小权匹配
    std::set<std::pair<int,int>> matchedEdges;
    double matchingWeight = doMinimumWeightMatching(graphe, oddVertices, matchedEdges);

    // 4. MST + 匹配 = 欧拉图
    std::set<std::pair<int,int>> eulerEdges = mstEdges;
    for (auto &me : matchedEdges) {
        eulerEdges.insert(me);
    }

    // 5. 欧拉 -> 哈密顿
    std::vector<int> eulerCircuit;
    constructEulerianCircuit(graphe, cluster[0], eulerCircuit, eulerEdges);

    std::vector<int> hamiltonianCircuit;
    constructHamiltonianCircuit(eulerCircuit, hamiltonianCircuit);

    // 计算最终哈密顿回路权重
    double hamiltonianWeight = 0.0;
    for (size_t i = 1; i < hamiltonianCircuit.size(); i++) {
        hamiltonianWeight += distanceCityToCity(
            graphe, 
            getSommetParClef(graphe, hamiltonianCircuit[i - 1]), 
            getSommetParClef(graphe, hamiltonianCircuit[i]));
    }

    return {hamiltonianWeight, hamiltonianCircuit};
}

// 遍历所有簇，分别用 Christofides 算法计算 (Cmax, 路径)
std::vector<std::pair<double, std::vector<int>>> getAllClusterRoutes(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters)
{
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



// 在所有簇里找到最大的 Cmax 以及对应路径
std::pair<double, std::vector<int>> getMaxCmaxWithPath(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters)
{
    double maxCmax = 0.0;
    std::vector<int> cmaxPath;

    for (size_t i = 0; i < clusters.size(); i++) {
        auto [cMax, path] = calculateChristofidesWithPath(graphe, clusters[i]);
        std::cout << "Cluster " << i << " Cmax: " << cMax << std::endl;
        if (cMax > maxCmax) {
            maxCmax = cMax;
            cmaxPath = path; 
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

// 获取“卡车路线”：实际上就是 Christofides 算出的哈密顿路径上的城市名字列表
std::vector<std::string> getTruckRouteForCluster(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster) {
    // 用 calculateChristofidesWithPath 来拿到哈密顿回路
    auto [hamiltonianWeight, hamiltonianCircuit] = calculateChristofidesWithPath(graphe, cluster);

    // 将城市 ID 转换成城市名字
    std::vector<std::string> truckRoute;
    for (int id : hamiltonianCircuit) {
        Sommet<Ville>* sommet = getSommetParClef(graphe, id);
        if (sommet) {
            truckRoute.push_back(sommet->v.name);
        }
    }
    return truckRoute;// 返回城市名字列表，表示卡车的路线
}

// 获取所有簇的卡车路线并打印
std::vector<std::vector<std::string>> getAllTruckRoutes(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters) {
    std::vector<std::vector<std::string>> allRoutes;

    for (size_t i = 0; i < clusters.size(); i++) {
        std::vector<std::string> route = getTruckRouteForCluster(graphe, clusters[i]);
        allRoutes.push_back(route);

        // 打印路线
        std::cout << "Truck Route for Cluster " << i << ": ";
        for (const auto& cityName : route) {
            std::cout << cityName << " ";
        }
        std::cout << std::endl;
    }

    return allRoutes;
}

/**
 * 对所有 KMeans++ 得到的簇，计算其 TSP 最小环路(Cmax)和访问路径（城市名）。
 * @param graphe   : 包含所有城市与距离的图
 * @param clusters : KMeans++ 的输出，每个子向量是一组城市的 clef
 * @return         : 大小为 clusters.size() 的向量，元素是 {Cmax, path}
 *                   其中 path 为城市名字的访问顺序（从第一个城市出发最终回到它）
 */
std::vector<std::pair<double, std::vector<std::string>>> 
calculateCmaxForAllClustersCL(const Graphe<double, Ville>& graphe,
                              const std::vector<std::vector<int>>& clusters)
{
    // 存放所有簇的结果，每个元素是 {Cmax, path (城市名序列)}
    std::vector<std::pair<double, std::vector<std::string>>> results;
    results.reserve(clusters.size());

    // 依次处理每个簇
    for (const auto& cluster : clusters) {
        // 利用前面实现好的函数，得到 (权重, 哈密顿回路的顶点 ID)
        auto [cMax, hamiltonianCircuit] = calculateChristofidesWithPath(graphe, cluster);

        // 将整数 ID 转换成城市名字
        std::vector<std::string> cityRoute;
        cityRoute.reserve(hamiltonianCircuit.size() + 1);

        for (int id : hamiltonianCircuit) {
            Sommet<Ville>* s = getSommetParClef(graphe, id);
            if (s) {
                cityRoute.push_back(s->v.name);
            }
        }

        // 如果想让最后回到起点，则在末尾再加一次首城市的名字
        if (!cityRoute.empty()) {
            cityRoute.push_back(cityRoute.front());
        }

        // 记录下 {Cmax, 该簇的城市访问顺序}
        results.emplace_back(cMax, cityRoute);
    }

    for (size_t i = 0; i < results.size(); ++i) {
    double cmax = results[i].first;
    const auto& route = results[i].second;
    std::cout << "Cluster " << i << " Cmax = " << cmax << "\nPath: ";
    for (const auto& city : route) {
        std::cout << city << " ";
    }
    std::cout << std::endl;
    }

    return results;
}
