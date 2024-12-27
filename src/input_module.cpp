#include "../include/input_module.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>

static const int MAX_TASKS = 100;
static const int MAX_MACHINES = 100;

/**
 * 生成随机兼容矩阵
 */
static void generate_random_compatibility_matrix(std::vector<std::vector<int>>& matrix, int N) {
    // 这里调用 srand() 需要注意重复
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            } else {
                matrix[i][j] = rand() % 2; // 0或1
            }
        }
    }
}

/**
 * 生成随机任务和兼容性矩阵
 */
void generate_random_tasks(std::vector<int>& tasks,
                           std::vector<std::vector<int>>& compatibility,
                           int& N,
                           int& m,
                           int max_time) {
    srand((unsigned)time(nullptr));

    N = rand() % (MAX_TASKS - 1) + 1;     // 1~MAX_TASKS
    m = rand() % (MAX_MACHINES - 1) + 1;  // 1~MAX_MACHINES

    tasks.resize(N);
    for (int i = 0; i < N; i++) {
        tasks[i] = rand() % max_time + 1; // 1~max_time
    }

    compatibility.assign(N, std::vector<int>(N, 0));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            compatibility[i][j] = 0; // 默认全部兼容
        }
    }
}

/**
 * 交互式/用户输入式读取
 */
void read_input(std::vector<int>& tasks,
                std::vector<std::vector<int>>& compatibility,
                int& N,
                int& m) {
    std::cout << "Please enter the number of tasks (N) and machines (m) (max "
              << MAX_TASKS << " and " << MAX_MACHINES << " respectively): ";
    std::cin >> N >> m;

    if (N > MAX_TASKS || N <= 0) {
        std::cerr << "Error: The number of tasks must be between 1 and "
                  << MAX_TASKS << std::endl;
        exit(1);
    }
    if (m > MAX_MACHINES || m <= 0) {
        std::cerr << "Error: The number of machines must be between 1 and "
                  << MAX_MACHINES << std::endl;
        exit(1);
    }

    tasks.resize(N);
    std::cout << "Enter the processing time for each task (total " << N << " tasks): " << std::endl;
    for (int i = 0; i < N; i++) {
        std::cin >> tasks[i];
    }

    std::cout << "Do you want to input a compatibility matrix for the tasks? (1 for yes, 0 for no): ";
    int need_matrix = 0;
    std::cin >> need_matrix;

    compatibility.assign(N, std::vector<int>(N, 0)); 
    if (need_matrix) {
        std::cout << "Do you want to randomly generate the compatibility matrix? (1 for yes, 0 for manual): ";
        int random_matrix = 0;
        std::cin >> random_matrix;
        if (random_matrix) {
            generate_random_compatibility_matrix(compatibility, N);
            std::cout << "Generated compatibility matrix:\n";
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    std::cout << compatibility[i][j] << " ";
                }
                std::cout << std::endl;
            }
        } else {
            std::cout << "Enter a " << N << " x " << N
                      << " binary compatibility matrix:\n";
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    std::cin >> compatibility[i][j];
                    if (compatibility[i][j] != 0 && compatibility[i][j] != 1) {
                        std::cerr << "Error: The elements must be 0 or 1.\n";
                        exit(1);
                    }
                }
            }
        }
    } else {
        // 全部设为 0 表示完全兼容
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                compatibility[i][j] = 0;
            }
        }
    }

    std::cout << "Input completed!\n";
}
