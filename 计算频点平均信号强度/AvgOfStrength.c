#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
float* read_float_array_from_file(const char* filename, int* array_size);
float calculate_average_signal_strength(const float* signal_array, int array_size);
float calculate_signal_strength_from_file(const char* filename);
float* process_multiple_files(const char* base_filename, int* result_count);
void printData(const float* results, int num_files, const char* title);
void bubble_sort_float(float* array, int size);
void filterByThreshold(const float* results, int num_files, float threshold);


int main() {
    int num_files;

    float* results = process_multiple_files("data", &num_files);

    if (results == NULL || num_files == 0) {
        printf("错误：无法处理文件数据\n");
        return 1;
    }
    // 1. 显示原始数据
    printData(results, num_files, "原始数据");
    // 2. 排序
    bubble_sort_float(results, num_files);
    printData(results, num_files, "排序后数据");
    // 3. 筛选符合条件的信号强度
    filterByThreshold(results, num_files, 1000.0f);

    // 4. 确保在程序最后释放内存
    free(results);
    results = NULL; // 安全设置

    return 0;
}

// 筛选阈值函数
void filterByThreshold(const float* results, int num_files, float threshold) {
    int count = 0;

    if (results == NULL || num_files <= 0) {
        printf("没有可筛选的数据\n");
        return;
    }

    printf("\n满足阈值(%.2f)的信号强度:\n", threshold);
    for (int i = 0; i < num_files; i++) {
        if (results[i] > threshold) {
            printf("文件 %d: %.2f\n", i, results[i]);
            count++;
        }
    }

    if (count == 0) {
        printf("未找到符合条件的信号强度\n");
    } else {
        printf("共找到 %d 个符合条件的信号强度\n", count);
    }
}

//打印相关文件数据
void printData(const float* results, int num_files, const char* title) {
    if (results != NULL && num_files > 0) {
        printf("\n%s [共 %d 个]:\n", title, num_files);
        for (int i = 0; i < num_files; i++) {
            printf("文件 %d: 信号强度 = %.2f\n", i, results[i]);
        }
    } else {
        printf("没有可显示的数据\n");
    }
}

/*返回一个浮点型数组 存储所有结果*/
float* read_float_array_from_file(const char* filename, int* array_size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("文件打开失败");
        *array_size = 0;
        return NULL;
    }

    // 临时缓冲区读取数据
    float* temp = NULL;
    float value;
    int capacity = 10;
    int count = 0;

    temp = (float*)malloc(capacity * sizeof(float));
    if (temp == NULL) {
        fclose(file);
        *array_size = 0;
        return NULL;
    }

    // 读取文件中的数据
    while (fscanf(file, "%f", &value) == 1) {
        // 如果空间不足则扩容
        if (count >= capacity) {
            capacity *= 2;
            float* new_temp = (float*)realloc(temp, capacity * sizeof(float));
            if (new_temp == NULL) {
                free(temp);
                fclose(file);
                *array_size = 0;
                return NULL;
            }
            temp = new_temp;
        }
        temp[count++] = value;
    }

    // 如果文件中有未正确读取的内容
    if (!feof(file)) {
        fprintf(stderr, "文件中有未正确读取的内容\n");
    }

    // 缩小数组到实际大小
    float* result = (float*)realloc(temp, count * sizeof(float));
    if (result == NULL) {
        result = temp; // 如果失败则使用原始数组
    }

    fclose(file);
    *array_size = count;
    return result;
}

/*返回一个平均信号强度值*/
float calculate_average_signal_strength(const float* signal_array, int array_size) {
    // 确保数组大小是偶数（实部和虚部成对出现）
    if (array_size % 2 != 0) {
        // 可以根据需要返回0或特殊错误值
        return 0.0f;
    }

    int num_complex = array_size / 2; // 复数对的数量
    float total = 0.0f;  // 总和

    // 遍历数组中的每一对实部和虚部
    for (int i = 0; i < num_complex; i++) {
        float real_part = signal_array[2 * i];      // 虚部（偶数索引）
        float imag_part = signal_array[2 * i + 1];  // 实部（奇数索引）

        // 计算该复数模值的平方和
        float magnitude_squared = (real_part * real_part) + (imag_part * imag_part);

        // 累加模值
        total += sqrtf(magnitude_squared);
    }

    // 计算平均值
    return total / num_complex;
}

