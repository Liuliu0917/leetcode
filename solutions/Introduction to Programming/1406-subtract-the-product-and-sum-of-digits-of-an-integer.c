// 题目: 整数的各位积和之差
// 题号: 1406
// 语言: c
// 提交时间: 2026-07-27 09:58:08

int subtractProductAndSum(int n) {
    int mulres = 1; //*n最小为1，不用讨论n = 0 的情况
    int addres = 0;
    //1 <= n <= 10^5
    int temp = 0;
    while(n > 0) {
        temp = n % 10;
        n /= 10;
        mulres *= temp;
        addres += temp;
    }
    return mulres - addres;
}