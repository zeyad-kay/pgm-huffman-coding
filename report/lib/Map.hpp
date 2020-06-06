#ifndef MAP_HPP
#define MAP_HPP

#include<iostream>
#include<vector>
#include<fstream>
#include "Node.hpp"

template <typename K , typename V>
class Map
{
private:
    
    Node<K,V> * root = nullptr;
    
    static bool isEmpty(Node<K,V> * node)
    {
        return node == nullptr;
    }

    static size_t size(Node<K,V> * node)
    {
        if (!isEmpty(node))
        {
            return 1 + size(node->left) + size(node->right);
        }
    }

    static Node<K,V> * insert(Node<K,V> * node, K key, V value)
    {
        if(isEmpty(node))
        {
            node = new Node<K,V>{key,value,nullptr,nullptr};
        }
        else if( key != node->key)
        {
            if(key > node->key)
            {
               node->right = insert(node->right,key,value);
            }
            else
            {
                node->left = insert(node->left,key,value);        
            }
        }
        return node;
    }
    
    static bool find(Node<K,V> * node , K key)
    {
        if (isEmpty(node))
        {
            return false;
        }
        else
        {
            if (node->key == key)
            {
                return true;
            }
            else if (key > node->key)
            {
                find(node->right,key);
            }
            else if (key < node->key)
            {
                find(node->left,key);
            }
        }
    }
    
    static V &valueAt(Node<K,V> * node,K key)
    {
        if (isEmpty(node))
        {
            std::cout << "Key not found" << '\n';
            exit(1);
        }
        else
        {
            if (key == node->key)
            {
                return node->value;
            }
            else if (key > node->key)
            {
                valueAt(node->right,key);
            }
            else
            {
                valueAt(node->left,key);
            }
        }   
    }
    static void clear( Node<K,V> * node )
    {
        if ( !isEmpty( node ))
        {
            clear( node->left );
            clear( node->right );
            delete node;
        }
    }

    template<typename Function>
    static void foreach(Node<K,V> * node , Function fn)
    {
        if(!isEmpty(node))
        {
            foreach(node->left,fn);
            fn(node->key,node->value);
            foreach(node->right,fn);
        }
    }
public:

    bool isEmpty()
    {
        return isEmpty(root);
    }

    size_t size()
    {
        return size(root);
    }

    void insert(K key, V value)
    {
        root = insert(root,key,value);
    }
    
    bool find(K key)
    {
        return find(root,key);
    }

    V &valueAt(K key)
    {
        return valueAt(root,key);
    }

    V &operator[](K key)
    {
        if (!find(key))
        {
            insert(key,V());
        }
        return valueAt(key);
    }

    void writeFreq(std::ofstream &file)
    {
        foreach(root,[&file](K key, V value){
            file.write((char *) &key,sizeof(K));
            file.write((char *) &value,sizeof(V));
        });
    }

    void readFreq(std::ifstream &file)
    {
        K key;
        V value;    
        while (file.good())
        {
            file.read((char *) &key,sizeof(key));
            file.read((char *) &value,sizeof(value));
            insert(key,value);
        }    
    }

    template<typename Function>
    void foreach(Function fn)
    {
        foreach(root,fn);
    }

    ~Map()
    {
        clear(root);
    }
};

#endif // MAP_HPP