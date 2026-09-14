// 题目: 包含所有三种字符的子字符串数目
// 题号: 1460
// 语言: c
// 提交时间: 2026-09-09 12:30:04

int numberOfSubstrings(char* s) {
    int num = 0;
    int cnt[3] = {0};
    int left = 0;
    for(int right = 0; right < strlen(s); right++) {
        //右
        cnt[s[right] - 'a']++;
        //左
        while(cnt[0] > 0 && cnt[1] > 0 && cnt[2] > 0 && left <= right) {
            cnt[s[left] - 'a']--;
            left++;
        }
        num += left; //ans += left 是统计右端点固定的情况下，有多少个左端点可以让子串合法。
    }
    return num;
}