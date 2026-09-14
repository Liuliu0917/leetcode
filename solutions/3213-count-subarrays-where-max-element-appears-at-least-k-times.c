// 题目: 统计最大元素出现至少 K 次的子数组
// 题号: 3213
// 语言: c
// 提交时间: 2026-09-09 13:03:34

#define MAX(a, b) a > b ? a : b

long long countSubarrays(int* nums, int numsSize, int k) {
    long long ans = 0;
    int max = 0;
    for(int i = 0; i < numsSize; i++) {
        max = MAX(max, nums[i]);
    }
    int max_cnt = 0;
    int left = 0;
    for(int right = 0; right < numsSize; right++) {
        if(nums[right] == max) {
            max_cnt++;
        }
        while(max_cnt >= k) {
            if(nums[left] == max) {
                max_cnt--;
            }
            left++;
        }
        ans += left;
    }
    return ans;
}