#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

inline std::wstring quote_arg(const std::wstring& arg) {
    if (arg.empty()) {
        return L"\"\"";
    }
    if (arg.find_first_of(L" \t\"") == std::wstring::npos) {
        return arg;
    }
    std::wstring out = L"\"";
    for (wchar_t ch : arg) {
        if (ch == L'"') {
            out += L'\\';
        }
        out += ch;
    }
    out += L'"';
    return out;
}

inline std::wstring utf8_to_wide(const std::string& text) {
    if (text.empty()) {
        return {};
    }
    int size = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
    std::wstring out(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), out.data(), size);
    return out;
}

inline std::string wide_to_utf8(const std::wstring& text) {
    if (text.empty()) {
        return {};
    }
    int size = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);
    std::string out(size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), out.data(), size, nullptr, nullptr);
    return out;
}

inline std::string read_text_file(const std::string& path, std::string& error) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        error = "не удалось прочитать " + path;
        return {};
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

inline bool write_text_file(const std::string& path, const std::string& data, std::string& error) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        error = "не удалось записать " + path;
        return false;
    }
    out << data;
    if (!out) {
        error = "не удалось записать " + path;
        return false;
    }
    return true;
}

inline std::size_t json_key_pos(std::string_view text, std::string_view key) {
    const std::string pattern = "\"" + std::string(key) + "\"";
    std::size_t key_at = text.find(pattern);
    if (key_at == std::string_view::npos) {
        return std::string_view::npos;
    }
    return text.find(':', key_at + pattern.size());
}

inline std::optional<std::string> json_string(std::string_view text, std::string_view key) {
    std::size_t colon = json_key_pos(text, key);
    if (colon == std::string_view::npos) {
        return std::nullopt;
    }
    std::size_t open = text.find('"', colon + 1);
    if (open == std::string_view::npos) {
        return std::nullopt;
    }
    std::size_t close = text.find('"', open + 1);
    if (close == std::string_view::npos) {
        return std::nullopt;
    }
    return std::string(text.substr(open + 1, close - open - 1));
}

inline std::optional<long long> json_number(std::string_view text, std::string_view key) {
    std::size_t colon = json_key_pos(text, key);
    if (colon == std::string_view::npos) {
        return std::nullopt;
    }
    std::size_t pos = colon + 1;
    while (pos < text.size() && (text[pos] == ' ' || text[pos] == '\t' || text[pos] == '\n' || text[pos] == '\r')) {
        ++pos;
    }
    if (pos >= text.size()) {
        return std::nullopt;
    }
    int sign = 1;
    if (text[pos] == '-') {
        sign = -1;
        ++pos;
    }
    if (pos >= text.size() || text[pos] < '0' || text[pos] > '9') {
        return std::nullopt;
    }
    long long value = 0;
    while (pos < text.size() && text[pos] >= '0' && text[pos] <= '9') {
        value = value * 10 + (text[pos] - '0');
        ++pos;
    }
    return value * sign;
}
