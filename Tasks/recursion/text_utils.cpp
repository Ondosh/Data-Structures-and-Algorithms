#include "text_utils.hpp"
#include <fstream>
#include <sstream>

// Итеративный подход

std::size_t count_digits_iterative(const std::string& text) {
    std::size_t count = 0;

    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '.') break;          // Точка — конец текста по условию задания
        if (text[i] >= '0' && text[i] <= '9') {
            ++count;
        }
    }

    return count;
}


// Рекурсивный подход
//
// Идея: задача «посчитать цифры с позиции pos до конца» сводится к:
//   (1 если символ pos — цифра, иначе 0) + (задача с позиции pos+1)
// База рекурсии: дошли до конца строки или встретили точку → 0.

std::size_t count_digits_recursive(const std::string& text, std::size_t pos) {
    // ── База рекурсии ──────────────────────────────────────────
    if (pos >= text.size() || text[pos] == '.') {
        return 0;
    }

    // ── Рекурсивный шаг ───────────────────────────────────────
    // Вклад текущего символа (1 или 0) + результат для хвоста строки
    std::size_t current = (text[pos] >= '0' && text[pos] <= '9') ? 1 : 0;
    return current + count_digits_recursive(text, pos + 1);
}


// Чтение файла

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
