// 题目: 统计字符串中的元音子字符串
// 题号: 2186
// 语言: cpp
// 提交时间: 2026-09-13 06:59:47

/*在遍历 word 的同时，维护子串的最小左端点 start（初始值为 0）。当遍历到辅音 word[i] 时，更新 start=i+1, 表示后续子串的左端点不能小于 i+1，否则子串包含辅音 word[i]，不满足要求*/
class Solution {
public:
    int countVowelSubstrings(string word) { //※元音子字符串只包含元音
        unordered_map<char, int> cnt;
        int ans = 0;
        int start = 0;
        int left = 0;

        for(int right = 0; right < word.size(); right++) {
            if(word[right] != 'a' && word[right] != 'e' && word[right] != 'i' && word[right] != 'o' && word[right] != 'u') {
                cnt.clear();  //重置哈希表
                start = left = right + 1;
                continue;
            }

            cnt[word[right]]++;
            while(cnt.size() == 5) {  //包含所有元音
                cnt[word[left]]--;
                if(cnt[word[left]] == 0) {
                    cnt.erase(word[left]);
                }
                left++;
            }
            ans += left - start;
        }
        return ans;
    }
};