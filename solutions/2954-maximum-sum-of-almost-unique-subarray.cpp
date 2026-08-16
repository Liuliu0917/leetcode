// 题目: 几乎唯一子数组的最大和
// 题号: 2954
// 语言: cpp
// 提交时间: 2026-08-15 09:34:33

class Solution {
public:
    long long maxSum(vector<int>& nums, int m, int k) {
        long long ans = 0; //最大的sum
        long long sum = 0; //暂存本次循环的sum值
        unordered_map<int, int> cnt; //创建键和值都为int类型的哈希表(记录每个整数出现的次数)
        for(int right = 0; right < nums.size(); right++) {
            //1
            sum += nums[right];
            cnt[nums[right]]++;

            int left = right - k + 1;
            if(left < 0) {
                continue;
            }

            //2
            if(cnt.size() >= m) {//cnt.size()计算的是哈希表中键的个数（刚好对应互不相同的元素）
                ans = max(ans, sum);
            }

            //3
            sum -= nums[left];
            cnt[nums[left]]--;
            if(cnt[nums[left]] == 0) {
                cnt.erase(nums[left]); // 保证 cnt.size() 不计入出现 0 次的元素
            }
        }
        return ans;
    }
};