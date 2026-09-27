#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <utility> // std::pair

// =========================================================================
// ЗАДАЧА 1. БИНАРНОЕ ДЕРЕВО (без правила упорядоченности, как в дереве поиска)
// =========================================================================
//
// Важное отличие от BST (Binary Search Tree):
// здесь НЕТ правила "меньше — влево, больше — вправо".
// Значит, при добавлении узла нужно самим решить, куда его ставить.
//
// Выбранное правило: вставка ПО УРОВНЯМ (level-order), как в куче (heap).
// Дерево всегда остаётся "полным" (complete tree) — без дырок:
// сначала заполняется уровень 0, потом уровень 1 слева направо, и т.д.
//
//         1
//       /   \
//      2     3
//     / \   /
//    4   5 6      <-- следующий вставленный узел встанет справа от 6
//
// Это же правило пригодится в задачах 4-5 (дерево на массиве, пирамидальная
// и турнирная сортировки), поэтому выбираем его заранее.
//
// ПОЧЕМУ .hpp, А НЕ .h + .cpp:
// Класс шаблонный (template<typename T>), а шаблоны в C++ должны быть
// целиком видны компилятору в каждом файле, где их используют. Поэтому
// объявление и реализацию не разносят по .h/.cpp, а держат вместе в
// одном заголовочном файле (.hpp), который просто #include-ится там,
// где нужен.

template <typename T>
class BinaryTree {
private:
    // Узел дерева. В задании его называют "лист (ячейка)",
    // у которого есть ссылки на поддеревья (другие "листы").
    struct Node {
        T data;
        Node* left;
        Node* right;

        explicit Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root_;   // корень дерева (nullptr, если дерево пустое)
    size_t size_;  // количество узлов

    // Рекурсивное удаление всех узлов (используется в деструкторе и clear())
    void clearRecursive(Node* node) {
        if (!node) return;
        clearRecursive(node->left);
        clearRecursive(node->right);
        delete node;
    }

public:
    BinaryTree() : root_(nullptr), size_(0) {}

    ~BinaryTree() {
        clearRecursive(root_);
    }

    // Дерево можно копировать/перемещать при необходимости позже,
    // но пока намеренно НЕ добавляю конструкторы копирования/перемещения —
    // в задании их нет, а лишний код только мешает читать структуру.
    // Если понадобится копировать дерево — допишем отдельно.

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // ---------------------------------------------------------------
    // ДОБАВЛЕНИЕ УЗЛА (по уровням, BFS)
    // ---------------------------------------------------------------
    // Идём в ширину от корня и ищем первый узел, у которого есть
    // свободное место (left == nullptr или right == nullptr).
    // Именно туда и вставляем новый узел.
    void insert(const T& value) {
        Node* newNode = new Node(value);
        ++size_;

        if (!root_) {
            // Дерево было пустым — новый узел становится корнем
            root_ = newNode;
            return;
        }

        std::queue<Node*> q;
        q.push(root_);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            if (!current->left) {
                current->left = newNode;
                return;
            }
            else {
                q.push(current->left);
            }

            if (!current->right) {
                current->right = newNode;
                return;
            }
            else {
                q.push(current->right);
            }
        }
    }

    // ---------------------------------------------------------------
    // УДАЛЕНИЕ УЗЛА ПО ЗНАЧЕНИЮ
    // ---------------------------------------------------------------
    // Раз в дереве нет порядка, "аккуратно вырезать" узел и сшить
    // поддеревья, как в BST, не получится — дерево потеряло бы
    // свойство "полноты" (появились бы дырки в структуре).
    //
    // Поэтому применяем приём из кучи (heap delete):
    // 1) находим узел с нужным значением (первый по уровням, BFS);
    // 2) находим ПОСЛЕДНИЙ узел дерева по уровням (самый "новый");
    // 3) переносим данные последнего узла в удаляемый узел;
    // 4) сам последний узел (теперь дубликат) физически удаляем.
    //
    // Если удаляемый узел сам оказался последним — шаги 3-4
    // отработают как копирование "самого в себя" и корректное
    // удаление, отдельный случай обрабатывать не нужно.
    //
    // Возвращает true, если значение найдено и удалено.
    bool remove(const T& value) {
        if (!root_) return false;

        // Обходим дерево в ширину и запоминаем для КАЖДОГО узла
        // пару (родитель, сам узел). Это нужно, чтобы потом уметь
        // отцепить последний узел от его родителя.
        std::vector<std::pair<Node*, Node*>> levelOrder; // {parent, node}

        std::queue<std::pair<Node*, Node*>> q;
        q.push({ nullptr, root_ });

        while (!q.empty()) {
            std::pair<Node*, Node*> item = q.front();
            Node* parent = item.first;
            Node* node = item.second;
            q.pop();
            levelOrder.push_back({ parent, node });

            if (node->left)  q.push({ node, node->left });
            if (node->right) q.push({ node, node->right });
        }

        // Ищем первый узел с искомым значением
        int targetIndex = -1;
        for (size_t i = 0; i < levelOrder.size(); ++i) {
            if (levelOrder[i].second->data == value) {
                targetIndex = static_cast<int>(i);
                break;
            }
        }

        if (targetIndex == -1) {
            return false; // значение не найдено
        }

        // Последний узел по уровням — последний элемент в levelOrder
        int lastIndex = static_cast<int>(levelOrder.size()) - 1;
        Node* targetNode = levelOrder[targetIndex].second;
        Node* lastNode = levelOrder[lastIndex].second;
        Node* lastParent = levelOrder[lastIndex].first;

        // Переносим значение последнего узла в удаляемый узел
        targetNode->data = lastNode->data;

        // Отцепляем последний узел от родителя
        if (lastParent) {
            if (lastParent->left == lastNode) {
                lastParent->left = nullptr;
            }
            else {
                lastParent->right = nullptr;
            }
        }
        else {
            // Последний узел был единственным узлом дерева (корнем)
            root_ = nullptr;
        }

        delete lastNode;
        --size_;
        return true;
    }

    // Поиск наличия значения (пригодится для проверки и для будущих обходов)
    bool contains(const T& value) const {
        if (!root_) return false;
        std::queue<Node*> q;
        q.push(root_);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            if (current->data == value) return true;
            if (current->left)  q.push(current->left);
            if (current->right) q.push(current->right);
        }
        return false;
    }

    // Вспомогательный вывод "по уровням" — чтобы визуально проверять
    // структуру дерева после insert/remove. Обходы (задача 2) допишем
    // отдельным блоком позже.
    void printLevelOrder() const {
        if (!root_) {
            std::cout << "(дерево пустое)\n";
            return;
        }
        std::queue<Node*> q;
        q.push(root_);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            std::cout << current->data << " ";
            if (current->left)  q.push(current->left);
            if (current->right) q.push(current->right);
        }
        std::cout << "\n";
    }
};