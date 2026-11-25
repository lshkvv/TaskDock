// Эталон: число неупорядоченных пар i < j с a[i] + a[j] == s.
// Медленный полный перебор — ему можно доверять на коротких тестах.
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
        for (int j = i + 1; j < n; ++j) {
            if (a[i] + a[j] == s) {
                ++answer;
            }
        }
    }
    std::cout << answer << '\n';
    return 0;
}
