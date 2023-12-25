#pragma once

#include <string>
#include <unordered_map>
#include <queue>
#include <fstream>
#include <limits>
#include <vector>

struct huffman_node {
    char id;
    int freq;
    std::string code;
    huffman_node* left;
    huffman_node* right;
    huffman_node(): left(NULL), right(NULL) {}
};

class compare {
public:
    bool operator()(const huffman_node* c1, const huffman_node* c2) {
        return c1->freq > c2->freq;
    }
};

constexpr auto MAX_SIZE = 256;

class huffman {
protected:
    std::unordered_map<char, huffman_node*> node_arr;
    std::fstream in_file, out_file;
    huffman_node *child, *parent, *root;
    char id;
    std::string in_file_name, out_file_name;
    std::priority_queue<huffman_node*, std::vector<huffman_node*>, compare> pq;
    
    void create_node_array();
    void traverse(huffman_node*, std::string);
    int binary_to_decimal(std::string&);
    std::string decimal_to_binary(int);
    inline void build_tree(std::string&, char);

public:
    huffman(std::string, std::string);
    void create_pq();
    void create_huffman_tree();
    void calculate_huffman_codes();
    void coding_save();
    void decoding_save();
    void recreate_huffman_tree();

    
    void encode();
    void decode();
};