#include "BinarySearchTree.hpp"
#include <string>
#include <iomanip>   // std::setw, std::setprecision
#include <algorithm> // std::max

#ifdef _WIN32
#include <windows.h>
#endif

// ==================== ПРОВЕРКА РАБОТЫ ====================

void check(const std::string& name, bool ok) {
    std::cout << (ok ? "  OK      " : "  ОШИБКА  ") << name << "\n";
}

void checkInOrder(const std::string& name,
                  const BinarySearchTree<int>& tree,
                  const std::vector<int>& expected) {
    std::vector<int> actual = tree.inOrder();
    std::cout << (actual == expected ? "  OK      " : "  ОШИБКА  ") << name << ": ";
    for (int v : actual) std::cout << v << " ";
    if (actual != expected) {
        std::cout << "  (ожидали: ";
        for (int v : expected) std::cout << v << " ";
        std::cout << ")";
    }
    std::cout << "\n";
}

// Дерево из шапки BinarySearchTree.hpp:
//            8
//          /   \       <- вставляем в порядке 8 3 10 1 6 14 4 7 13
//         3     10
//        / \      \    (порядок важен: от него зависит форма дерева)
//       1   6      14
//          / \     /
//         4   7   13
void fillExample(BinarySearchTree<int>& tree) {
    for (int v : {8, 3, 10, 1, 6, 14, 4, 7, 13}) tree.insert(v);
}

void testInsert() {
    std::cout << "=========== ДОБАВЛЕНИЕ ===========\n\n";
    BinarySearchTree<int> tree;
    fillExample(tree);
    tree.print();
    std::cout << "\n";

    checkInOrder("inOrder отсортирован ", tree, {1, 3, 4, 6, 7, 8, 10, 13, 14});
    check("size == 9", tree.size() == 9);
    check("height == 4", tree.height() == 4);
    check("дубликат 6 не вставляется", !tree.insert(6) && tree.size() == 9);

    BinarySearchTree<int> chain;
    for (int i = 1; i <= 5; ++i) chain.insert(i);
    check("вставка 1..5 по порядку -> вырожденное дерево, height == 5",
          chain.height() == 5);
    std::cout << "\n";
}

void testSearch() {
    std::cout << "=========== ПОИСК ===========\n\n";
    BinarySearchTree<int> tree;
    check("в пустом дереве ничего нет", !tree.contains(1));

    fillExample(tree);
    check("находит корень 8", tree.contains(8));
    check("находит лист 13", tree.contains(13));
    check("находит внутренний 6", tree.contains(6));
    check("не находит 5", !tree.contains(5));
    check("не находит 100", !tree.contains(100));
    check("не находит 0", !tree.contains(0));
    std::cout << "\n";
}

void testRemove() {
    std::cout << "=========== УДАЛЕНИЕ ===========\n\n";
    BinarySearchTree<int> tree;
    fillExample(tree);

    check("удаление отсутствующего 5 -> false", !tree.remove(5));

    check("лист 4 удалён", tree.remove(4));
    checkInOrder("после удаления 4     ", tree, {1, 3, 6, 7, 8, 10, 13, 14});

    check("10 (один ребёнок) удалён", tree.remove(10));
    checkInOrder("после удаления 10    ", tree, {1, 3, 6, 7, 8, 13, 14});

    check("3 (два ребёнка) удалён", tree.remove(3));
    checkInOrder("после удаления 3     ", tree, {1, 6, 7, 8, 13, 14});

    check("корень 8 удалён", tree.remove(8));
    checkInOrder("после удаления 8     ", tree, {1, 6, 7, 13, 14});
    check("8 больше не находится", !tree.contains(8));
    check("size == 5", tree.size() == 5);

    std::cout << "\nДерево сейчас:\n";
    tree.print();
    std::cout << "\n";

    for (int v : {1, 6, 7, 13, 14}) tree.remove(v);
    check("удалили всё -> пустое", tree.empty() && tree.inOrder().empty());

    tree.insert(42);
    check("вставка после опустошения", tree.contains(42) && tree.size() == 1);
    std::cout << "\n";
}

