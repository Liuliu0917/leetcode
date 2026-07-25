// 题目: 最小偶倍数
// 题号: 2491
// 语言: c
// 提交时间: 2025-12-19 10:25:26

int smallestEvenMultiple(int n) {
    if(n%2 == 0)
    {
        return n;
    }else
    {
        return n*2;
    }
}