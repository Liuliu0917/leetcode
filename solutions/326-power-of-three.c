// 题目: 3 的幂
// 题号: 326
// 语言: c
// 提交时间: 2026-07-27 11:03:01

bool isPowerOfThree(int n) {
    if(n == 1) return true;
    if(n == 0) return false;
    if(n % 3 != 0) return false;
    return isPowerOfThree(n /= 3);
}