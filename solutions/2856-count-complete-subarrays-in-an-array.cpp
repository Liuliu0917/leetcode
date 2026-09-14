// 题目: 统计完全子数组的数目
// 题号: 2856
// 语言: cpp
// 提交时间: 2026-09-13 07:20:01

class Solution {
public:
    int countCompleteSubarrays(vector<int>& nums) {
        int ans = 0;
        //求出整个数组中不同元素的数目
        unordered_map<int, int> cnt;
        for(int i = 0; i < nums.size(); i++) {
            cnt[nums[i]]++;
        }
        int diff_Num = cnt.size();
        //std::cout << diff_Num << std::endl;
        cnt.clear();

        int left = 0;
        for(int right = 0; right < nums.size(); right++) {
            cnt[nums[right]]++;
            
            while(cnt.size() == diff_Num) {
                cnt[nums[left]]--;
                if(cnt[nums[left]] == 0) {
                    cnt.erase(nums[left]);
                }
                left++;
            }
            ans += left;
        }
        return ans;
    }
};