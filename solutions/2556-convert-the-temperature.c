// 题目: 温度转换
// 题号: 2556
// 语言: c
// 提交时间: 2025-12-19 09:14:14

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
double* convertTemperature(double celsius, int* returnSize) {
     double *res = (double*)malloc(sizeof(double)*2);
     res[0] = celsius + 273.15;
     res[1] = celsius*1.80 + 32.00;
     *returnSize = 2;
     return res;
}