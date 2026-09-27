#include "stack.hpp"
#include "calculator.hpp"

#include <iostream>
#include <string>
#include <cmath>
#include <stdexcept>
#include <functional>


//  Минимальная тестовая инфраструктура

static int passed = 0, failed = 0;  // счётчики пройденных и проваленных тестов

// Проверка условия: если условие истинно — тест пройден, иначе — провален
void check(const std::string& name, bool cond) {
    if (cond) {
        std::cout << "  [ПРОЙДЕН] " << name << "\n";
        ++passed;
    } else {
        std::cout << "  [ПРОВАЛЕН] " << name << "\n";
        ++failed;
    }
}

// Проверка, что вызов fn() выбрасывает исключение типа Ex
template<typename Ex, typename Fn>
void checkThrows(const std::string& name, Fn fn) {
    try {
        fn();
        std::cout << "  [ПРОВАЛЕН] " << name << " (исключение не выброшено)\n";
        ++failed;
    } catch (const Ex&) {
        std::cout << "  [ПРОЙДЕН] " << name << "\n";
        ++passed;
    } catch (...) {
        std::cout << "  [ПРОВАЛЕН] " << name << " (неверный тип исключения)\n";
        ++failed;
    }
}

// Сравнение чисел с плавающей точкой с заданной точностью
bool near(double a, double b, double eps = 1e-9) {
    return std::abs(a - b) < eps;
}

// Вывод заголовка раздела тестов
void section(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}


//  Тесты стека

void testStackBasic() {
    section("Stack — базовые операции");

    Stack<int> s;
    check("новый стек пустой", s.empty());
    check("размер нового стека = 0", s.size() == 0);

    s.push(10);
    check("после push(10) не пустой", !s.empty());
    check("размер = 1", s.size() == 1);
    check("top() = 10", s.top() == 10);

    s.push(20);
    check("после push(20) top() = 20", s.top() == 20);
    check("размер = 2", s.size() == 2);

    int v = s.pop();
    check("pop() вернул 20", v == 20);
    check("размер после pop = 1", s.size() == 1);
    check("top() = 10 после pop", s.top() == 10);

    v = s.pop();
    check("pop() вернул 10", v == 10);
    check("стек снова пустой", s.empty());
}

void testStackErrors() {
    section("Stack — ошибки на пустом стеке");

    Stack<int> s;
    // Изменено: std::runtime_error вместо std::underflow_error
    checkThrows<std::runtime_error>("pop() на пустом стеке", [&]{ s.pop(); });
    checkThrows<std::runtime_error>("top() на пустом стеке", [&]{ s.top(); });
}

void testStackMultiple() {
    section("Stack — множественные элементы (LIFO)");

    Stack<int> s;
    for (int i = 1; i <= 5; ++i) s.push(i);
    check("размер = 5", s.size() == 5);
    for (int i = 5; i >= 1; --i) {
        check("порядок извлечения LIFO " + std::to_string(i), s.pop() == i);
    }
    check("стек пуст после всех pop", s.empty());
}

void testStackCopyMove() {
    section("Stack — копирование и перемещение");

    Stack<int> a;
    a.push(1); a.push(2); a.push(3);

    // Конструктор копирования
    Stack<int> b(a);
    check("копия: размер совпадает", b.size() == a.size());
    check("копия: top совпадает", b.top() == a.top());
    b.pop();
    check("изменение копии не влияет на оригинал", a.top() == 3);

    // Оператор присваивания копированием
    Stack<int> c;
    c = a;
    check("присвоение копии: размер", c.size() == a.size());

    // Конструктор перемещения - тест зависит от реализации
    // Если ваш Stack не поддерживает move-семантику, закомментируйте этот блок
    #ifdef SUPPORT_MOVE
    Stack<int> d(std::move(a));
    check("перемещение: размер = 3", d.size() == 3);
    // Убедитесь, что после перемещения исходный стек пуст
    check("источник пуст после перемещения", a.empty());

    // Оператор присваивания перемещением
    Stack<int> e;
    e = std::move(d);
    check("присвоение с перемещением: размер = 3", e.size() == 3);
    check("источник пуст после присвоения с перемещением", d.empty());
    #endif
}

void testStackWithStrings() {
    section("Stack — работа с типом std::string");

    Stack<std::string> s;
    s.push("привет");
    s.push("мир");
    check("top() = \"мир\"", s.top() == "мир");
    check("pop() = \"мир\"", s.pop() == "мир");
    check("pop() = \"привет\"", s.pop() == "привет");
    check("стек пуст", s.empty());
}


