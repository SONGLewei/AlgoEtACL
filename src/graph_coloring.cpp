#include "../includeAlgo/graph_coloring.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>  // 如需 memset，可改用 std::fill

static const int MAX_TASKS = 100; // 也可以改用 tasks.size() 动态处理

/**
 * 找到任务 task 的可用颜色（机器编号）。
 * incompatibility[i][j] = 1 表示 i 与 j 不兼容。
 */
static int find_available_color(int task,
                                const std::vector<int>& colors,
                                const std::vector<std::vector<int>>& incompatibility) {
    int num_tasks = (int)colors.size();
    std::vector<int> used_colors(num_tasks, 0);

    // 标记与 task 不兼容的任务已使用的颜色
    for (int i = 0; i < num_tasks; i++) {
        if (incompatibility[task][i] == 1 && colors[i] != -1) {
            used_colors[colors[i]] = 1;
        }
    }

    // 找到一个未使用的颜色
    for (int color = 0; color < num_tasks; color++) {
        if (!used_colors[color]) {
            return color;
        }
    }
    return -1;
}

void graph_coloring(const std::vector<int>& tasks,
                    const std::vector<std::vector<int>>& incompatibility) {
    int num_tasks = (int)tasks.size();
    if (incompatibility.size() != (size_t)num_tasks) {
        std::cout << "GraphColoring: incompatibility matrix size mismatch." << std::endl;
        return;
    }

    // colors[i]：任务 i 分配的颜色（机器 ID），-1 表示未分配
    std::vector<int> colors(num_tasks, -1);

    // 建立一个索引数组，用于按处理时间降序对任务排序
    std::vector<int> sorted_indices(num_tasks);
    for (int i = 0; i < num_tasks; i++) {
        sorted_indices[i] = i;
    }
    // 按 tasks[...] 降序排序
    std::sort(sorted_indices.begin(), sorted_indices.end(),
              [&tasks](int a, int b) {
                  return tasks[a] > tasks[b];
              });

    int max_color = 0;
    // 依次给任务分配颜色
    for (int i = 0; i < num_tasks; i++) {
        int task = sorted_indices[i];
        int color = find_available_color(task, colors, incompatibility);
        colors[task] = color;
        if (color > max_color) {
            max_color = color;
        }
    }

    std::cout << "\nGraph coloring completed.\n";
    std::cout << "Number of machines (colors) used: " << (max_color + 1) << std::endl;

    // 计算每台机器的负载
    std::vector<int> machine_loads(max_color + 1, 0);
    for (int i = 0; i < num_tasks; i++) {
        machine_loads[colors[i]] += tasks[i];
    }

    // 打印结果
    for (int c = 0; c <= max_color; c++) {
        std::cout << "Machine " << (c + 1) << ": Load = " << machine_loads[c] << ", Tasks = ";
        for (int i = 0; i < num_tasks; i++) {
            if (colors[i] == c) {
                std::cout << tasks[i] << "(task " << i << ") ";
            }
        }
        std::cout << std::endl;
    }
}
