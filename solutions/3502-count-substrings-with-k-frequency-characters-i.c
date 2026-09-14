// 题目: 字符至少出现 K 次的子字符串 I
// 题号: 3502
// 语言: c
// 提交时间: 2026-09-09 13:13:08

int numberOfSubstrings(char* s, int k) {
    int ans = 0;
    int left = 0;
    int cnt[26] = {0}; //s 仅由小写英文字母组成
    for(int right = 0; s[right]; right++) {
        cnt[s[right] - 'a']++;
        while(cnt[s[right] - 'a'] >= k) {
            cnt[s[left] - 'a']--;
            left++;
        }
        ans += left;
    }
    return ans;
}