#include "BinaryTree.hpp"
#include <string>

// ==================== ПРОВЕРКА РАБОТЫ ====================

// Сравнивает результат обхода с ожидаемым и печатает OK / ОШИБКА
void check(const std::string& name,
           const std::vector<int>& actual,
           const std::vector<int>& expected) {
    std::cout << (actual == expected ? "  OK      " : "  ОШИБКА  ") << name << ": ";
    for (int v : actual) std::cout << v << " ";
    if (actual != expected) {
        std::cout << "  (ожидали: ";
        for (int v : expected) std::cout << v << " ";
        std::cout << ")";
    }
    std::cout << "\n";
}

// ---------------- ЗАДАЧА 1: вставка и удаление ----------------
void testInsertRemove() {
    std::cout << "=========== ЗАДАЧА 1: вставка и удаление ===========\n\n";
    BinaryTree<int> tree;

    tree.setDebug(true); // включили дебаг

    std::cout << "1) Пустое дерево:\n";
    tree.printLevelOrder(); // ожидаем: (дерево пустое)
    std::cout << "Размер: " << tree.size() << "\n\n";

    std::cout << "2) Добавляем 1..7 по уровням:\n";
    for (int i = 1; i <= 7; ++i) tree.insert(i);
    tree.printLevelOrder(); // ожидаем: 1 2 3 4 5 6 7
    std::cout << "Размер: " << tree.size() << "\n\n";

    std::cout << "3) Удаляем 3 (узел из середины):\n";
    tree.remove(3);
    tree.printLevelOrder(); // ожидаем: 1 2 7 4 5 6  (на место 3 встала 7)
    std::cout << "Размер: " << tree.size() << "\n\n";

    std::cout << "4) Удаляем 6 (сейчас это последний узел):\n";
    tree.remove(6);
    tree.printLevelOrder(); // ожидаем: 1 2 7 4 5
    std::cout << "Размер: " << tree.size() << "\n\n";

    std::cout << "5) Удаляем 1 (корень):\n";
    tree.remove(1);
    tree.printLevelOrder(); // ожидаем: 5 2 7 4  (на место корня встала 5)
    std::cout << "Размер: " << tree.size() << "\n\n";

    std::cout << "6) Удаляем 100 (такого значения нет):\n";
    std::cout << "remove вернул: " << (tree.remove(100) ? "true" : "false") << "\n"; // ожидаем: false
    tree.printLevelOrder(); // ожидаем: 5 2 7 4 (без изменений)
    std::cout << "Размер: " << tree.size() << "\n\n";

    std::cout << "7) Удаляем оставшиеся 4, 7, 2, 5 до пустого дерева:\n";
    tree.remove(4);
    tree.remove(7);
    tree.remove(2);
    tree.remove(5);
    tree.printLevelOrder(); // ожидаем: (дерево пустое)
    std::cout << "Размер: " << tree.size() << "\n\n";

    std::cout << "8) Добавляем после опустошения:\n";
    tree.insert(10);
    tree.insert(20);
    tree.printLevelOrder(); // ожидаем: 10 20
    std::cout << "Размер: " << tree.size() << "\n\n";
}

// ---------------- ЗАДАЧА 2: обходы ----------------
void testTraversals() {
    std::cout << "=========== ЗАДАЧА 2: обходы ===========\n\n";

    // Полное дерево:      1
    //                   /   \   (у всех внутренних узлов по два ребёнка)
    //                  2     3
    //                 / \   / \   (нижний уровень заполнен полностью)
    //                4   5 6   7
    {
        BinaryTree<int> tree;
        for (int i = 1; i <= 7; ++i) tree.insert(i);
        std::cout << "Дерево 1..7 (полное):\n";
        check("левый прямой       ", tree.preOrderLeft(),      {1, 2, 4, 5, 3, 6, 7});
        check("левый обратный     ", tree.postOrderLeft(),     {4, 5, 2, 6, 7, 3, 1});
        check("правый прямой      ", tree.preOrderRight(),     {1, 3, 7, 6, 2, 5, 4});
        check("правый обратный    ", tree.postOrderRight(),    {7, 6, 3, 5, 4, 2, 1});
        check("в ширину           ", tree.levelOrder(),        {1, 2, 3, 4, 5, 6, 7});
        check("обратный в ширину  ", tree.reverseLevelOrder(), {4, 5, 6, 7, 2, 3, 1});
        std::cout << "\n";
    }

    // Неполное дерево:    1
    //                   /   \   (у узла 3 нет правого ребёнка)
    //                  2     3
    //                 / \   /
    //                4   5 6
    {
        BinaryTree<int> tree;
        for (int i = 1; i <= 6; ++i) tree.insert(i);
        std::cout << "Дерево 1..6 (у 3 нет правого ребёнка):\n";
        check("левый прямой       ", tree.preOrderLeft(),      {1, 2, 4, 5, 3, 6});
        check("левый обратный     ", tree.postOrderLeft(),     {4, 5, 2, 6, 3, 1});
        check("правый прямой      ", tree.preOrderRight(),     {1, 3, 6, 2, 5, 4});
        check("правый обратный    ", tree.postOrderRight(),    {6, 3, 5, 4, 2, 1});
        check("в ширину           ", tree.levelOrder(),        {1, 2, 3, 4, 5, 6});
        check("обратный в ширину  ", tree.reverseLevelOrder(), {4, 5, 6, 2, 3, 1});
        std::cout << "\n";
    }

    // Один узел
    {
        BinaryTree<int> tree;
        tree.insert(42);
        std::cout << "Дерево из одного узла:\n";
        check("левый прямой       ", tree.preOrderLeft(),      {42});
        check("левый обратный     ", tree.postOrderLeft(),     {42});
        check("правый прямой      ", tree.preOrderRight(),     {42});
        check("правый обратный    ", tree.postOrderRight(),    {42});
        check("в ширину           ", tree.levelOrder(),        {42});
        check("обратный в ширину  ", tree.reverseLevelOrder(), {42});
        std::cout << "\n";
    }

    // Пустое дерево — все обходы возвращают пустой вектор
    {
        BinaryTree<int> tree;
        std::cout << "Пустое дерево:\n";
        check("левый прямой       ", tree.preOrderLeft(),      {});
        check("левый обратный     ", tree.postOrderLeft(),     {});
        check("правый прямой      ", tree.preOrderRight(),     {});
        check("правый обратный    ", tree.postOrderRight(),    {});
        check("в ширину           ", tree.levelOrder(),        {});
        check("обратный в ширину  ", tree.reverseLevelOrder(), {});
        std::cout << "\n";
    }
}

int main() {
    testInsertRemove();
    testTraversals();
    return 0;
}
