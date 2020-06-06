#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
// #include "lib/cxxopts-2.2.0/include/cxxopts.hpp"
#include "lib/helpers.hpp"
#include "lib/Queue.hpp"
#include "lib/Map.hpp"
#include "lib/Huffman.hpp"
int main()
{
    // ------------------------------------------> Compress
    int width,height,maxValue,size;
    std::ifstream pgmFile("../data/NORMAL2-IM-1427-0001.pgm");
    std::vector<uint8_t> pixels = pgm::readPGM(pgmFile,width,height,maxValue);

    size = width * height;
    pgmFile.close();  
    
    Map<uint8_t,int> frequencyTable;
    pgm::freqTable(frequencyTable,pixels);
    
    HuffmanTree<uint8_t,int> huffTree;
    huffTree.buildTree(frequencyTable);


    std::ofstream frqfile("test.frq");
    frequencyTable.writeFreq(frqfile);
    frqfile.close();


    std::ofstream encfile("test.enc",std::ios::binary);
    huffTree.encodeFile(encfile,pixels);
    encfile.close();

// ------------------------------------------> Decompress


    std::ifstream testfrq("test.frq");
    Map<uint8_t,int> newfreq;
    newfreq.readFreq(testfrq);
    testfrq.close();
    
    std::ifstream testenc("test.enc",std::ios::binary | std::ios::ate);
    HuffmanTree<uint8_t,int> testTree;
    testTree.buildTree(newfreq);
    std::vector<uint8_t> testpixels(size);
    testpixels = testTree.decodeFile(testenc);
    testenc.close();
    
    
    std::ofstream testpgm("test.pgm");
    pgm::writePGM(testpgm,"P5",testpixels,width,height,maxValue);
    testpgm.close();

}