// 题目: 无重复字符的最长子串
// 题号: 3
// 语言: c
// 提交时间: 2026-08-06 05:06:02

#define max(a, b) ((a > b) ? a : b)

int lengthOfLongestSubstring(char* s) {
    //因为ASCII一共只有128个，所以可以不用哈希表，而是直接创建一个有128个元素的数组来统计每个字符出现个数
    int c[128] = {};
    memset(c, 0, 128);
    int length = 0;
    int left = 0;
    for(int right = 0; s[right]; right++) {
        c[s[right]]++;
        while(c[s[right]] > 1){
            c[s[left]]--;
            left++;
        }
        length = max(length, right - left + 1);
    }
    return length;
}