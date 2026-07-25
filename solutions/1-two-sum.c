// 题目: 两数之和
// 题号: 1
// 语言: c
// 提交时间: 2026-07-25 09:36:15

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    for (int i = 0; i < numsSize; i++)
    {
        for (int j = i + 1; j < numsSize; j++)
        {
            if (nums[i] + nums[j] == target)
            {
                int* result = (int*)malloc(2 * sizeof(int));//创建返回的新整数数组result
                result[0] = i;
                result[1] = j;
                *returnSize = 2;
                return result;
            }
        }
    }
    *returnSize = 0;
    return 0;
}