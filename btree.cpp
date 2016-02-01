// b 树

#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std; 

const int M = 4;

using keyT = char;

struct Node {
    unsigned int cnt;
    Node *parent;
    // 多出一个空间，方便在满元素的情况下先找好顺序再分裂
    keyT key[M + 1] = {};
    Node *ptr[M + 2] = {};

    Node(): cnt(0), parent(nullptr) {}
};

Node *BTree = nullptr;

void showNode(Node *node) {
    printf("signle node: %d - ", node -> cnt);
    for (unsigned int i = 0; i <= node ->cnt; ++i) {
        cout << node -> key[i] << ' ';
    }
    putchar(10);
    for (unsigned int i = 0; i <= node ->cnt; ++i) {
        cout << node -> ptr[i] << ' ';
    }
    printf("- %p\n", (void*)node);
}

void _show( Node *node, unsigned int level = 1) {
    printf("node: %d - ", node -> cnt);
    for (unsigned int i = 0; i < node ->cnt; ++i) {
        cout << node -> key[i] << ' ';
    }
    printf("- %p - parent: %p\n", (void*)node, (void*)node -> parent);
    // cout << "address of parent " << &node -> parent << endl;
    for (unsigned int i = 0; i <= node ->cnt; ++i) {
        cout << node -> ptr[i] << ' ';
    }
    fflush(stdout);
    putchar(10);
    // printf("- LEVEL%d\n", level);
    for (unsigned int i = 0; node -> ptr[i] && i <= node -> cnt; ++i) {
        printf("----- LEVEL%d ------\n", level);
        _show(node -> ptr[i], level + 1);
    }
}

int show( Node *node, unsigned int level = 0) {
    puts("=========================== start =========================");
    _show(node);
    return 0;
}

int insert_key_in_order(keyT item, Node *&node) {
    bool moveFlag = false;
    unsigned int i = 0;
    for (i = 0; node ->key[i] != 0 && i < node ->cnt; ++i) {
        // 按顺序存放，插入
        if (item < node -> key[i]) {
            moveFlag = true;
            break;
        }
    }
    if (moveFlag) {
        printf("move key\n");
        fflush(stdout);
        for (unsigned int j = node -> cnt; j > i; --j) {
            node -> key[j] = node -> key[j - 1];
        }
        printf("move ptr\n");
        for (unsigned int j = node -> cnt + 1; j > i; --j) {
            node -> ptr[j] = node -> ptr[j - 1];
        }
        fflush(stdout);
    }
    node ->key[i] = item;
    return i;
}

int insert(keyT item, Node *node, bool up = false) {
    if (!item) {
        return 0;
    }
    cout << item << endl;
    cout << node -> cnt << endl;    
    if (!node) {
        printf("got an empty node, new it.\n");
        fflush(stdout);
        node = new Node();
    }
    if (!up) {
        unsigned int i = 0;
        for (; i < node -> cnt; ++i) {
            if (item < node -> key[i]) {
                if (node -> ptr[i + 1]) {
                    printf("down: %d\n", i);
                    return insert(item, node -> ptr[i]);
                }
            }
        }
        if (node -> ptr[i]) {
            printf("down: %d\n", i);
            return insert(item, node -> ptr[i]);
        }
    }
    
    // 一个节点的 key 满了，需要分裂
    if (node -> cnt == M) {
        printf("split.\n");
        fflush(stdout);
        // 如果需要新建父节点
        if (!node -> parent) {
            Node *parent = new Node();
            node -> parent = parent;
            parent -> ptr[0] = node;
        }
        Node *right = new Node();

        // 先把 key 放进去
        unsigned int pos = insert_key_in_order(item, node);
        // showNode(node);

        // 分裂成两半，mid 指向中间的 key
        unsigned int mid = node -> cnt >> 1;
        // 把中间元素放到 parent 中
        // 在这里，父节点中插入了新的元素，其 cnt 已经加 1，可以直接连接 right
        int posInParent = insert(node -> key[mid], node ->parent, true);
        printf("address = %p, pos = %d\n", (void*)node, posInParent);
        fflush(stdout);
        // showNode(node);
        // cout << BTree -> ptr[1] << BTree -> ptr[0] << endl;
        
        // 接下来向右节点放入元素
        printf("insert into right\n");
        fflush(stdout);
        // 右半边写到右边元素
        ++mid;
        int i = 0;
        for (; mid <= node -> cnt; ++mid, ++i) {
            right -> key[i] = node -> key[mid];
            right -> ptr[i] = node -> ptr[mid];
        }
        right -> ptr[i] = node -> ptr[mid];
        right -> cnt = M >> 1;
        // 反向重新连接 右节点拿过去的 子节点
        for (int i = 0; i < (M >> 1) + 1; ++i) {
            if (right -> ptr[i]) {
                right -> ptr[i] -> parent = right;
            } else {
                break;
            }
        }
        node -> parent -> ptr[posInParent + 1] = right;
        right -> parent = node -> parent;
        node -> key[mid] = 0;
        showNode(right);
        // 重新设置 cnt 为 一半
        node -> cnt = M >> 1;
        // 最后 根节点指针指向 新的父节点
        if (!node -> parent -> parent) {
            BTree = node -> parent;
        }
        
        return pos >= (M - 1) ? pos - (M - 1) : pos;
    }
    // 未到达限制，可以继续插入
    else if (node ->cnt < M) {
            ++(node ->cnt);
            return insert_key_in_order(item, node);
    }
}

void distory(Node *&node) {
    delete node ->parent;
    node ->parent = nullptr;
}

int insertTest() {
    BTree = new Node();
    const char *str = "CNGAHEKQMFWLTZDPRXYS";
    for (int i = 0; i < 20; ++i) {
        insert(str[i], BTree);
        // show(BTree);
    }
    show(BTree);
    distory(BTree);
    return 0;
}

int main() {
    insertTest();
    return 0;
}