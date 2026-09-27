#include "BinaryTree.hpp"

// ==================== ПОШАГОВАЯ ОТЛАДКА ====================
// Показывает, что происходит внутри insert и remove на каждом шаге.

int main() {
    BinaryTree<int> tree;

    // Строим дерево 1..6 молча, чтобы не засорять вывод
    for (int i = 1; i <= 6; ++i) tree.insert(i);
    std::cout << "Исходное дерево: ";
    tree.printLevelOrder();

    tree.setDebug(true);   // --- дальше каждый шаг печатается ---

    tree.insert(7);        // должен встать справа от 3
    tree.remove(2);        // на место 2 встанет 7 (последний узел)
    tree.remove(100);      // такого нет

    tree.setDebug(false);  // --- отладка выключена ---

    std::cout << "\nИтоговое дерево: ";
    tree.printLevelOrder();
    return 0;
}
