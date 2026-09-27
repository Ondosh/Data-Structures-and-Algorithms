#include "BinaryTree.hpp"

// ==================== ПРОВЕРКА РАБОТЫ ====================

int main() {
    BinaryTree<int> tree;

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
    std::cout << "Размер: " << tree.size() << "\n";

    return 0;
}
