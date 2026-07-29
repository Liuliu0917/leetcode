// 题目: 重新排列数组
// 题号: 1580
// 语言: c
// 提交时间: 2026-07-27 12:02:05

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* shuffle(int* nums, int numsSize, int n, int* returnSize) {
    int* nums2 = (int*)malloc(2*n*sizeof(int));  
    //注释已经说了 "The returned array must be malloced"，必须用 malloc 在堆上分配内存。
    for(int i = 0; i < n; i++) {
        nums2[i*2] = nums[i];
        nums2[i*2+1] = nums[i+n];
    }
    *returnSize = 2 * n;
    return nums2;
}