#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <utility> // std::pair
#include <algorithm> // std::reverse

// =========================================================================
// ЗАДАЧА 1. БИНАРНОЕ ДЕРЕВО (без правила упорядоченности, как в дереве поиска)
// =========================================================================
//
// В отличие от BST, здесь НЕТ правила "меньше — влево, больше — вправо",
// поэтому место для нового узла выбираем сами.
//
// Выбранное правило: вставка ПО УРОВНЯМ (level-order), как в куче.
// Дерево всегда остаётся "полным" (complete tree) — без дырок:
//
//         1
//       /   \  <- у каждого узла не больше двух детей
//      2     3
//     / \   /
//    4   5 6      <-- следующий узел встанет справа от 6
//
// Это же правило пригодится в задачах 4-5 (дерево на массиве, сортировки).
//
// Файл .hpp, потому что класс шаблонный: реализация шаблона должна быть
// видна компилятору целиком, поэтому её не выносят в .cpp.
//
// ОТЛАДКА: tree.setDebug(true) включает пошаговый вывод в insert и remove.
// По умолчанию выключена, чтобы не мешать обычным тестам.

template <typename T>
class BinaryTree {
private:
    // Узел дерева ("лист"/ячейка) со ссылками на левое и правое поддеревья
    struct Node {
        T data;
        Node* left;
        Node* right;

        explicit Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root_;   // корень (nullptr, если дерево пустое)
    size_t size_;  // количество узлов
    bool debug_;   // печатать ли шаги алгоритмов

    // Рекурсивное удаление всех узлов (для деструктора)
    void clearRecursive(Node* node) {
        if (!node) return;
        clearRecursive(node->left);
        clearRecursive(node->right);
        delete node;
    }

    // ------------------------- ОТЛАДОЧНЫЙ ВЫВОД -------------------------
    // Печатает значение узла или "-", если узла нет (nullptr).
    static void printNode(const Node* node) {
        if (node) std::cout << node->data;
        else      std::cout << "-";
    }

    // Печатает содержимое очереди узлов: [2, 3].
    // Очередь принимается ПО ЗНАЧЕНИЮ (копия): у std::queue нет обхода
    // по элементам, поэтому печатаем, опустошая копию. Настоящая очередь
    // при этом не меняется.
    static void printQueue(std::queue<Node*> q) {
        std::cout << "[";
        bool first = true;
        while (!q.empty()) {
            if (!first) std::cout << ", ";
            printNode(q.front());
            q.pop();
            first = false;
        }
        std::cout << "]";
    }

    // То же для очереди пар {родитель, узел}: [{1,2}, {1,3}]
    static void printPairQueue(std::queue<std::pair<Node*, Node*>> q) {
        std::cout << "[";
        bool first = true;
        while (!q.empty()) {
            if (!first) std::cout << ", ";
            std::cout << "{";
            printNode(q.front().first);
            std::cout << ",";
            printNode(q.front().second);
            std::cout << "}";
            q.pop();
            first = false;
        }
        std::cout << "]";
    }

public:
    BinaryTree() : root_(nullptr), size_(0), debug_(false) {}

    ~BinaryTree() {
        clearRecursive(root_);
    }

    // Копирование запрещено: иначе две копии указывали бы на одни и те же
    // узлы, и деструктор удалил бы их дважды (падение программы).
    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void setDebug(bool on) { debug_ = on; }

