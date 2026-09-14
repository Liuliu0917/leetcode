// 题目: 乘积小于 K 的子数组
// 题号: 713
// 语言: cpp
// 提交时间: 2026-09-08 08:43:07

class Solution {
public:
    int numSubarrayProductLessThanK(vector<int>& nums, int k) {
        int num = 0;
        int Product = 1;
        int left = 0;
        /*if(k == 0) {
            return 0;
        }*/
        for(int right = 0; right < nums.size(); right++) {
            //右
            Product *= nums[right];
            //左
            while(Product >= k && left <= right) { //※数组的边界检查很重要
                Product /= nums[left];
                left++;
            }
            if(Product >= k) {
                return 0;
            }
            //num
            num = num + right - left + 1;
        }
        return num;
    }
};