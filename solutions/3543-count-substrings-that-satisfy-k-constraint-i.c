// 题目: 统计满足 K 约束的子字符串数量 I
// 题号: 3543
// 语言: c
// 提交时间: 2026-09-09 02:47:23

int countKConstraintSubstrings(char* s, int k) {
    int num = 0;
    char cnt[2] = {};
    int left = 0;
    for(int right = 0; right < strlen(s); right++) {
        cnt[s[right] - '0']++;
        while(cnt[0] > k && cnt[1] > k && left < right) {  //任一条件 二选一即可
            cnt[s[left] - '0']--;
            left++;
        }
        num += right - left + 1;
    }
    return num;
}