    // ---------------------------------------------------------------
    // ДОБАВЛЕНИЕ УЗЛА (по уровням, BFS)
    // ---------------------------------------------------------------
    // Идём в ширину от корня и ищем первый узел со свободным местом
    // (left == nullptr или right == nullptr) — туда и вставляем.
    void insert(const T& value) {
        if (debug_) std::cout << "\n[insert " << value << "]\n";

        Node* newNode = new Node(value);
        ++size_;

        if (!root_) {
            root_ = newNode;
            if (debug_) std::cout << "  дерево было пустым -> " << value << " стал корнем\n";
            return;
        }

        std::queue<Node*> q;
        q.push(root_);
        if (debug_) {
            std::cout << "  старт: очередь = ";
            printQueue(q);
            std::cout << "\n";
        }

        int step = 0; // номер итерации — только для вывода
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            ++step;

            if (debug_) {
                std::cout << "  шаг " << step << ": достали " << current->data
                    << " (left = ";
                printNode(current->left);
                std::cout << ", right = ";
                printNode(current->right);
                std::cout << ")\n";
            }

            if (!current->left) {
                current->left = newNode;
                if (debug_) std::cout << "    left свободен -> " << value
                    << " стал ЛЕВЫМ ребёнком " << current->data << "\n";
                return;
            }
            q.push(current->left);
            if (debug_) std::cout << "    left занят -> " << current->left->data
                << " в очередь\n";

            if (!current->right) {
                current->right = newNode;
                if (debug_) std::cout << "    right свободен -> " << value
                    << " стал ПРАВЫМ ребёнком " << current->data << "\n";
                return;
            }
            q.push(current->right);
            if (debug_) {
                std::cout << "    right занят -> " << current->right->data
                    << " в очередь, очередь = ";
                printQueue(q);
                std::cout << "\n";
            }
        }
    }

    // ---------------------------------------------------------------
    // УДАЛЕНИЕ УЗЛА ПО ЗНАЧЕНИЮ
    // ---------------------------------------------------------------
    // Приём из кучи, чтобы дерево осталось полным:
    // 1) находим первый (по уровням) узел с нужным значением;
    // 2) находим ПОСЛЕДНИЙ узел дерева по уровням;
    // 3) переносим данные последнего узла в удаляемый;
    // 4) физически удаляем последний узел.
    // Если удаляемый узел и есть последний — алгоритм тоже работает.
    //
    // Возвращает true, если значение найдено и удалено.
    bool remove(const T& value) {
        if (debug_) std::cout << "\n[remove " << value << "]\n";

        if (!root_) {
            if (debug_) std::cout << "  дерево пустое -> false\n";
            return false;
        }

        // ---- Этап 1: обход в ширину, собираем пары {родитель, узел} ----
        std::vector<std::pair<Node*, Node*>> levelOrder;

        std::queue<std::pair<Node*, Node*>> q;
        q.push({ nullptr, root_ });

        if (debug_) {
            std::cout << "  Этап 1: обход в ширину\n";
            std::cout << "  старт: очередь = ";
            printPairQueue(q);
            std::cout << "\n";
        }

        int step = 0;
        while (!q.empty()) {
            Node* parent = q.front().first;
            Node* node = q.front().second;
            q.pop();
            ++step;
            levelOrder.push_back({ parent, node });

            if (node->left)  q.push({ node, node->left });
            if (node->right) q.push({ node, node->right });

            if (debug_) {
                std::cout << "  шаг " << step << ": достали {";
                printNode(parent);
                std::cout << "," << node->data << "}, записали в levelOrder["
                    << levelOrder.size() - 1 << "], очередь = ";
                printPairQueue(q);
                std::cout << "\n";
            }
        }

        // ---- Этап 2: ищем первый узел с искомым значением ----
        if (debug_) std::cout << "  Этап 2: ищем " << value << "\n";

        Node* targetNode = nullptr;
        for (size_t i = 0; i < levelOrder.size(); ++i) {
            if (levelOrder[i].second->data == value) {
                targetNode = levelOrder[i].second;
                if (debug_) std::cout << "    нашли на индексе " << i << "\n";
                break;
            }
        }

        if (!targetNode) {
            if (debug_) std::cout << "    не нашли -> false, дерево не меняется\n";
            return false;
        }

        // ---- Этап 3: последний узел и его родитель ----
        Node* lastNode = levelOrder.back().second;
        Node* lastParent = levelOrder.back().first;

        if (debug_) {
            std::cout << "  Этап 3: последний узел = " << lastNode->data
                << ", его родитель = ";
            printNode(lastParent);
            std::cout << "\n";
        }

        // ---- Этап 4: переносим значение и удаляем последний узел ----
        if (debug_) std::cout << "  Этап 4: в узел " << targetNode->data
            << " записываем " << lastNode->data << "\n";
        targetNode->data = lastNode->data;

        if (lastParent) {
            if (lastParent->left == lastNode) {
                lastParent->left = nullptr;
                if (debug_) std::cout << "    обнулили left у " << lastParent->data << "\n";
            }
            else {
                lastParent->right = nullptr;
                if (debug_) std::cout << "    обнулили right у " << lastParent->data << "\n";
            }
        }
        else {
            root_ = nullptr; // это был единственный узел (корень)
            if (debug_) std::cout << "    это был единственный узел -> дерево пустое\n";
        }

        delete lastNode;
        --size_;
        if (debug_) std::cout << "    удалили последний узел, размер = " << size_ << "\n";
        return true;
    }

