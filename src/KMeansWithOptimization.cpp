#include "../includeGraphe/KMeansWithOptimization.h"
#include "../includeAlgo/fptas_algorithm.hpp"
#include "../includeAlgo/graph_coloring.hpp"
#include "../includeGraphe/KMeans.h"
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <limits>
#include <chrono>

Graphe<double, Ville> KMeansWithOptimization::optimizeAndReturnGraph(
    const Graphe<double, Ville>& graphe, double epsilon, double improvementThreshold) {
    int K = 1;
    double prevCmax = std::numeric_limits<double>::max();
    Graphe<double, Ville> bestGraph;

    const int MAX_ITERATIONS = 100;
    int iterationCount = 0;

    std::ofstream resultFile("result.txt");
    if (!resultFile.is_open()) {
        std::cerr << "Failed to open result file." << std::endl;
        return Graphe<double, Ville>();
    }

    while (true) {
        iterationCount++;
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<Ville> villes;
        for (auto it = graphe.lSommets; it != nullptr; it = it->suivant) {
            villes.push_back(it->valeur->v);
        }

        auto bestClusters = std::vector<std::vector<Ville>>();
        double bestCmax = std::numeric_limits<double>::max();

        for (int i = 0; i < 10; ++i) {
            auto tempClusters = kMeansClustering(villes, K);
            double tempCmax = calculateMaxLoad(tempClusters, graphe);
            if (tempCmax < bestCmax) {
                bestClusters = tempClusters;
                bestCmax = tempCmax;
            }
        }

        auto clusters = bestClusters;

        std::vector<Graphe<double, Ville>> clusterGraphs;
        std::vector<int> clusterLoads;

        for (const auto& cluster : clusters) {
            std::unordered_set<std::string> clusterCityNames;
            for (const auto& ville : cluster) {
                clusterCityNames.insert(ville.name);
            }

            Graphe<double, Ville> clusterGraph = graphe.extractSubgraph(clusterCityNames);
            clusterGraphs.push_back(clusterGraph);
            clusterLoads.push_back(calculateClusterLoad(clusterGraph));
        }

        auto balancedResult = balanceClustersFPTAS(clusterLoads, K, epsilon);
        if (balancedResult.first < 0) {
            std::cerr << "FPTAS balancing failed." << std::endl;
            break;
        }

        double currentCmax = balancedResult.first;
        double improvement = (prevCmax - currentCmax) / prevCmax;
        double absoluteChange = std::abs(prevCmax - currentCmax);

        std::cout << "Previous Cmax: " << prevCmax << ", Current Cmax: " << currentCmax << std::endl;
        resultFile << "Previous Cmax: " << prevCmax << ", Current Cmax: " << currentCmax << std::endl;

        std::cout << "Iteration with K = " << K << ", Approx Cmax: " << currentCmax << std::endl;
        resultFile << "Iteration with K = " << K << ", Approx Cmax: " << currentCmax << std::endl;

        std::cout << "Improvement from previous Cmax: " << improvement * 100 << "%" << std::endl;
        resultFile << "Improvement from previous Cmax: " << improvement * 100 << "%" << std::endl;

        std::cout << "Absolute change in Cmax: " << absoluteChange << std::endl;
        resultFile << "Absolute change in Cmax: " << absoluteChange << std::endl;

        for (size_t i = 0; i < clusterGraphs.size(); ++i) {
            std::cout << "Cluster " << i + 1 << ": ";
            resultFile << "Cluster " << i + 1 << ": ";

            for (auto it = clusterGraphs[i].lSommets; it != nullptr; it = it->suivant) {
                std::cout << it->valeur->v.name << " ";
                resultFile << it->valeur->v.name << " ";
            }
            std::cout << "Load: " << clusterLoads[i] << std::endl;
            resultFile << "Load: " << clusterLoads[i] << std::endl;
        }

        if ((improvement < improvementThreshold && absoluteChange < 1e-3) || iterationCount >= MAX_ITERATIONS) {
            std::cout << "Stopping optimization: ";
            if (improvement < improvementThreshold) std::cout << "Improvement below threshold.";
            else if (absoluteChange < 1e-3) std::cout << "Cmax change below minimum threshold.";
            else if (iterationCount >= MAX_ITERATIONS) std::cout << "Reached max iterations.";
            std::cout << std::endl;

            resultFile << "Stopping optimization." << std::endl;
            break;
        }

        prevCmax = currentCmax;
        bestGraph = mergeClusters(clusterGraphs);

        K++;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Iteration time: " << elapsed.count() << " seconds." << std::endl;
        resultFile << "Iteration time: " << elapsed.count() << " seconds." << std::endl;
    }

    resultFile << "Final Cmax: " << prevCmax << std::endl;
    resultFile.close();
    return bestGraph;
}

int KMeansWithOptimization::calculateClusterLoad(const Graphe<double, Ville>& cluster) {
    double totalLoad = 0.0;
    for (auto it = cluster.lAretes; it != nullptr; it = it->suivant) {
        totalLoad += it->valeur->v;
    }
    return static_cast<int>(totalLoad);
}

std::pair<double, std::vector<int>> KMeansWithOptimization::balanceClustersFPTAS(
    const std::vector<int>& clusterLoads, int numClusters, double epsilon) {
    return FPTAS_with_order(clusterLoads, numClusters, epsilon);
}

Graphe<double, Ville> KMeansWithOptimization::mergeClusters(
    const std::vector<Graphe<double, Ville>>& clusters) {
    Graphe<double, Ville> mergedGraph;
    for (const auto& cluster : clusters) {
        for (auto it = cluster.lSommets; it != nullptr; it = it->suivant) {
            mergedGraph.creeSommet(it->valeur->v);
        }
        for (auto it = cluster.lAretes; it != nullptr; it = it->suivant) {
            mergedGraph.creeArete(it->valeur->v, it->valeur->debut, it->valeur->fin);
        }
    }
    return mergedGraph;
}

double KMeansWithOptimization::calculateMaxLoad(
    const std::vector<std::vector<Ville>>& clusters, const Graphe<double, Ville>& graphe) {
    double maxLoad = 0.0;
    for (const auto& cluster : clusters) {
        std::unordered_set<std::string> clusterCityNames;
        for (const auto& ville : cluster) {
            clusterCityNames.insert(ville.name);
        }
        Graphe<double, Ville> subgraph = graphe.extractSubgraph(clusterCityNames);
        maxLoad = std::max(maxLoad, static_cast<double>(calculateClusterLoad(subgraph)));
    }
    return maxLoad;
}
