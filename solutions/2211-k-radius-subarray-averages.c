// 题目: 半径为 k 的子数组平均值
// 题号: 2211
// 语言: c
// 提交时间: 2026-08-04 03:11:16

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* getAverages(int* nums, int numsSize, int k, int* returnSize) {
    //※记住要设置 returnSize 的值, LeetCode 需要通过 *returnSize 知道返回数组的长度
    
    //根据示例3可知，需要增加对2 * k 和 numsSize 的大小判断

    int n = numsSize;
    int* avgs = (int*) malloc (n * sizeof(int));
    *returnSize = n;

    if(2 * k + 1 > numsSize) {
        for(int i = 0; i < n; i++){
            avgs[i] = -1;
        }
    }else {
        //因为sum累加会很大，会超过int的表示范围，所以要用longlong
        long long sum = 0;

        for(int j = 0; j < 2 * k; j++) {
            sum += nums[j]; 
        }
        for(int i = 0; i < n; i++) {
            //1
            //下标在 i - k 和 i + k 范围
            int left = i - k;
            int right = i + k;

            if(left < 0 || right > n-1) {
                avgs[i] = -1;
                continue;
            }

            sum += nums[i + k];

            //2
            avgs[i] = sum / (2 * k + 1);

            //3
            sum -= nums[i - k];
        }
    }
    return avgs;
}