    // =================================================================
    // ЗАДАЧА 2. ОБХОДЫ
    // =================================================================
    // Каждый обход возвращает вектор значений в порядке посещения узлов.
    //
    // Названия:
    //   "прямой"  — сначала КОРЕНЬ, потом поддеревья   (pre-order)
    //   "обратный"— сначала поддеревья, потом КОРЕНЬ   (post-order)
    //   "левый"   — левое поддерево раньше правого
    //   "правый"  — правое поддерево раньше левого
    //
    // Пример для дерева 1..7:
    //         1
    //       /   \  <- все четыре обхода в глубину разобраны ниже
    //      2     3
    //     / \   / \  <- у каждого из 2 и 3 по два ребёнка
    //    4   5 6   7
    //
    // Все обходы в глубину — рекурсивные: публичный метод создаёт пустой
    // вектор и запускает приватную рекурсивную функцию от корня.
    // Параметр depth нужен только для отступов в отладочном выводе.

    // 1. Левый прямой: Корень -> Лево -> Право      1 2 4 5 3 6 7
    std::vector<T> preOrderLeft() const {
        std::vector<T> result;
        if (debug_) std::cout << "\n[левый прямой обход]\n";
        preOrderLeftRec(root_, result, 0);
        return result;
    }

    // 2. Левый обратный: Лево -> Право -> Корень    4 5 2 6 7 3 1
    std::vector<T> postOrderLeft() const {
        std::vector<T> result;
        if (debug_) std::cout << "\n[левый обратный обход]\n";
        postOrderLeftRec(root_, result, 0);
        return result;
    }

    // 3. Правый прямой: Корень -> Право -> Лево     1 3 7 6 2 5 4
    std::vector<T> preOrderRight() const {
        std::vector<T> result;
        if (debug_) std::cout << "\n[правый прямой обход]\n";
        preOrderRightRec(root_, result, 0);
        return result;
    }

    // 4. Правый обратный: Право -> Лево -> Корень   7 6 3 5 4 2 1
    std::vector<T> postOrderRight() const {
        std::vector<T> result;
        if (debug_) std::cout << "\n[правый обратный обход]\n";
        postOrderRightRec(root_, result, 0);
        return result;
    }