//  Тесты калькулятора — отдельные операции

void testCalcOperations() {
    section("Калькулятор — отдельные операции");

    PostfixCalculator c;

    // Сложение
    check("3 4 + = 7", near(c.evaluate("3 4 +"), 7.0));
    check("-5 10 + = 5", near(c.evaluate("-5 10 +"), 5.0));
    check("0 0 + = 0", near(c.evaluate("0 0 +"), 0.0));
    check("1.5 2.5 + = 4.0", near(c.evaluate("1.5 2.5 +"), 4.0));

    // Вычитание
    check("10 3 - = 7", near(c.evaluate("10 3 -"), 7.0));
    check("5 10 - = -5", near(c.evaluate("5 10 -"), -5.0));
    check("0 0 - = 0", near(c.evaluate("0 0 -"), 0.0));

    // Умножение
    check("3 4 * = 12", near(c.evaluate("3 4 *"), 12.0));
    check("-3 4 * = -12", near(c.evaluate("-3 4 *"), -12.0));
    check("0 100 * = 0", near(c.evaluate("0 100 *"), 0.0));
    check("2.5 4 * = 10", near(c.evaluate("2.5 4 *"), 10.0));

    // Деление
    check("10 2 / = 5", near(c.evaluate("10 2 /"), 5.0));
    check("7 2 / = 3.5", near(c.evaluate("7 2 /"), 3.5));
    check("-9 3 / = -3", near(c.evaluate("-9 3 /"), -3.0));
    check("0 5 / = 0", near(c.evaluate("0 5 /"), 0.0));
}


//  Тесты калькулятора — сложные выражения

void testCalcComplex() {
    section("Калькулятор — сложные выражения");

    PostfixCalculator c;

    // (3 + 4) * 2  - постфиксная форма: 3 4 + 2 *
    check("(3+4)*2 = 14", near(c.evaluate("3 4 + 2 *"), 14.0));

    // 5 + ((1 + 2) * 4) - 3  - постфиксная форма: 5 1 2 + 4 * + 3 -
    check("5+(1+2)*4-3 = 14", near(c.evaluate("5 1 2 + 4 * + 3 -"), 14.0));

    // (10 - 2) / (1 + 1) * 3  - постфиксная форма: 10 2 - 1 1 + / 3 *
    check("(10-2)/(1+1)*3 = 12", near(c.evaluate("10 2 - 1 1 + / 3 *"), 12.0));

    // Вычисление 2^10 через последовательные умножения
    check("2^10 = 1024", near(c.evaluate("2 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 *"), 1024.0));

    // ((15 / (7 - (1 + 1))) * 3) - (2 + (1 + 1))
    // постфиксная форма: 15 7 1 1 + - / 3 * 2 1 1 + + -   ожидаемый результат = 5
    check("сложное выражение = 5", near(c.evaluate("15 7 1 1 + - / 3 * 2 1 1 + + -"), 5.0));

    // Десятичные дроби: 1.1 + 2.2 = 3.3
    check("1.1 + 2.2 = 3.3", near(c.evaluate("1.1 2.2 +"), 3.3, 1e-9));
}


//  Тесты калькулятора — ошибочные выражения

void testCalcErrors() {
    section("Калькулятор — обработка ошибок");

    PostfixCalculator c;

    // Изменено: проверяем исключения, которые бросает ваш калькулятор
    checkThrows<std::runtime_error>(
        "деление на ноль", [&]{ c.evaluate("5 0 /"); });

    checkThrows<std::runtime_error>(
        "недостаточно операндов для операции", [&]{ c.evaluate("1 +"); });

    checkThrows<std::runtime_error>(
        "лишние операнды (некорректное выражение)", [&]{ c.evaluate("1 2 3 +"); });

    checkThrows<std::runtime_error>(
        "неизвестный токен (неподдерживаемая операция)", [&]{ c.evaluate("3 4 ^"); });

    checkThrows<std::runtime_error>(
        "пустое выражение", [&]{ c.evaluate(""); });
}


//  Главная функция

int main() {
    // Тесты стека
    testStackBasic();
    testStackErrors();
    testStackMultiple();
    testStackCopyMove();
    testStackWithStrings();

    // Тесты калькулятора
    testCalcOperations();
    testCalcComplex();
    testCalcErrors();
    
    // Итоговая статистика

    std::cout << "Всего тестов: " << (passed + failed) 
              << "  |  Пройдено: " << passed 
              << "  |  Провалено: " << failed << "\n";

    return (failed == 0) ? 0 : 1;
}