//接收文件名并返回计算结果
float calculate_signal_strength_from_file(const char* filename) {
    int array_size = 0;

    // 从文件读取数据
    float* data_array = read_float_array_from_file(filename, &array_size);

    // 检查文件读取是否成功
    if (data_array == NULL || array_size == 0) {
        fprintf(stderr, "文件读取是否失败 '%s'\n", filename);
        return 0.0f;
    }

    // 检查数据是否成对（实部和虚部）
    if (array_size % 2 != 0) {
        fprintf(stderr, "数据未成对出现\n", filename);
    }

    // 计算平均信号强度
    float result = calculate_average_signal_strength(data_array, array_size);

    // 释放分配的内存
    free(data_array);

    return result;
}

/*处理多个文件*/
float* process_multiple_files(const char* base_filename, int* result_count) {
    // 初始化动态数组
    int capacity = 10;  // 初始容量
    int count = 0;     // 当前处理文件数量
    float* results = (float*)malloc(capacity * sizeof(float));
    if (results == NULL) {
        perror("内存分配失败");
        *result_count = 0;
        return NULL;
    }

    // 处理文件序列 - 从0开始直到找不到文件
    for (int i = 0; ; i++) {
        // 构建文件名
        char filename[256];  // 假设文件名不超过255字符
        snprintf(filename, sizeof(filename), "%s%d.txt", base_filename, i);

        // 尝试打开文件以确认其存在
        FILE* test = fopen(filename, "r");
        if (test == NULL) {
            // 文件不存在，结束处理
            if (i == 0) {
                // 尝试无序号的文件名（如base_filename.txt）
                snprintf(filename, sizeof(filename), "%s.txt", base_filename);
                test = fopen(filename, "r");
                if (test == NULL) {
                    // 主文件也不存在，结束处理
                    break;
                }
                fclose(test);
            } else {
                // 序号文件不存在，结束处理
                break;
            }
        } else {
            fclose(test);
        }

        // 检查是否需要扩展结果数组
        if (count >= capacity) {
            capacity *= 2;
            float* new_results = (float*)realloc(results, capacity * sizeof(float));
            if (new_results == NULL) {
                perror("内存不足");
                // 返回已有结果（部分）
                *result_count = count;
                return results;
            }
            results = new_results;
        }

        // 处理当前文件
        float strength = calculate_signal_strength_from_file(filename);
        results[count++] = strength;

        printf(" %s: strength = %f\n", filename, strength);
    }

    // 如果没有任何文件被处理
    if (count == 0) {
        free(results);
        *result_count = 0;
        return NULL;
    }

    // 缩小数组到实际大小（可选）
    float* final_results = (float*)realloc(results, count * sizeof(float));
    if (final_results != NULL) {
        results = final_results;
    }

    *result_count = count;
    return results;
}

// 冒泡排序实现（升序排列）
void bubble_sort_float(float* array, int size) {
    // 检查边界条件
    if (array == NULL || size <= 1) {
        return;
    }

    int i, j;
    int swapped; // 标记是否发生交换
    for (i = 0; i < size - 1; i++) {
        swapped = 0; // 初始化为未发生交换

        // 每次遍历数组，两两比较相邻元素
        for (j = 0; j < size - 1 - i; j++) {
            // 如果前一个元素大于后一个元素，交换它们
            if (array[j] > array[j + 1]) {
                // 交换元素
                float temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;

                swapped = 1; // 标记发生交换
            }
        }

        // 如果在一次完整的遍历中没有发生任何交换，说明数组已排序完成
        if (!swapped) {
            break;
        }
    }
}

