#include <iostream>
#include <vector>
#include <functional>
#include <queue>

template <typename T>
class BinarySearchTree {
private:
    struct Node {
        T data;       // Хранимое значение (ключ/данные узла)
        Node* left;   // Указатель на левое поддерево (меньшие значения)
        Node* right;  // Указатель на правое поддерево (большие значения)

        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root_;    // Указатель на корневой узел дерева (nullptr, если дерево пустое)
    size_t size_;   // Количество узлов в дереве

    // Рекурсивная вставка
    Node* insert(Node* node, const T& value) {
        if (!node) {
            ++size_;
            return new Node(value);
        }
        if (value < node->data) {
            node->left = insert(node->left, value);
        } else if (value > node->data) {
            node->right = insert(node->right, value);
        }
        // если value == node->data — дубликаты игнорируем
        return node;
    }

    // Поиск минимального элемента в поддереве
    Node* findMin(Node* node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    // Рекурсивное удаление
    Node* remove(Node* node, const T& value) {
        if (!node) return nullptr;

        if (value < node->data) {
            node->left = remove(node->left, value);
        } else if (value > node->data) {
            node->right = remove(node->right, value);
        } else {
            // Найден узел для удаления
            if (!node->left && !node->right) {
                delete node;
                --size_;
                return nullptr;
            } else if (!node->left) {
                Node* temp = node->right;
                delete node;
                --size_;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                --size_;
                return temp;
            } else {
                // Два потомка: заменяем минимальным элементом из правого поддерева
                Node* temp = findMin(node->right);
                node->data = temp->data;
                node->right = remove(node->right, temp->data);
            }
        }
        return node;
    }

    // Рекурсивный поиск
    Node* search(Node* node, const T& value) const {
        if (!node || node->data == value) return node;
        if (value < node->data) return search(node->left, value);
        return search(node->right, value);
    }

    // Рекурсивное освобождение памяти
    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    // Обходы
    void inorder(Node* node, std::vector<T>& result) const {
        if (!node) return;
        inorder(node->left, result);
        result.push_back(node->data);
        inorder(node->right, result);
    }

    void preorder(Node* node, std::vector<T>& result) const {
        if (!node) return;
        result.push_back(node->data);
        preorder(node->left, result);
        preorder(node->right, result);
    }

    void postorder(Node* node, std::vector<T>& result) const {
        if (!node) return;
        postorder(node->left, result);
        postorder(node->right, result);
        result.push_back(node->data);
    }

    // Глубина дерева
    int height(Node* node) const {
        if (!node) return -1;
        return 1 + std::max(height(node->left), height(node->right));
    }

    // Копирование дерева
    Node* clone(Node* node) {
        if (!node) return nullptr;
        Node* newNode = new Node(node->data);
        newNode->left = clone(node->left);
        newNode->right = clone(node->right);
        return newNode;
    }

public:
    // Конструктор по умолчанию
    BinarySearchTree() : root_(nullptr), size_(0) {}

    // Деструктор
    ~BinarySearchTree() {
        clear(root_);
    }

    // Конструктор копирования (Rule of Three/Five)
    BinarySearchTree(const BinarySearchTree& other) : root_(nullptr), size_(0) {
        root_ = clone(other.root_);
        size_ = other.size_;
    }

    // Оператор присваивания копированием
    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            clear(root_);
            root_ = nullptr;
            root_ = clone(other.root_);
            size_ = other.size_;
        }
        return *this;
    }

    // Конструктор перемещения
    BinarySearchTree(BinarySearchTree&& other) noexcept 
        : root_(other.root_), size_(other.size_) {
        other.root_ = nullptr;
        other.size_ = 0;
    }

    // Оператор присваивания перемещением
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
        if (this != &other) {
            clear(root_);
            root_ = other.root_;
            size_ = other.size_;
            other.root_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // Вставка
    void insert(const T& value) {
        root_ = insert(root_, value);
    }

    // Удаление
    void remove(const T& value) {
        root_ = remove(root_, value);
    }

    // Поиск
    bool contains(const T& value) const {
        return search(root_, value) != nullptr;
    }

    // Очистка дерева
    void clear() {
        clear(root_);
        root_ = nullptr;
        size_ = 0;
    }

    // Размер
    size_t size() const { return size_; }

    // Пустое ли дерево
    bool empty() const { return size_ == 0; }

    // Высота дерева
    int height() const {
        return height(root_);
    }

    // Обходы — возвращают вектор
    std::vector<T> inorder() const {
        std::vector<T> result;
        inorder(root_, result);
        return result;
    }

    std::vector<T> preorder() const {
        std::vector<T> result;
        preorder(root_, result);
        return result;
    }

    std::vector<T> postorder() const {
        std::vector<T> result;
        postorder(root_, result);
        return result;
    }

    // Обход в ширину (BFS)
    std::vector<T> levelOrder() const {
        std::vector<T> result;
        if (!root_) return result;

        std::queue<Node*> q;
        q.push(root_);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            result.push_back(current->data);

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        return result;
    }

    // Применить функцию к каждому элементу (in-order)
    void forEach(const std::function<void(const T&)>& func) const {
        std::vector<T> items = inorder();
        for (const auto& item : items) {
            func(item);
        }
    }
};

// ==================== ПРИМЕР ИСПОЛЬЗОВАНИЯ ====================

int main() {
    BinarySearchTree<int> tree;

    // Вставка
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    std::cout << "Размер: " << tree.size() << "\n";
    std::cout << "Высота: " << tree.height() << "\n";

    // Обходы
    std::cout << "In-order: ";
    for (int x : tree.inorder()) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "Pre-order: ";
    for (int x : tree.preorder()) std::cout << x << " ";
    std::cout << "\n";

    std::cout << "Level-order: ";
    for (int x : tree.levelOrder()) std::cout << x << " ";
    std::cout << "\n";

    // Поиск
    std::cout << "Содержит 40? " << (tree.contains(40) ? "Да" : "Нет") << "\n";
    std::cout << "Содержит 100? " << (tree.contains(100) ? "Да" : "Нет") << "\n";

    // Удаление
    tree.remove(30);
    std::cout << "После удаления 30 (in-order): ";
    for (int x : tree.inorder()) std::cout << x << " ";
    std::cout << "\n";

    // Копирование
    BinarySearchTree<int> tree2 = tree;
    std::cout << "Копия (in-order): ";
    for (int x : tree2.inorder()) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}