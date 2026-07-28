// 题目: 转置矩阵
// 题号: 898
// 语言: c
// 提交时间: 2026-07-28 12:02:06

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** transpose(int** matrix, int matrixSize, int* matrixColSize, int* returnSize, int** returnColumnSizes) {
    int rows = matrixSize;          // 原矩阵行数
    int cols = matrixColSize[0];    // 原矩阵列数
    
    //※returnSize指向matrixSize；returnColumnSizes指向matrixClosize

    *returnSize = cols;   // 转置后：cols 行，rows 列
    *returnColumnSizes = (int*)malloc(cols * sizeof(int));  // 分配 returnColumnSizes：记录结果矩阵每行的列数
    for (int i = 0; i < cols; i++) {
        (*returnColumnSizes)[i] = rows;
    }

    //※分配结果矩阵 （先分配行，后分配列）
    int** result = (int**)malloc(cols * sizeof(int*)); //cols * (int*)
    for(int i = 0; i < cols; i++) {
        result[i] = (int*)malloc(rows * sizeof(int)); // row * (int)
    }
    //填充转置数据
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}