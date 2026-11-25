// Намеренно неправильное решение для демонстрации TaskDock.
// Внутренний цикл идёт до n - 1, поэтому пара, в которой правый
// элемент стоит последним, не учитывается.
// На cases/ok.in ответы совпадают, на cases/fail.in эталон даёт 3, это решение — 2.
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    int s = 0;
    if (!(std::cin >> n >> s)) {
        return 1;
    }
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        if (!(std::cin >> a[i])) {
            return 1;
        }
    }

    long long answer = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n - 1; ++j) {
            if (a[i] + a[j] == s) {
                ++answer;
            }
        }
    }
    std::cout << answer << '\n';
    return 0;
}
