#include <iostream>
#include <cmath>

using namespace std;

int max(int x, int y)
{
    return (x > y) ? x : y;
}

class AVL_TREE;
class Node
{
public:
    Node *left;
    Node *right;
    int height;
    int val;

    Node(int val)
    {
        this->val = val;
        height = 1;
        left = nullptr;
        right = nullptr;
    }

    friend class AVL_TREE;
    friend ostream &operator<<(ostream &, const AVL_TREE &);
    friend ostream &operator<<(ostream &, const Node &);
};

ostream &operator<<(ostream &out, const Node &node)
{
    out << node.val;
    return out;
}

class AVL_TREE
{
public:
    Node *root = nullptr;

    static int get_height(Node *node)
    {
        return (node ? node->height : 0);
    }

    static void update_height(Node *node)
    {
        node->height = max(get_height(node->left), get_height(node->right)) + 1;
    }

    static int get_balance(Node *node)
    {
        return get_height(node->left) - get_height(node->right);
    }

    static Node *rotate_right(Node *node)
    {
        Node *other = node->left;
        node->left = other->right;
        other->right = node;

        update_height(node);
        update_height(other);
        return other;
    }

    static Node *rotate_left(Node *node)
    {
        Node *other = node->right;
        node->right = other->left;
        other->left = node;

        update_height(node);
        update_height(other);
        return other;
    }

    static Node *insert_helper(Node *node, int val)
    {
        if (node == nullptr)
            return new Node(val);

        if (val < node->val)
            node->left = insert_helper(node->left, val);
        else
            node->right = insert_helper(node->right, val);

        update_height(node);

        if (get_balance(node) > 1 && val < node->left->val)
            return rotate_right(node);

        if (get_balance(node) < -1 && val > node->right->val)
            return rotate_left(node);

        if (get_balance(node) > 1 && val > node->left->val)
        {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }

        if (get_balance(node) < -1 && val < node->right->val)
        {
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }

        return node;
    }

    static Node *delete_helper(Node *node, int val)
    {
        if (node == nullptr)
            return nullptr;

        if (val < node->val)
            node->left = delete_helper(node->left, val);
        else if (val > node->val)
            node->right = delete_helper(node->right, val);
        else
        {
            if (node->left == nullptr)
            {
                Node *temp = node->right;
                delete node;
                return temp;
            }

            if (node->right == nullptr)
            {
                Node *temp = node->left;
                delete node;
                return temp;
            }

            Node *successor = node->right;
            while (successor->left != nullptr)
            {
                successor = successor->left;
            }
            node->val = successor->val;
            node->right = delete_helper(node->right, node->val);
            return node;
        }

        update_height(node);

        int balance = get_balance(node);

        if (balance > 1)
        {
            if (get_balance(node->left) >= 0)
                return rotate_right(node);
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }

        if (balance < -1)
        {
            if (get_balance(node->right) <= 0)
                return rotate_left(node);
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }

        return node;
    }

    static bool search_helper(Node *node, int val)
    {
        if (node == nullptr)
            return false;
        if (node->val == val)
            return true;
        if (val < node->val)
            return search_helper(node->left, val);
        return search_helper(node->right, val);
    }

    static void destroy_helper(Node *node)
    {
        if (node == nullptr)
            return;
        destroy_helper(node->left);
        destroy_helper(node->right);
        delete node;
    }

    static void inorder_helper(Node *node)
    {
        if (node == nullptr)
            return;
        inorder_helper(node->left);
        cout << node->val << " ";
        inorder_helper(node->right);
    }

public:
    friend ostream &operator<<(ostream &, const AVL_TREE &);
    void insert_node(int val)
    {
        root = insert_helper(root, val);
    }

    void delete_node(int val)
    {
        root = delete_helper(root, val);
    }

    bool search_node(int val)
    {
        return search_helper(root, val);
    }

    void destroy()
    {
        destroy_helper(this->root);
    }

    void inorder()
    {
        inorder_helper(this->root);
        cout << endl;
    }
};

ostream &operator<<(ostream &out, const AVL_TREE &tree)
{
    Node **prev = new Node *[1];
    prev[0] = {tree.root};

    int height = AVL_TREE::get_height(tree.root);
    int maxlength = pow(2, height);

    for (int length = 1; length < maxlength; length *= 2)
    {
        Node **current = new Node *[2 * length] {};
        for (int k = 0; k < 3 * (maxlength - length) / 2; k++)
            out << " ";
        for (int j = 0; j < length; j++)
        {
            if (prev[j] == nullptr)
                out << " # ";
            else
            {
                out << " " << prev[j]->val << " ";
                current[2 * j] = prev[j]->left;
                current[2 * j + 1] = prev[j]->right;
            }
        }

        delete[] prev;
        prev = current;
        out << endl;
    }
    delete[] prev;
    return out;
}

int main(void)
{
    AVL_TREE tree;
    tree.insert_node(1);
    tree.insert_node(5);
    tree.insert_node(6);
    tree.insert_node(8);
    tree.insert_node(9);
    tree.insert_node(7);
    tree.insert_node(3);
    tree.insert_node(2);
    tree.inorder();
    cout << tree;
    tree.destroy();
}
