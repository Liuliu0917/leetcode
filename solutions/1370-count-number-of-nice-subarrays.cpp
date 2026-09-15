// 题目: 统计「优美子数组」
// 题号: 1370
// 语言: cpp
// 提交时间: 2026-09-15 06:20:54

class Solution {
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        int num_subarrays = 0;
        unordered_map<int, int> cnt;
        int ans = 0;
        for(int i = 0; i < nums.size(); i++) {
            cnt[num_subarrays]++;
            if(nums[i]%2 != 0){
                num_subarrays++;
            }
            ans += cnt[num_subarrays - k];
        }
        return ans;
    }
};