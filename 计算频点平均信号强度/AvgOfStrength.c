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
        printf("�����޷������ļ�����\n");
        return 1;
    }
    // 1. ��ʾԭʼ����
    printData(results, num_files, "ԭʼ����");
    // 2. ����
    bubble_sort_float(results, num_files);
    printData(results, num_files, "���������");
    // 3. ɸѡ�����������ź�ǿ��
    filterByThreshold(results, num_files, 1000.0f);

    // 4. ȷ���ڳ�������ͷ��ڴ�
    free(results);
    results = NULL; // ��ȫ����

    return 0;
}

// ɸѡ��ֵ����
void filterByThreshold(const float* results, int num_files, float threshold) {
    int count = 0;

    if (results == NULL || num_files <= 0) {
        printf("û�п�ɸѡ������\n");
        return;
    }

    printf("\n������ֵ(%.2f)���ź�ǿ��:\n", threshold);
    for (int i = 0; i < num_files; i++) {
        if (results[i] > threshold) {
            printf("�ļ� %d: %.2f\n", i, results[i]);
            count++;
        }
    }

    if (count == 0) {
        printf("δ�ҵ������������ź�ǿ��\n");
    } else {
        printf("���ҵ� %d �������������ź�ǿ��\n", count);
    }
}

//��ӡ����ļ�����
void printData(const float* results, int num_files, const char* title) {
    if (results != NULL && num_files > 0) {
        printf("\n%s [�� %d ��]:\n", title, num_files);
        for (int i = 0; i < num_files; i++) {
            printf("�ļ� %d: �ź�ǿ�� = %.2f\n", i, results[i]);
        }
    } else {
        printf("û�п���ʾ������\n");
    }
}

/*����һ������������ �洢���н��*/
float* read_float_array_from_file(const char* filename, int* array_size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("�ļ���ʧ��");
        *array_size = 0;
        return NULL;
    }

    // ��ʱ��������ȡ����
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

    // ��ȡ�ļ��е�����
    while (fscanf(file, "%f", &value) == 1) {
        // ����ռ䲻��������
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

    // ����ļ�����δ��ȷ��ȡ������
    if (!feof(file)) {
        fprintf(stderr, "�ļ�����δ��ȷ��ȡ������\n");
    }

    // ��С���鵽ʵ�ʴ�С
    float* result = (float*)realloc(temp, count * sizeof(float));
    if (result == NULL) {
        result = temp; // ���ʧ����ʹ��ԭʼ����
    }

    fclose(file);
    *array_size = count;
    return result;
}

/*����һ��ƽ���ź�ǿ��ֵ*/
float calculate_average_signal_strength(const float* signal_array, int array_size) {
    // ȷ�������С��ż����ʵ�����鲿�ɶԳ��֣�
    if (array_size % 2 != 0) {
        // ���Ը�����Ҫ����0���������ֵ
        return 0.0f;
    }

    int num_complex = array_size / 2; // �����Ե�����
    float total = 0.0f;  // �ܺ�

    // ���������е�ÿһ��ʵ�����鲿
    for (int i = 0; i < num_complex; i++) {
        float real_part = signal_array[2 * i];      // �鲿��ż��������
        float imag_part = signal_array[2 * i + 1];  // ʵ��������������

        // ����ø���ģֵ��ƽ����
        float magnitude_squared = (real_part * real_part) + (imag_part * imag_part);

        // �ۼ�ģֵ
        total += sqrtf(magnitude_squared);
    }

    // ����ƽ��ֵ
    return total / num_complex;
}

//�����ļ��������ؼ�����
float calculate_signal_strength_from_file(const char* filename) {
    int array_size = 0;

    // ���ļ���ȡ����
    float* data_array = read_float_array_from_file(filename, &array_size);

    // ����ļ���ȡ�Ƿ�ɹ�
    if (data_array == NULL || array_size == 0) {
        fprintf(stderr, "�ļ���ȡ�Ƿ�ʧ�� '%s'\n", filename);
        return 0.0f;
    }

    // ��������Ƿ�ɶԣ�ʵ�����鲿��
    if (array_size % 2 != 0) {
        fprintf(stderr, "����δ�ɶԳ���\n", filename);
    }

    // ����ƽ���ź�ǿ��
    float result = calculate_average_signal_strength(data_array, array_size);

    // �ͷŷ�����ڴ�
    free(data_array);

    return result;
}

/*�������ļ�*/
float* process_multiple_files(const char* base_filename, int* result_count) {
    // ��ʼ����̬����
    int capacity = 10;  // ��ʼ����
    int count = 0;     // ��ǰ�����ļ�����
    float* results = (float*)malloc(capacity * sizeof(float));
    if (results == NULL) {
        perror("�ڴ����ʧ��");
        *result_count = 0;
        return NULL;
    }

    // �����ļ����� - ��0��ʼֱ���Ҳ����ļ�
    for (int i = 0; ; i++) {
        // �����ļ���
        char filename[256];  // �����ļ���������255�ַ�
        snprintf(filename, sizeof(filename), "%s%d.txt", base_filename, i);

        // ���Դ��ļ���ȷ�������
        FILE* test = fopen(filename, "r");
        if (test == NULL) {
            // �ļ������ڣ���������
            if (i == 0) {
                // ��������ŵ��ļ�������base_filename.txt��
                snprintf(filename, sizeof(filename), "%s.txt", base_filename);
                test = fopen(filename, "r");
                if (test == NULL) {
                    // ���ļ�Ҳ�����ڣ���������
                    break;
                }
                fclose(test);
            } else {
                // ����ļ������ڣ���������
                break;
            }
        } else {
            fclose(test);
        }

        // ����Ƿ���Ҫ��չ�������
        if (count >= capacity) {
            capacity *= 2;
            float* new_results = (float*)realloc(results, capacity * sizeof(float));
            if (new_results == NULL) {
                perror("�ڴ治��");
                // �������н�������֣�
                *result_count = count;
                return results;
            }
            results = new_results;
        }

        // ����ǰ�ļ�
        float strength = calculate_signal_strength_from_file(filename);
        results[count++] = strength;

        printf(" %s: strength = %f\n", filename, strength);
    }

    // ���û���κ��ļ�������
    if (count == 0) {
        free(results);
        *result_count = 0;
        return NULL;
    }

    // ��С���鵽ʵ�ʴ�С����ѡ��
    float* final_results = (float*)realloc(results, count * sizeof(float));
    if (final_results != NULL) {
        results = final_results;
    }

    *result_count = count;
    return results;
}

// ð������ʵ�֣��������У�
void bubble_sort_float(float* array, int size) {
    // ���߽�����
    if (array == NULL || size <= 1) {
        return;
    }

    int i, j;
    int swapped; // ����Ƿ�������
    for (i = 0; i < size - 1; i++) {
        swapped = 0; // ��ʼ��Ϊδ��������

        // ÿ�α������飬�����Ƚ�����Ԫ��
        for (j = 0; j < size - 1 - i; j++) {
            // ���ǰһ��Ԫ�ش��ں�һ��Ԫ�أ���������
            if (array[j] > array[j + 1]) {
                // ����Ԫ��
                float temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;

                swapped = 1; // ��Ƿ�������
            }
        }

        // �����һ�������ı�����û�з����κν�����˵���������������
        if (!swapped) {
            break;
        }
    }
}

