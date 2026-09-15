// 题目: 统计「优美子数组」
// 题号: 1370
// 语言: c
// 提交时间: 2026-09-15 06:29:34

int numberOfSubarrays(int* nums, int numsSize, int k) {
    int ans = 0;
    int num1 = 0, num2 = 0;
    int left1 = 0, left2 = 0;

    for(int right = 0; right < numsSize; right++) {
        if(nums[right] % 2 != 0) {
            num1++;
            num2++;
        }

        while(num1 >= k && left1 <= right) {
            if(nums[left1] % 2 != 0) {
                num1--;
            }
            left1++;
        }
        while(num2 > k) {
            if(nums[left2] % 2 != 0) {
                num2--;
            }
            left2++;
        }

        ans = ans + left1 - left2;
    }
    return ans;
}