#include <iostream>
#include "../include/dp_algorithm.hpp"
#include "../include/fptas_algorithm.hpp"
#include "../include/lpt_algorithm.hpp"
#include "../include/graph_coloring.hpp"
#include "../include/input_module.hpp"

int main() {
    // 准备一些数据容器
    std::vector<int> tasks;
    std::vector<std::vector<int>> compatibility;
    int N = 0, m = 0;

    // 方式 1: 让用户输入
    read_input(tasks, compatibility, N, m);
    // 或者:
    // generate_random_tasks(tasks, compatibility, N, m, 20);

    // 打印输入的任务信息
    std::cout << "Number of tasks N = " << N << ", number of machines m = " << m << std::endl;
    std::cout << "Tasks: ";
    for (int t : tasks) {
        std::cout << t << " ";
    }
    std::cout << std::endl;

    // 1) 测试 DP
    int dpCmax = DP(tasks, m);

    // 2) 测试 FPTAS (若 epsilon 太小，会被拒绝)
    double epsilon = 1.0; // 示例
    double approxCmax = FPTAS(tasks, m, epsilon);

    // 3) 测试 LPT
    // 注意 LPT 会对 tasks 排序，为了演示独立性，先拷贝一份
    std::vector<int> tasks_copy = tasks;
    int lptCmax = LPT(tasks_copy, m);

    // 4) 测试图着色
    graph_coloring(tasks, compatibility);

    std::cout << "\n===== Summary =====\n"
              << "DP Cmax = " << dpCmax << "\n"
              << "FPTAS Cmax = " << approxCmax << "\n"
              << "LPT Cmax = " << lptCmax << "\n"
              << std::endl;

    return 0;
}
