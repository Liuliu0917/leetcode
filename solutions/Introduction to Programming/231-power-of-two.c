// 题目: 2 的幂
// 题号: 231
// 语言: c
// 提交时间: 2026-07-27 10:45:27

bool isPowerOfTwo(int n) {
    /*int PowerOfTwo = 1;
    while(n >= PowerOfTwo) {
        if(n == PowerOfTwo) {
            return true;
        }
        PowerOfTwo *= 2;
    }
    return false;
    问题：当n = 1073741825时PowerOfTwo超出了int能表示的数字范围*/
    
    //2的幂次方即：n与参照幂次方的差不能小于参照幂次方
    int PowerOfTwo = 1;
    if(n <= 0){
        return false;
    }
    if(n == 1) {
        return true;
    }
    while(n - PowerOfTwo >= PowerOfTwo) {
        PowerOfTwo *= 2;
        if(n == PowerOfTwo) {
            return true;
        }
    }
    return false;
}