    // 5. Обход в ширину: по уровням сверху вниз, слева направо
    //                                               1 2 3 4 5 6 7
    // Та же схема с очередью, что в insert и remove.
    std::vector<T> levelOrder() const {
        std::vector<T> result;
        if (debug_) std::cout << "\n[обход в ширину]\n";
        if (!root_) return result;

        std::queue<Node*> q;
        q.push(root_);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            result.push_back(current->data);
            if (current->left)  q.push(current->left);
            if (current->right) q.push(current->right);

            if (debug_) {
                std::cout << "  посетили " << current->data << ", очередь = ";
                printQueue(q);
                std::cout << "\n";
            }
        }
        return result;
    }

    // 6. Обратный обход в ширину: по уровням СНИЗУ ВВЕРХ,
    //    внутри уровня слева направо                4 5 6 7 2 3 1
    //
    // Приём: делаем обычный обход в ширину, но детей кладём в очередь
    // в порядке "правый, потом левый". Получается 1 3 2 7 6 5 4 —
    // сверху вниз, справа налево. Если этот список развернуть задом
    // наперёд, выйдет снизу вверх, слева направо: 4 5 6 7 2 3 1.
    std::vector<T> reverseLevelOrder() const {
        std::vector<T> result;
        if (debug_) std::cout << "\n[обратный обход в ширину]\n";
        if (!root_) return result;

        std::queue<Node*> q;
        q.push(root_);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            result.push_back(current->data);
            if (current->right) q.push(current->right); // сначала ПРАВЫЙ
            if (current->left)  q.push(current->left);  // потом левый

            if (debug_) {
                std::cout << "  посетили " << current->data << ", очередь = ";
                printQueue(q);
                std::cout << "\n";
            }
        }

        if (debug_) {
            std::cout << "  до разворота:    ";
            printVector(result);
        }
        std::reverse(result.begin(), result.end());
        if (debug_) {
            std::cout << "  после разворота: ";
            printVector(result);
        }
        return result;
    }

    // Печать результата любого обхода: "1 2 3" или "(дерево пустое)"
    static void printVector(const std::vector<T>& values) {
        if (values.empty()) {
            std::cout << "(дерево пустое)\n";
            return;
        }
        for (const T& v : values) std::cout << v << " ";
        std::cout << "\n";
    }

    // Вывод по уровням — для тестов insert/remove, теперь через levelOrder()
    void printLevelOrder() const {
        printVector(levelOrder());
    }

private:
    // ------------- Рекурсивные части обходов в глубину -------------
    // Во всех четырёх одна и та же схема:
    //   если узла нет — выходим (база рекурсии);
    //   иначе в нужном порядке: записать корень / обойти левое / обойти правое.
    // Отличаются только порядком этих трёх строк.

    static void debugIndent(int depth) {
        for (int i = 0; i < depth; ++i) std::cout << "  ";
    }

    void preOrderLeftRec(Node* node, std::vector<T>& result, int depth) const {
        if (!node) return;
        if (debug_) { debugIndent(depth); std::cout << "посетили " << node->data << "\n"; }
        result.push_back(node->data);                      // Корень
        preOrderLeftRec(node->left, result, depth + 1);    // Лево
        preOrderLeftRec(node->right, result, depth + 1);   // Право
    }

    void postOrderLeftRec(Node* node, std::vector<T>& result, int depth) const {
        if (!node) return;
        if (debug_) { debugIndent(depth); std::cout << "зашли в " << node->data << "\n"; }
        postOrderLeftRec(node->left, result, depth + 1);   // Лево
        postOrderLeftRec(node->right, result, depth + 1);  // Право
        if (debug_) { debugIndent(depth); std::cout << "посетили " << node->data << "\n"; }
        result.push_back(node->data);                      // Корень
    }

    void preOrderRightRec(Node* node, std::vector<T>& result, int depth) const {
        if (!node) return;
        if (debug_) { debugIndent(depth); std::cout << "посетили " << node->data << "\n"; }
        result.push_back(node->data);                      // Корень
        preOrderRightRec(node->right, result, depth + 1);  // Право
        preOrderRightRec(node->left, result, depth + 1);   // Лево
    }

    void postOrderRightRec(Node* node, std::vector<T>& result, int depth) const {
        if (!node) return;
        if (debug_) { debugIndent(depth); std::cout << "зашли в " << node->data << "\n"; }
        postOrderRightRec(node->right, result, depth + 1); // Право
        postOrderRightRec(node->left, result, depth + 1);  // Лево
        if (debug_) { debugIndent(depth); std::cout << "посетили " << node->data << "\n"; }
        result.push_back(node->data);                      // Корень
    }
};
