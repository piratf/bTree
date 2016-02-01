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

void showNodeOneMore(Node *node) {
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

void showNode(Node *node) {
    printf("signle node: %d - ", node -> cnt);
    for (unsigned int i = 0; i < node ->cnt; ++i) {
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

/**
 * [find_index_in_parent description]
 * @author piratf
 * @param  node 
 * @return      返回子节点在父节点中 ptr 的下标，出错返回 -1
 */
 unsigned int find_index_in_parent(Node *&node) {
    if (!node -> parent || node -> cnt < 1) {
        return -1;
    }
    Node *&parent = node -> parent;
    unsigned int i = 0;
    for (; i < parent -> cnt; ++i) {
        if (parent -> key[i] > node -> key[0]) {
            return i;
        }
    }
    return i;
}

int insert(keyT item, Node *node, bool up = false) {
    if (!item) {
        return 0;
    }
    cout << "insert: -> ";
    cout << item << ' ';
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
        // showNode(right);
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

bool push_back(keyT item, Node *node) {
    if (!item || !node || node -> cnt >= M) {
        showNode(node);
        puts("push_back false.");
        return false;
    }
    node -> key[node -> cnt] = item;
    ++node -> cnt;
    return true;
}

/**
 * 删除元素
 * @author piratf
 * @param  item 
 * @param  node 
 * @return      
 */
 bool remove(keyT item, Node *node) {
    if (!item || !node) {
        return false;
    }
    cout << "remove " << item << endl;
    // 查找元素的位置
    int flag = -1;
    unsigned int i = 0;
    for (; i < node -> cnt; ++i) {
        // 找到该元素
        if (item == node -> key[i]) {
            flag = i;
        }
        // 到子节点中找
        if (item < node -> key[i] && node -> ptr[i]) {
            return remove(item, node -> ptr[i]);
        }
    }
    // 最后一个孩子
    if (-1 == flag) {
        return remove(item, node -> ptr[i]);
    }
    // 没找到节点
    if (-1 == flag) {
        return false;
    }
    printf("flag = %d\n", flag);
    fflush(stdout);
    // 现在找到这个节点
    // 存在子节点，就要保护
    // 从该 key 的孩子中拿一个上来，节点个数不变
    if (node -> ptr[flag] && node -> ptr[flag + 1]) {
        node -> key[flag] = node -> ptr[flag + 1] -> key[0];
        remove (node -> ptr[flag + 1] -> key[0], node -> ptr[flag + 1]);
    } else {
        // 不需要保护，直接删除
        // 为了程序不出错，如果节点的 key 是 1 个，则直接删除这个子节点
        // 此时退化成二叉树
        if (node -> cnt == 1) {
            Node *parent = node -> parent;
            unsigned int i = 0;
            for (; i < parent -> cnt; ++i) {
                if (parent -> key[i] > node -> key[0]) {
                    delete parent -> ptr[i];
                    parent -> ptr[i] = nullptr;
                }
            }
            // 一定存在，如果没找到，就删最后一个
            delete parent -> ptr[i];
            parent -> ptr[i] = nullptr;
        }
        // 左移元素
        for (unsigned int i = flag + 1; i < node -> cnt; ++i) {
            node -> key[i - 1] = node -> key[i];
        }
        // 左移指针域
        for (unsigned int i = flag + 1; i < node -> cnt; ++i) {
            node -> ptr[i] = node -> ptr[i+1];
        }
        // 收缩 cnt
        --node -> cnt;
        show(BTree);
        
        // 检查是否元素过少，如果是看是否可以左旋或右旋
        if (node -> cnt < (M >> 1)) {
            puts("element not enough in this node.");
            // 查找当前节点在父节点中的位置
            Node *parent = node -> parent;
            int posInParent = find_index_in_parent(node);
            printf("posInParent: %d\n", posInParent);
            // 如果右边的节点存在且可以左旋
            if (posInParent < parent -> cnt && parent -> ptr[posInParent + 1] -> cnt > (M >> 1)) {
                Node *right = parent -> ptr[posInParent + 1];
                // 先拿到父节点的 key，放到自己的当前位置
                // 因为是 父节点的 key，所以一定是 最后一个
                node -> key[node -> cnt] = parent -> key[posInParent];
                ++node -> cnt;
                // 删除 父节点 被拿走的 key
                remove(parent -> key[posInParent], parent);
                // 会自动从右节点拿最左的 key 补充给 父节点
            } else if (posInParent > 0 && parent -> ptr[posInParent - 1] -> cnt > (M >> 1)) {
                // 左节点可以右旋
                Node *left = parent -> ptr[posInParent - 1];
                // 先拿到父节点的 key，放到自己的当前位置
                // 因为是 父节点的 key，所以一定是 最后一个
                node -> key[node -> cnt] = parent -> key[posInParent];
                ++node -> cnt;
                // 删除 父节点 被拿走的 key
                remove(parent -> key[posInParent], parent);
                // 会自动从右节点拿最左的 key 补充给 父节点
            } else {
                // 如果两边都不能旋转，就合并
                puts("merge here.");
                // 左节点可以合并
                if (posInParent > 0) {
                    puts("merge left.");
                    Node *left = parent -> ptr[posInParent - 1];
                    // 先从父节点获取 key
                    push_back(parent -> key[posInParent - 1], left);
                    // 把右边的一起拿过来
                    for (int i = 0; i < node -> cnt; ++i) {
                        push_back(node -> key[i], left);
                        left -> ptr[left -> cnt - 1] = node -> ptr[i];
                    }
                    // 清理当前被废弃的节点
                    delete parent -> ptr[posInParent];
                    parent -> ptr[posInParent] = nullptr;
                    // 删除 父节点的 key
                    if (1 == parent -> cnt) {
                        delete BTree;
                        BTree = node;
                    } else {
                        remove(parent -> key[posInParent - 1], parent);
                    }
                } else {
                    // 合并右节点
                    puts("merge right.");
                    // showNode(node);
                    Node *right = parent -> ptr[posInParent + 1];
                    // 先从父节点获取 key
                    push_back(parent -> key[posInParent], node);
                    // 把右边的一起拿过来
                    // show(BTree);
                    for (int i = 0; i < right -> cnt; ++i) {
                        push_back(right -> key[i], node);
                        node -> ptr[node -> cnt - 1] = right -> ptr[i];
                    }
                    // 清理当前被废弃的节点
                    delete parent -> ptr[posInParent + 1];
                    parent -> ptr[posInParent + 1] = nullptr;
                    // 删除 父节点的 key
                    if (1 == parent -> cnt) {
                        delete BTree;
                        BTree = node;
                    } else {
                        remove(parent -> key[posInParent - 1], parent);
                    }
                }
            }
        }
    }
    return true;
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
    // show(BTree);
    remove('H', BTree);
    remove('T', BTree);
    remove('R', BTree);
    // show(BTree);
    remove('E', BTree);
    show(BTree);
    distory(BTree);
    return 0;
}

int main() {
    insertTest();
    return 0;
}