// Печатает статистику одного поиска и сверяет с ожидаемой
void checkStats(const BinarySearchTree<int>& tree, int value,
                bool found, size_t visited, size_t comparisons) {
    auto s = tree.searchWithStats(value);
    bool ok = s.found == found && s.visited == visited && s.comparisons == comparisons;
    std::cout << (ok ? "  OK      " : "  ОШИБКА  ")
              << "поиск " << std::setw(2) << value << ": "
              << (s.found ? "найден   " : "не найден")
              << ", узлов = " << s.visited
              << ", сравнений = " << s.comparisons << "\n";
}

// Вставляет lo..hi так, чтобы дерево получилось сбалансированным:
// сначала середина (станет корнем), потом середины левой и правой половин
void insertBalanced(BinarySearchTree<int>& tree, int lo, int hi) {
    if (lo > hi) return;
    int mid = lo + (hi - lo) / 2;
    tree.insert(mid);
    insertBalanced(tree, lo, mid - 1);
    insertBalanced(tree, mid + 1, hi);
}

// Ищет каждое значение 1..n и печатает максимум и среднее число посещённых узлов
void printSearchCost(const std::string& name, const BinarySearchTree<int>& tree, int n) {
    size_t maxVisited = 0, totalVisited = 0, totalComparisons = 0;
    for (int v = 1; v <= n; ++v) {
        auto s = tree.searchWithStats(v);
        maxVisited = std::max(maxVisited, s.visited);
        totalVisited += s.visited;
        totalComparisons += s.comparisons;
    }
    // Название на отдельной строке: std::setw считает байты, а кириллица
    // в UTF-8 занимает 2 байта на букву, и колонки бы разъехались
    std::cout << "  " << name << ":\n"
              << "    высота = " << tree.height()
              << ", узлов: макс = " << maxVisited
              << ", в среднем = " << std::fixed << std::setprecision(1)
              << double(totalVisited) / n
              << ", сравнений в среднем = " << double(totalComparisons) / n << "\n";
}

void testSearchStats() {
    std::cout << "=========== ПОИСК С ПОДСЧЁТОМ ОПЕРАЦИЙ ===========\n\n";

    // Пример: дерево из шапки, путь к 7 = 8 -> 3 -> 6 -> 7
    //   8: 7 < 8                    -> 1 сравнение
    //   3: 7 < 3 нет, 3 < 7 да      -> 2
    //   6: 7 < 6 нет, 6 < 7 да      -> 2
    //   7: 7 < 7 нет, 7 < 7 нет     -> 2 (равны, нашли)   итого 7
    BinarySearchTree<int> tree;
    fillExample(tree);
    checkStats(tree, 8, true, 1, 2);   // корень: сразу нашли
    checkStats(tree, 7, true, 4, 7);
    checkStats(tree, 13, true, 4, 7);  // 8 -> 10 -> 14 -> 13
    checkStats(tree, 5, false, 4, 6);  // 8 -> 3 -> 6 -> 4 -> nullptr
    checkStats(tree, 0, false, 3, 3);  // 8 -> 3 -> 1 -> nullptr, всё время влево

    std::cout << "Пустое дерево:\n";
    BinarySearchTree<int> empty;
    checkStats(empty, 1, false, 0, 0);
    std::cout << "\n";

    // Одни и те же числа 1..1023, разный порядок вставки
    const int n = 1023;
    BinarySearchTree<int> balanced;
    insertBalanced(balanced, 1, n);
    BinarySearchTree<int> chain;
    for (int v = 1; v <= n; ++v) chain.insert(v);

    std::cout << "Ищем каждое число от 1 до " << n << ":\n";
    printSearchCost("сбалансированное", balanced, n);
    printSearchCost("вырожденное", chain, n);

    check("сбалансированное: любой поиск <= 10 узлов (log2 1024 = 10)",
          balanced.searchWithStats(n).visited == 10);
    check("вырожденное: поиск 1023 проходит все 1023 узла",
          chain.searchWithStats(n).visited == 1023);
    std::cout << "\n";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    testInsert();
    testSearch();
    testRemove();
    testSearchStats();
    return 0;
}
