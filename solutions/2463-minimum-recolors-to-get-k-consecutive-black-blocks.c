// 题目: 得到 K 个黑块的最少涂色次数
// 题号: 2463
// 语言: c
// 提交时间: 2026-08-04 04:33:35

#include <string.h>

#define max(a, b) ((a > b) ? a : b);

int minimumRecolors(char* blocks, int k) {
    int ans = 0;
    int black = 0;
    for(int i = 0; i < strlen(blocks); i++) {
        //1
        if(blocks[i] == 'B') {
            black ++;
        }
        
        //滑块长度设定为k
        int left = i - k + 1;
        if(left < 0){
            continue;
        }

        //2
        ans = max(ans, black);

        //3
        if(blocks[left] == 'B') {
            black --;
        }

    }

    return k - ans;
}