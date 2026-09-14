// 题目: 和相同的二元子数组
// 题号: 966
// 语言: cpp
// 提交时间: 2026-09-14 14:28:55

/*假设原数组的前缀和数组为 sum，且子数组 (i,j] 的区间和为 goal，那么 sum[j]−sum[i]=goal。因此我们可以枚举 j ，每次查询满足该等式的 i 的数量*/
class Solution {
public:
    int numSubarraysWithSum(vector<int>& nums, int goal) {
        int sum = 0;
        unordered_map<int, int> cnt;
        int ans = 0;
        for(int i = 0; i < nums.size(); i++) {
            cnt[sum]++;
            sum += nums[i]; //sum[j]的开头j应该是符合要求子数组左端点-1（left - 1）
            ans += cnt[sum - goal];
        }
        return ans;
    }
};