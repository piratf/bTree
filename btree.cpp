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
    keyT key[M] = {};
    Node *ptr[M + 1] = {};

    Node(): cnt(0), parent(nullptr) {
        // memset(key, 0, sizeof(key));
        // memset(ptr, 0, sizeof(ptr));
    }
};

Node *BTree = nullptr;

void showNode(Node *&node) {
    printf("node: ");
    for (unsigned int i = 0; i < node ->cnt; ++i) {
        cout << node -> key[i] << ' ';
    }
    putchar(10);
    fflush(stdout);
}

int show(Node *&node, unsigned int level = 0) {
    printf("node: %d - ", node -> cnt);
    for (unsigned int i = 0; i < node ->cnt; ++i) {
        cout << node -> key[i] << ' ';
    }
    putchar(10);
    for (unsigned int i = 0; i <= node ->cnt; ++i) {
        cout << node -> ptr[i] << ' ';
    }
    putchar(10);
    fflush(stdout);
    printf("- LEVEL%d\n", level);
    for (unsigned int i = 0; node -> ptr[i] && i <= node -> cnt; ++i) {
        printf("----- %d ------\n", i);
        show(node -> ptr[i], level + 1);
    }
    return 0;
}

int insert(keyT item, Node *&node, bool up = false) {
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
        printf("new right node\n");
        fflush(stdout);
        Node *right = new Node();
        printf("get right node\n");
        fflush(stdout);
        right -> parent = node -> parent;

        // 设置 父子节点之间 的连接关系
        // 分裂成两半，mid 指向中间的 key
        unsigned int mid = node -> cnt >> 1;
        // 较小的放进 item 放到父节点，较大的往右放
        if (node -> key[mid] < item) {
            swap(node -> key[mid], item);
        }
        // 在这里，父节点中插入了新的元素，其 cnt 已经加 1，可以直接连接 right
        int pos = insert(item, node ->parent, true);
        printf("pos = %d\n", pos);
        fflush(stdout);
        // showNode(node);
        // cout << BTree -> ptr[1] << BTree -> ptr[0] << endl;
        node -> parent -> ptr[pos + 1] = right;
        // 接下来向右节点放入元素
        printf("insert into right\n");
        fflush(stdout);
        for (; mid < node -> cnt; ++mid) {
            insert(node -> key[mid], right);
        }
        // 重新设置 cnt 为 一半
        node -> cnt >>= 1;
        // 最后 根节点指针指向 新的父节点
        BTree = node -> parent;
    }
    // 未到达限制，可以继续插入
    else if (node ->cnt < M) {
        // 可以直接插入新的元素
        unsigned int i = 0;
        // 是否出现插入，需要右移部分元素
        bool moveFlag = false;
        for (i = 0; node ->key[i] != 0 && i < node ->cnt; ++i) {
            // 按顺序存放，插入
            // 如果 可以放到 子节点中，就放到子节点中
            if (item < node -> key[i]) {
                // 如果是 分裂 挤上来的 key，就直接往里放，不再下放
                if (!up && node -> ptr[i + 1]) {
                    printf("down: %d\n", i);
                    // 下放到子节点中
                    return insert(item, node -> ptr[i]);
                } else {
                    moveFlag = true;
                    break;
                }
            }
        }
        // 最后放入元素
        // 如果可以放到子节点中，且已经有 key，不会重复往下放
        if (!up && node -> ptr[i]) {
            printf("down: %d\n", i);
            return insert(item, node -> ptr[i]);
        } else {
            // 元素放到当前节点，先右移，保护元素
            if (moveFlag) {
                printf("move key\n");
                fflush(stdout);
                for (unsigned int j = node -> cnt; j > i; --j) {
                    node -> key[j] = node -> key[j - 1];
                }
                for (unsigned int j = node -> cnt + 1; j > i; --j) {
                    node -> ptr[j] = node -> ptr[j - 1];
                }
                printf("move ptr\n");
                fflush(stdout);
            }
            node ->key[i] = item;
            ++(node ->cnt);
            return i;
        }
    }
    return 0;
}

void distory(Node *&node) {
    delete node ->parent;
    node ->parent = nullptr;
}

int insertTest() {
    BTree = new Node();
    const char *str = "CNGAHEKQMFWLTZDPRXYS";
    for (int i = 0; i < 16; ++i) {
        insert(str[i], BTree);
    }
    show(BTree);
    distory(BTree);
    return 0;
}

int main() {
    insertTest();
    return 0;
}