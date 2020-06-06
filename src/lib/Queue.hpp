#ifndef QUEUE_HPP
#define QUEUE_HPP

#include<iostream>
#include<vector>
#include "Node.hpp"

template <typename K, typename V>
class priority_queue
{
private:

    std::vector<Node<K,V>> data;
    
    size_t parentIdx(size_t childIdx)
    {
        if (childIdx % 2 == 0)
        {
            return (childIdx - 2) / 2;
        }
        else
        {
            return (childIdx - 1) / 2;
        }
    }
    size_t rightChildIdx(size_t parentIdx)
    {
        return parentIdx * 2 + 2;
    }    
    size_t leftChildIdx(size_t parentIdx)
    {
        return parentIdx * 2 + 1;
    }
    void shiftUp(size_t child)
    {
        size_t parent = parentIdx(child);
        if (child > 0 && data[child].value < data[parent].value)
        {
            std::swap(data[parent],data[child]);
            shiftUp(parent);
        }
    }
    void shiftDown(size_t parent)
    {
        size_t rightChild = rightChildIdx(parent);
        size_t leftChild = leftChildIdx(parent);
        size_t length = size();
        size_t swapChild = parent;
        if (leftChild < length && data[swapChild].value > data[leftChild].value)
        {
            swapChild = leftChild;
        }
        if (rightChild < length && data[swapChild].value > data[rightChild].value)
        {
            swapChild = rightChild;
        }
        if (swapChild != parent)
        {
            std::swap(data[parent],data[swapChild]);
            shiftDown(swapChild);
        }
        
    }
public:

    size_t size()
    {
        return data.size();
    }    
    void push(K k, V v)
    {   
        Node<K,V> node {k,v,nullptr,nullptr};
        data.push_back(node);   
        size_t child = data.size() - 1;
        shiftUp(child);   
    }
    void push(Node<K,V> * node)
    {
        data.push_back(*node);   
        size_t child = data.size() - 1;
        shiftUp(child);   
    }
    Node<K,V> * pop()
    {   
        if(data.empty())
        {
            std::cout << "Queue is Empty\n";
            exit( 1 ); // Crash
        }
        size_t last = size() - 1;
        std::swap(data[0],data[last]);
        Node<K,V> * node = new Node<K,V>{data[last].key,data[last].value,data[last].left,data[last].right};
        data.pop_back();
        shiftDown(0);
        return node;
    }
    Node<K,V> * front()
    {
        if(data.size() == 0)
        {
            return nullptr;
        }
        else
        {
            return &data[0];
        }
        
    }
    static priority_queue <K,V> make(std::vector<Node<K,V>> d)
    {
        priority_queue q;
        q.data.swap(d);
        int last = q.size() - 1;
        if (last < 1)
        {
            return q;
        }
        for (int parent = q.parentIdx(last); parent >= 0; parent--)
        {
            q.shiftDown(parent);
        }
        return q;
    }
    
};

#endif // QUEUE_HPP