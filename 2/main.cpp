/*4_2 Дано число N и N строк. Каждая строка содержащит команду добавления или удаления натуральных чисел, а также запрос на получение k-ой порядковой статистики.
 * Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
 * Запрос на получение k-ой порядковой статистики задается числом k. Требуемая скорость выполнения запроса - O(log n).
 */
#include <iostream>

using std::cin;
using std::cout;

#define int long long

template<typename T, class Compare>
class AVLTree {
private:
    class AVLTreeNode {
    public:
        T key;
        int height;
        int cntChild;
        AVLTreeNode* right;
        AVLTreeNode* left;
        explicit AVLTreeNode(const T& key) : key(key), height(1), cntChild(1), left(nullptr), right(nullptr) {}
        ~AVLTreeNode() = default;
        AVLTreeNode(const AVLTreeNode&) = delete;
        AVLTreeNode& operator=(const AVLTreeNode&) = delete;
    };

    Compare comp;
    AVLTreeNode* root;

    int getHeight(AVLTreeNode* p) const { return (p == nullptr ? 0 : p->height); }

    int getCntChild(AVLTreeNode* p) const { return (p == nullptr ? 0 : p->cntChild); }

    int balance(AVLTreeNode* p) const { return getHeight(p->right) - getHeight(p->left); }

    void updateNode(AVLTreeNode* node)
    {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        node->cntChild = getCntChild(node->left) + getCntChild(node->right) + 1;
    }

    AVLTreeNode* rotateRight(AVLTreeNode* node)
    {
        AVLTreeNode* tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        updateNode(node);
        updateNode(tmp);
        return tmp;
    }

    AVLTreeNode* rotateLeft(AVLTreeNode* node)
    {
        AVLTreeNode* tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        updateNode(node);
        updateNode(tmp);
        return tmp;
    }

    AVLTreeNode* rebalance(AVLTreeNode* node) {
        updateNode(node);
        switch (balance(node))
        {
            case 2:
                if (balance(node->right) < 0) node->right = rotateRight(node->right);
                return rotateLeft(node);
            case -2:
                if (balance(node->left) > 0) node->left = rotateLeft(node->left);
                return rotateRight(node);
            default:
                return node;
        }
    }

    AVLTreeNode* insert(AVLTreeNode* node, const T& key)
    {
        if (node == nullptr) {
            return new AVLTreeNode(key);
        }
        if (comp(key, node->key)) {
            node->left = insert(node->left, key);
        }
        else {
            node->right = insert(node->right, key);
        }
        return rebalance(node);
    }


    AVLTreeNode* deleteMin(AVLTreeNode* node)
    {
        AVLTreeNode* min;
        if (node->left == nullptr) {
            min = node;
            node = node->right;
        }
        else {
            min = deleteMin(node->left);
            node = rebalance(node);
        }
        return min;
    }

    AVLTreeNode* deleteMax(AVLTreeNode*& node)
    {
        AVLTreeNode* max;
        if (node->right == nullptr) {
            max = node;
            node = node->left;
        }
        else {
            max = deleteMax(node->right);
            node = rebalance(node);
        }
        return max;
    }

    AVLTreeNode* deleteNode(AVLTreeNode* node, const T& key)
    {
        if (node == nullptr) return nullptr;
        if (key == node->key) {
            AVLTreeNode* leftChild = node->left;
            AVLTreeNode* rightChild = node->right;
            AVLTreeNode* choose = nullptr;
            if (balance(node) > 0) {
                choose = deleteMin(rightChild);
            } else {
                if (leftChild != nullptr) {
                    choose = deleteMax(leftChild);
                }
            }
            delete node;
            if (choose == nullptr) return nullptr;
            choose->left = leftChild;
            choose->right = rightChild;
            return rebalance(choose);
        } else if (comp(key, node->key)) {
            node->left = deleteNode(node->left, key);
        } else {
            node->right = deleteNode(node->right, key);
        }
        return rebalance(node);
    }

    void free(AVLTreeNode* node) {
        if (node == nullptr) return;
        free(node->left);
        free(node->right);
        delete node;
    }

public:
    AVLTree() = delete;
    explicit AVLTree(Compare comp) : comp(comp), root(nullptr) {}
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    ~AVLTree() {
        free(root);
    }

    void Add(const T& key) {
        root = insert(root, key);
    }

    void Delete(const T& key) {
        root = deleteNode(root, key);
    }

    T k_statistic(AVLTreeNode *node, int k) {
        if (node->left != nullptr && k < getCntChild(node->left)) {
            return k_statistic(node->left, k);
        }
        if (node->right != nullptr && k > getCntChild(node->left)) {
            return k_statistic(node->right, k - getCntChild(node->left) - 1);
        }
        return node->key;
    }
    AVLTreeNode* getRoot() {
        return root;
    }
};

struct Compare { bool operator()(int a, int b) const { return b > a; } };

void run() {
    int n;
    cin >> n;
    Compare comp;
    AVLTree<int, Compare> tree(comp);
    for (int i = 0, a, k; i < n; i++) {
        cin >> a >> k;
        if (a >= 0) {
            tree.Add(a);
        } else {
            tree.Delete(-a);
        }
        cout << tree.k_statistic(tree.getRoot(), k) << "\n";
    }
}

signed main()
{
    run();
    return 0;
}