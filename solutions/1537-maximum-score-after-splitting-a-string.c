// 题目: 分割字符串的最大得分
// 题号: 1537
// 语言: c
// 提交时间: 2026-07-29 07:01:50

#include <string.h>
int maxScore(char* s) {
    int n = strlen(s);
    int score = 0;
    for(int i = 1; i < n; i++) {  
    //这个分割点（数组下标）（算是右侧字符串的第一位）的范围是[1, n-1]，所以范围写作 int i = 1; i < n;
        int left = 0, right = 0;
        for(int j = 0; j < i; j++) {
            if(s[j] == '0') {
                left++;
            }
        }
        for(int k = i; k < n; k++) {
            if(s[k] == '1') {
                right++;
            }
        }
        int temscore = left + right;
        //printf("%d\n", temscore);
        if(temscore > score){
            score = temscore;
        }
    }
    return score;
}