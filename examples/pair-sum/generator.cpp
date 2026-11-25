// Внешний генератор по контракту из задания:
//   generator --seed 42 --case 2381 --size 100
// тест в stdout, диагностика в stderr, ненулевой код — ошибка генерации.
//
// Формат теста:
//   n s
//   a1 a2 ... an
//
// На каждом 7-м тесте последний элемент обязан входить в подходящую пару.
// Именно такие тесты ловит баг в solution.cpp. Остальные тесты с ним согласны.
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Args {
    std::uint64_t seed = 1;
    std::uint64_t test_case = 1;
    int size = 8;
};

std::uint64_t parse_u64(const char* text) {
    return std::strtoull(text, nullptr, 10);
}

Args parse_args(int argc, char** argv) {
    Args args;
    for (int i = 1; i < argc; ++i) {
        const std::string key = argv[i];
        if (i + 1 >= argc) {
            break;
        }
        if (key == "--seed") {
            args.seed = parse_u64(argv[++i]);
        } else if (key == "--case") {
            args.test_case = parse_u64(argv[++i]);
        } else if (key == "--size") {
            args.size = std::atoi(argv[++i]);
        }
    }
    if (args.size < 2) {
        args.size = 2;
    }
    if (args.size > 200) {
        args.size = 200;
    }
    return args;
}

class Lcg {
public:
    explicit Lcg(std::uint64_t state) : state_(state ? state : 1) {}

    std::uint64_t next() {
        state_ = state_ * 6364136223846793005ull + 1;
        return state_;
    }

    int uniform(int lo, int hi) {
        return lo + static_cast<int>(next() % static_cast<std::uint64_t>(hi - lo + 1));
    }

private:
    std::uint64_t state_;
};

}  // namespace

int main(int argc, char** argv) {
    const Args args = parse_args(argc, argv);
    Lcg rng(args.seed * 1000003ull + args.test_case);

    const int n = args.size;
    const bool trap_last = (args.test_case % 7 == 0);
    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = rng.uniform(0, 30);
    }

    // Пара внутри префикса (без последнего элемента): её видят и эталон, и решение.
    int left = 0;
    int right = 1;
    if (n >= 3) {
        left = rng.uniform(0, n - 2);
        right = rng.uniform(0, n - 2);
        if (left == right) {
            right = (right + 1) % (n - 1);
        }
    }
    const int s = a[left] + a[right];
    if (trap_last) {
        // Дополнительная пара с последним элементом. Её решение теряет.
        a[n - 1] = s - a[0];
    } else {
        a[n - 1] = 1'000'000;
    }

    std::cout << n << ' ' << s << '\n';
    for (int i = 0; i < n; ++i) {
        if (i) {
            std::cout << ' ';
        }
        std::cout << a[i];
    }
    std::cout << '\n';
    std::cerr << "generated case " << args.test_case << " trap_last=" << (trap_last ? 1 : 0) << '\n';
    return 0;
}
