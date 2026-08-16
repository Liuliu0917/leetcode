// 题目: 长度为 K 子数组中的最大和
// 题号: 2552
// 语言: cpp
// 提交时间: 2026-08-15 09:41:20

class Solution {
public:
    long long maximumSubarraySum(vector<int>& nums, int k) {
        long long ans = 0;
        long long sum = 0;
        unordered_map<int, int> cnt;
        for(int right = 0; right < nums.size(); right++) {
            //1
            sum += nums[right];
            cnt[nums[right]]++;

            int left = right - k + 1;
            if(left < 0) {
                continue;
            }

            //2
            if(cnt.size() == k) {
                ans = max(ans, sum);
            }

            //3
            sum -= nums[left];
            cnt[nums[left]]--;
            if(cnt[nums[left]] == 0) {
                cnt.erase(nums[left]);
            }
        }
        return ans;
    }
};