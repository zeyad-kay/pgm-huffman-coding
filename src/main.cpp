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
        std::string treeFileName = result["tree"].as<std::string>();
        std::ifstream frqFile(treeFileName);
        // read encoded file 
        std::string encodedFileName = (std::string) argv[1];
        std::ifstream encFile(encodedFileName,std::ios::binary);
        
        if (!frqFile)
        {
            std::cout << "Tree file doesn't exist\n";
            exit(1);
        }
        if (!encFile)
        {
            std::cout << "Encoded file doesn't exist\n";
            exit(1);
        }
        
        Map<uint8_t,int> frequencyTable;
        frequencyTable.readFreq(frqFile);
        frqFile.close();
        
        int width, height, size = width * height;

        HuffmanTree<uint8_t,int> huffTree;
        huffTree.buildTree(frequencyTable);      

        std::cout << "Decompressing...\n";

        encFile.read((char *) &width, sizeof(width));
        encFile.read((char *) &height, sizeof(height));
        std::vector<uint8_t> pixels;
        pixels = huffTree.decodeFile(encFile);
        encFile.close();

        std::cout << "Image Resolution: " << width << "x" << height << "\n";
        
        //  write pgm Image file
        std::ofstream pgmFile(std::string (encodedFileName.begin(),encodedFileName.end()-3) + "pgm");
        pgm::writePGM(pgmFile,"P5",pixels,width,height);
        pgmFile.flush();
        pgmFile.close();
        // Delete compressed files
        remove((char *) &treeFileName[0]);
        remove((char *) &encodedFileName[0]);
        std::cout << "DONE!\n";
    }

    // -----------------------------------------------> COMPRESSION
    else
    {
        int width, height, size;
        std::string fileName= argv[1];
        std::ifstream pgmFile(fileName);
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
        std::ofstream frqfile(fileName.replace(fileName.end()-3,fileName.end(),"frq"));
        frequencyTable.writeFreq(frqfile);
        frqfile.flush();
        frqfile.close();

        // Generate Encoded file
        std::ofstream encFile(fileName.replace(fileName.end()-3,fileName.end(),"enc"),std::ios::binary);
        encFile.write((char *) &width, sizeof(width));
        encFile.write((char *) &height, sizeof(height));
        huffTree.encodeFile(encFile,pixels);
        encFile.flush();
        encFile.close();
        remove((char *) argv[1]);
        std::cout << "DONE!\n";
    }
}