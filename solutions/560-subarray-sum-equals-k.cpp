// 题目: 和为 K 的子数组
// 题号: 560
// 语言: cpp
// 提交时间: 2026-09-14 14:34:41

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int sum = 0;
        int ans = 0;
        unordered_map<int, int> cnt;
        for(int i = 0; i < nums.size(); i++) {
            cnt[sum]++;
            sum += nums[i];
            ans += cnt[sum - k];
        }
        return ans;
    }
};