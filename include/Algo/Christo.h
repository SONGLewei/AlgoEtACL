#pragma once
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <set>
#include <stack>
#include <unordered_map>
#include "../Graphe/BuildNewGraphe.h"
#include "Kmeans.h"

template<typename T>
struct Sommet;

struct Ville;

//double distanceCityToCity(const Graphe<double, Ville>& graphe, Sommet<Ville>* city1, Sommet<Ville>* city2);

// 函数声明
Sommet<Ville>* getSommetParClef(const Graphe<double, Ville>& graphe, int clef);

void constructEulerianCircuit(const Graphe<double, Ville>& graphe, int start, std::vector<int>& circuit, std::set<std::pair<int, int>>& edges);

void constructHamiltonianCircuit(const std::vector<int>& eulerCircuit, std::vector<int>& hamiltonianCircuit);

std::pair<double, std::set<std::pair<int, int>>> calculateMSTWithEdges(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster);

double doMinimumWeightMatching(const Graphe<double, Ville>& graphe, const std::vector<int>& oddVertices, std::set<std::pair<int, int>>& matchedEdges);

double calculateChristofides(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster);

std::pair<double, std::vector<int>> calculateChristofidesWithPath(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster);

std::vector<std::pair<double, std::vector<int>>> getAllClusterRoutes(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters);

std::pair<double, std::vector<int>> getMaxCmaxWithPath(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters);

double getMaxCmax(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters);

std::vector<std::string> getTruckRouteForCluster(const Graphe<double, Ville>& graphe, const std::vector<int>& cluster);

std::vector<std::vector<std::string>> getAllTruckRoutes(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters);

std::vector<std::pair<double, std::vector<std::string>>> calculateCmaxForAllClustersCL(const Graphe<double, Ville>& graphe, const std::vector<std::vector<int>>& clusters);