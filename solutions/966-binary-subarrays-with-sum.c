// 题目: 和相同的二元子数组
// 题号: 966
// 语言: c
// 提交时间: 2026-09-15 05:07:13

int numSubarraysWithSum(int* nums, int numsSize, int goal) {
    int ans = 0;
    int sum1 = 0, sum2 = 0;
    int left1 = 0, left2 = 0;

    for(int right = 0; right < numsSize; right++) {
        sum1 += nums[right];
        sum2 += nums[right];

        while(sum1 >= goal && left1 <= right) {
            sum1 -= nums[left1];
            left1++;
        }

        while(sum2 > goal && left2 <= right) {
            sum2 -= nums[left2];
            left2++;
        }
        
        ans = ans + left1 - left2;
    }
    return ans;
}