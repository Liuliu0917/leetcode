// 题目: 统计范围内的元音字符串数
// 题号: 2654
// 语言: c
// 提交时间: 2026-07-29 08:09:43

#include <string.h>
int vowelStrings(char** words, int wordsSize, int left, int right) {
    int ans = 0;
    char s[5] = {'a', 'e', 'i', 'o', 'u',};
    for(int i = left; i <= right; i++) {
        int cnt = 0;
        int len = strlen(words[i]);
        for(int j = 0; j < 5; j++){
            if(words[i][0] == s[j]){
                cnt++;
            }
            if(words[i][len - 1] == s[j]){
                cnt++;
            }
        }
        if(cnt == 2) {
            ans++;
        }
    }
    return ans;
}