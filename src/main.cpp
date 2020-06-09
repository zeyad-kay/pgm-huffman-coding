#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lib/cxxopts.hpp"
#include "lib/helpers.hpp"
#include "lib/Queue.hpp"
#include "lib/Map.hpp"
#include "lib/Huffman.hpp"
int main(int argc, char** argv)
{

    cxxopts::Options options("Compress", "Huffman encoding");

    options.add_options()
        ("t,tree", "tree file", cxxopts::value<std::string>())
        ("h,help", "Print usage")
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    // -----------------------------------------------> DECOMPRESSION
     if (result.count("tree"))
    {

        // read frequency file
        std::string fileName = result["tree"].as<std::string>();
        std::ifstream frqFile(fileName);
        
        if (!frqFile)
        {
            std::cout << "Tree file doesn't exist\n";
            exit(1);
        }
        
        Map<uint8_t,int> frequencyTable;
        frequencyTable.readFreq(frqFile);
        frqFile.close();
        remove((char *) &fileName[0]);
        
        int width, height, size = width * height;

        HuffmanTree<uint8_t,int> huffTree;
        huffTree.buildTree(frequencyTable);

        // read encoded file 
        fileName = argv[1];
        std::ifstream encFile(fileName,std::ios::binary);
        
        if (!encFile)
        {
            std::cout << "Encoded file doesn't exist\n";
            exit(1);
        }

        std::cout << "Decompressing...\n";

        encFile.read((char *) &width, sizeof(width));
        encFile.read((char *) &height, sizeof(height));
        std::vector<uint8_t> pixels;
        pixels = huffTree.decodeFile(encFile);
        encFile.close();
        remove((char *) &fileName[0]);

        std::cout << "Image Resolution: " << width << "x" << height << "\n";
        
        //  write pgm Image file
        std::ofstream pgmFile("image.pgm");
        pgm::writePGM(pgmFile,"P5",pixels,width,height);
        pgmFile.flush();
        pgmFile.close();
        std::cout << "DONE!\n";
    }

    // -----------------------------------------------> COMPRESSION
    else
    {
        int width, height, size;
        std::ifstream pgmFile(argv[1]);
        if (!pgmFile)
        {
            std::cout << "PGM file doesn't exist\n";
            exit(1);
        }
        
        std::vector<uint8_t> pixels = pgm::readPGM(pgmFile,width,height);

        size = width * height;
        pgmFile.close();

        std::cout << "Image Resolution: " << width << "x" << height << "\n" << "Compressing..." << "\n";

        Map<uint8_t,int> frequencyTable;
        pgm::freqTable(frequencyTable,pixels);

        HuffmanTree<uint8_t,int> huffTree;
        huffTree.buildTree(frequencyTable);

        // Generate Frequency Table
        std::ofstream frqfile("image.frq");
        frequencyTable.writeFreq(frqfile);
        frqfile.flush();
        frqfile.close();

        // Generate Encoded file
        std::ofstream encFile("image.enc",std::ios::binary);
        encFile.write((char *) &width, sizeof(width));
        encFile.write((char *) &height, sizeof(height));
        huffTree.encodeFile(encFile,pixels);
        encFile.flush();
        encFile.close();
        remove((char *) argv[1]);
        std::cout << "DONE!\n";
    }
}