#ifndef HELPERS_HPP
#define HELPERS_HPP

#include<iostream>
#include<fstream>
#include<sstream>
#include "Map.hpp"
#include<vector>

namespace pgm
{
    std::vector<uint8_t> readPGM(std::ifstream &readFile, int &width, int &height)
    {
        int size;
        std::string line;
        std::stringstream ss;
        
        // skip first line
        getline(readFile,line);

        // check the file format
        if (line.compare("P5") == 0)
        {
            // get width and height
            getline(readFile,line);
            ss << line;
            ss >> width >> height;
            
            // skip Maximum value
            getline(readFile,line);
            
            
            size = width *  height;
            std::vector<uint8_t> pixels(size);
            
            for (int i = 0; i < size; i++)
            {
                pixels[i] = readFile.get();
            }
            readFile.close();
            return pixels;        
        }
        else if(line.compare("P2") == 0)
        {
            // get width and height
            getline(readFile,line);
            ss << line;
            ss >> width >> height;

            // skip Maximum value
            getline(readFile,line);
            
            size = width *  height;
            std::vector<uint8_t> pixels(size);
            for (int i = 0; i < size; i++)
            {
                getline(readFile,line,' ');
                pixels[i] = std::stoi(line);
            }
            return pixels;
        }
        else
        {
            std::cout << "File not in PGM format";
            exit(1);
        }
    }

    void writePGM(std::ofstream &writeFile, std::string format,std::vector<uint8_t> pixels, int width, int height)
    {
        if (!writeFile.is_open())
        {
            std::cout << "Couldn't open file.";
            exit(1);
        }
        int size = width * height;
        
        if (format.compare("P2") == 0)
        {
            writeFile << "P2" << '\n';
            writeFile << width << " " << height << '\n';
            writeFile << "255" << '\n';
            for (int i = 1; i <= size; i++)
            {
                if (i % width == 0)
                {
                    writeFile << "\n";
                }
                writeFile << pixels[i] << " ";      
            }
        }
        else if (format.compare("P5") == 0)
        {            
            writeFile << "P5" << '\n';
            writeFile << width << " " << height << '\n';
            writeFile << "255" << '\n';
            for (int i = 0; i < size; i++)
            {
                writeFile << (unsigned char) pixels[i];      
            } 
        }    
        else
        {
            std::cout << "Pick a proper pgm format";
            exit(1); 
        }
            writeFile.flush();
    }
    template<typename K, typename V>
    void freqTable(Map<K,V> &freq, std::vector<K> &pixels)
    {
        for (auto p : pixels)
        {
            freq[p]++;
        }
    }
}
#endif // HELPERS_HPP