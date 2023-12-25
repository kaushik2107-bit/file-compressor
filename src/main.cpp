#include <iostream>
#include <filesystem>

#include "misc.h"
#include "huffman.h"

int main(int argc, char** argv) {
    int encoding = -1;
    bool compare = false;
    int count = 0;
    for (int i=1; i<argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-e" || arg == "--encode") {
            encoding = 1; count++;
        } else if (arg == "-d" || arg == "--decoding") {
            encoding = 0; count++;
        } else if (arg == "-c" || arg == "--compare") {
            compare = true; count++;
        }
    }

    if (count != 1 || argc != (compare ? 4 : 3) || (encoding == -1 && compare == false)) {
        std::cout << "Usage: compressor.exe [options] files..." << std::endl;
        std::cout << "Options:\n"
                << "  -e <file>\t\t\t" << "Compress given file\n"
                << "  -d <file>\t\t\t" << "Decompress given file\n"
                << "  -c <file> <file>\t\t" << "Compare both files\n"
                << "  --encoding <file>\t\t" << "Compress given files\n"
                << "  --decoding <file>\t\t" << "Decompress given files\n"
                << "  --compare <file> <file>\t" << "Compare given files\n";
        exit(1);
    }

    std::string file1, file2;
    if (compare) {
        file1 = argv[2], file2 = argv[3];
    }
    
    if (encoding == 1) {
        std::string ot_file = gen_filename(8, ".txt");
        huffman h(argv[2], ot_file);
        h.encode();
        
        // output statistics
        long long in = std::filesystem::file_size(argv[2]);
        long long out = std::filesystem::file_size(ot_file);
        printf(" +------------------------------+----------------+\n");
        printf(" | Input File Size (bytes)      | %-11lld    |\n", in);
        printf(" | Compressed File Size (bytes) | %-11lld    |\n", out);
        printf(" +------------------------------+----------------+\n");
        printf(" | Compression Ratio            | %-12.2lf   |\n", in/(float)out);
        printf(" +------------------------------+----------------+\n");

    } else if (encoding == 0) {
        std::string re_file = gen_filename(8, ".txt");
        huffman h1(argv[2], re_file);
        h1.decode();
    } else if (compare) {
        if (compare_files(file1, file2)) {
            std::cout << "Files are identical" << std::endl;
        } else {
            std::cout << "Files are not identical" << std::endl;
        }
    }
    return 0;
}