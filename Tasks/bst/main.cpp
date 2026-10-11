#include "BinarySearchTree.hpp"
#include <string>

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

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    testInsert();
    testSearch();
    testRemove();
    return 0;
}
