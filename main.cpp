#include <iostream>
#include "Engine.cpp"
#include <stdio.h>
#include <string.h>

using namespace std;

int main(int argc, char* argv[])
{
    
    bool compress = false;
    bool decompress = false; 
    bool decompressDebug = false;
    string inputFile;
    string outputFile;


    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "-compress") == 0)
        {
            compress = true;
        }

        if(strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "-decompress") == 0)
        {
            decompress = true;
        }

        if(strcmp(argv[i], "-dd") == 0 || strcmp(argv[i], "-ddecompressdebug") == 0)
        {
            decompressDebug = true;
        }

        if(strcmp(argv[i], "-i") == 0)
        {
            inputFile = argv[i+1];
        }

        if(strcmp(argv[i], "-o") == 0)
        {
            outputFile = argv[i+1];
        }

    }

    if(compress)
    {
        Engine tocompress; 
        tocompress.compress(inputFile, outputFile);
    }

    if(decompress)
    {
        Engine todecompress;
        todecompress.decompress(inputFile, "dataFile.txt", outputFile);
    }

    if(decompressDebug)
    {
        Engine todecompressDebug; 
        todecompressDebug.decompressDebug(inputFile);
    }


    return 0;
}