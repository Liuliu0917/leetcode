// 题目: 定长子串中元音的最大数目
// 题号: 1567
// 语言: c
// 提交时间: 2026-08-02 06:55:20

#define Max(a,b) ((a > b) ? (a) : (b))

int maxVowels(char* s, int k) {
    int ans = 0, vowel = 0;
    for(int i = 0; s[i]; i++){
        //1
        if(s[i] == 'a' || s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u'){
            vowel++;
        }
        int left = i - k + 1; 
        if(left < 0){
            continue;
        }

        //2
        ans = Max(ans, vowel);
        if(ans == k){
            break;
        }

        //3
        if(s[left] == 'a' || s[left] == 'e' || s[left] == 'i' || s[left] == 'o' || s[left] == 'u'){
            vowel--;
        }
    }
    return ans;
}