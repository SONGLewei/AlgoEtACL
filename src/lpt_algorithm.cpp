#include "../include/lpt_algorithm.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

/**
 * LPT 调度算法实现：先对任务按降序排列，然后依次分配到当前负载最小的机器上。
 */
int LPT(std::vector<int>& tasks, int m) {
    int N = (int)tasks.size();
    if (N == 0 || m <= 0) {
        std::cout << "LPT: Invalid input parameters." << std::endl;
        return 0;
    }

    // 按处理时间降序排序
    std::sort(tasks.begin(), tasks.end(), std::greater<int>());

    // 机器负载数组
    std::vector<int> machines(m, 0);

    // 依次分配任务
    for (int i = 0; i < N; i++) {
        // 找到当前负载最小的机器
        int min_machine = 0;
        for (int j = 1; j < m; j++) {
            if (machines[j] < machines[min_machine]) {
                min_machine = j;
            }
        }
        // 将任务 i 分配给 min_machine
        machines[min_machine] += tasks[i];

        std::cout << "Task " << i << " (time " << tasks[i]
                  << ") assigned to machine " << (min_machine + 1)
                  << " (new load: " << machines[min_machine] << ")" << std::endl;
    }

    // 计算最大完成时间
    int Cmax = 0;
    for (int load : machines) {
        if (load > Cmax) {
            Cmax = load;
        }
    }
    std::cout << "LPT scheduling completed. Maximum completion time (Cmax): "
              << Cmax << std::endl;
    return Cmax;
}
