// 题目: 删掉一个元素以后全为 1 的最长子数组
// 题号: 1586
// 语言: c
// 提交时间: 2026-08-14 09:43:52

#define max(a, b) ((a > b) ? a : b)

int longestSubarray(int* nums, int numsSize) {
    int length = 0;
    int Not1cnt = 0;
    int left = 0;
    for(int right = 0; right < numsSize; right++) {
        //1
        if(nums[right] != 1) {
            Not1cnt++;
        }
        //2
        while(Not1cnt > 1) {
            if(nums[left] != 1) {
                Not1cnt--;
            }
            left++;
        }
        //3
        length = max(length, right - left + 1);
    }
    return length - 1;  //length是包含那个没删除的元素的子数组长度
}