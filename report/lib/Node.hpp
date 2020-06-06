#ifndef NODE_HPP
#define NODE_HPP
template <typename K,typename V>
struct Node
{
    K key;
    V value;
    Node * right = nullptr;
    Node * left = nullptr;
};

#endif // NODE_HPP