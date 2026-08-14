// 题目: 每个字符最多出现两次的最长子字符串
// 题号: 3349
// 语言: c
// 提交时间: 2026-08-14 09:32:38

#define max(a, b) ((a > b) ? a : b)

int maximumLengthSubstring(char* s) {
    int ans = 0;
    int c[128] = {};
    memset(c, 0, 128);
    int left = 0;
    for(int right = 0; s[right]; right++) {
        //1
        c[s[right]]++;
        //2
        while(c[s[right]] > 2) {
            c[s[left]]--;
            left++;
        }
        ans = max(ans, right - left + 1);
    }
    return ans;
}