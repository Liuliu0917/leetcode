// 题目: 丑数
// 题号: 263
// 语言: c
// 提交时间: 2026-07-27 11:33:02

bool isUgly(int n) {
    if(n <= 0) {
        return false;
    }
    while(n % 2 == 0) {
        n /= 2;
    }
    while(n % 3 == 0) {
        n /= 3;
    }
    while(n % 5 == 0) {
        n /= 5;
    }
    return n == 1;
}