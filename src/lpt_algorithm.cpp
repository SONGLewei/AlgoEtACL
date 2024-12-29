#include "../includeAlgo/lpt_algorithm.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

/**
 * 原有 LPT：返回 Cmax
 */
int LPT(std::vector<int>& tasks, int m) {
    int N = (int)tasks.size();
    if (N == 0 || m <= 0) {
        std::cout << "LPT: Invalid input parameters." << std::endl;
        return 0;
    }

    std::sort(tasks.begin(), tasks.end(), std::greater<int>());
    std::vector<int> machines(m, 0);

    for (int i = 0; i < N; i++) {
        int min_machine = 0;
        for (int j = 1; j < m; j++) {
            if (machines[j] < machines[min_machine]) {
                min_machine = j;
            }
        }
        machines[min_machine] += tasks[i];
        std::cout << "Task " << i << " (time " << tasks[i]
                  << ") -> machine " << (min_machine + 1)
                  << " (new load: " << machines[min_machine] << ")" << std::endl;
    }

    int Cmax = 0;
    for (int load : machines) {
        if (load > Cmax) {
            Cmax = load;
        }
    }
    std::cout << "LPT completed. Cmax: " << Cmax << std::endl;
    return Cmax;
}

/**
 * 新增 LPT_with_order：返回 (Cmax, assignment)
 * assignment[i] = 任务 i 分配到的机器编号。
 */
std::pair<int, std::vector<int>> LPT_with_order(std::vector<int>& tasks, int m) {
    int N = (int)tasks.size();
    if (N == 0 || m <= 0) {
        return {0, {}};
    }

    // 为了能回溯到“哪一个任务是原下标 i”，我们需要先保留原下标
    std::vector<std::pair<int,int>> indexedTasks(N);
    for (int i = 0; i < N; i++) {
        indexedTasks[i] = { tasks[i], i }; 
    }
    // 降序
    std::sort(indexedTasks.begin(), indexedTasks.end(), 
              [](auto& a, auto& b){ return a.first > b.first; });

    std::vector<int> machines(m, 0);        // 各机器当前负载
    std::vector<int> assignment(N, -1);     // assignment[i] = 哪台机器

    for (auto& it : indexedTasks) {
        int taskTime = it.first; 
        int originalIndex = it.second;

        // 找当前负载最小的机器
        int min_machine = 0;
        for (int j = 1; j < m; j++) {
            if (machines[j] < machines[min_machine]) {
                min_machine = j;
            }
        }
        // 分配
        machines[min_machine] += taskTime;
        assignment[originalIndex] = min_machine;
    }

    int Cmax = 0;
    for (int load : machines) {
        if (load > Cmax) {
            Cmax = load;
        }
    }
    return {Cmax, assignment};
}
