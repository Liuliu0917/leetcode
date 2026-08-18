// 题目: 使库存平衡的最少丢弃次数
// 题号: 3953
// 语言: cpp
// 提交时间: 2026-08-16 10:18:37

class Solution {
public:
    int minArrivalsToDiscard(vector<int>& arrivals, int w, int m) {
        int DiscardNum = 0; //本题直接按最少丢弃物品的方式计算（所以没有//2）
        unordered_map<int, int> cnt;
        for(int right = 0; right < arrivals.size(); right++) {
            //1
            cnt[arrivals[right]]++;

            if(cnt[arrivals[right]] > m){   //滑动窗口长度不到w，就可能开始丢弃物品，所以丢弃判断要提前到left判断前面
                //std::cout << cnt[arrivals[right]] << std::endl;
                cnt[arrivals[right]]--;
                arrivals[right] = 0;
                DiscardNum++;
            }
            
            int left = right - w + 1;
            if(left < 0) {
                continue;
            }
            //3
            cnt[arrivals[left]]--;
        }
        return DiscardNum;
    }
};