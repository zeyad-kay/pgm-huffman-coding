#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include "Node.hpp"
#include "Queue.hpp"
#include "Map.hpp"

template<typename K,typename V>
class HuffmanTree
{
private:

    Map<K,std::string> charCode;
    Node<K,V> * root = nullptr;
    
    static bool isEmpty(Node<K,V> * node)
    {
        return node == nullptr;
    }

    static bool isLeaf(Node<K,V> * node)
    {
        return !isEmpty(node) && isEmpty(node->right) && isEmpty(node->left);
    }

    static Node<K,V> * removeSmallest(priority_queue<K,V> &original, priority_queue<K,V> &merge)
    {
        if (original.front() && merge.front())
        {
            if (original.front()->value <= merge.front()->value)
            {
                return original.pop();
            }
            else
            {
                return merge.pop();
            }   
        }
        else if(!original.front())
        {
            return merge.pop();
        }
    }

    void decode(std::vector<K> &decodedStream, std::ifstream &file)
    {
        std::streamsize size = file.tellg();
        char * buffer = new char[size];
        file.seekg(0,std::ios::beg);

        file.read(buffer,size);
        std::vector<K> vect(buffer,buffer + size);
        auto node = root;
        
        
        for (int i = 0; i < size; i++)
        {
            if (vect[i] == 0)
            {
                node = node->left;
            }
            else
            {
                node = node->right;
            }
            if (isLeaf(node))
            {
                // std::cout << node->key << " ";
                decodedStream.push_back(node->key);
                node = root;
            }  
        }

    }
    
    void encode(Node<K,V> * node, std::string str, Map<K,std::string> &charCode)
    {
        if (isEmpty(node))
        {
            return;
        }
        
        if (isLeaf(node))
        {
            charCode[node->key] = str;
        }
        encode(node->left, str + "0", charCode);
        encode(node->right, str + "1", charCode);
    }

    size_t size(Node<K,V> * node)
    {
        if(!isEmpty(node))
        {
            return 1 + size(node->left) + size(node->right);
        }
    }
    
public:
    Map<K,std::string> rMap(){return charCode;}
    size_t size()
    {
        return size(root);
    }
    
    bool isEmpty()
    {
        return isEmpty(root);
    }
    
    void buildTree(Map<K,V> &freq)
    {

        priority_queue<K,V> original;
        priority_queue<K,V> merge;

        freq.foreach( [&original] (K k, V v) {
            original.push(k,v);
        } );

        auto left = original.pop();
        auto right = original.pop();
        Node<K,V> * node = new Node<K,V>{0,left->value + right->value,left,right};
        merge.push(node);
        
        while (original.front() || merge.size() != 1)
        {   
            left = removeSmallest(original,merge);
            right = removeSmallest(original,merge);
            node = new Node<K,V>{1,left->value + right-> value,left,right};
            merge.push(node);
        }
        root = merge.pop();
        
        encode(root,"",charCode);
    }

    void encodeFile(std::ofstream &encFile , std::vector<K> &charStream)
    {
        char byte = 0;
        int counter = 1;
        for (auto i : charStream)
        {           
            // feed me more food
            
            for(auto c : charCode[i])
            {
                if (c == '0')
                {   

                    byte <<= 1; 
                }

                else
                {
                    byte = (byte << 1) | 0x01;
                }

                if (counter % 8 == 0)
                {
                    encFile.write(&byte,sizeof(byte));
                    byte = 0;
                }

                counter++;
            }
            
        }
    }

    std::vector<K> decodeFile(std::ifstream &file)
    {
        std::vector<K> stream; 
        decode(stream,file);
        return stream;
    }
};

#endif // HUFFMAN_HPP