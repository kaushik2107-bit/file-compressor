#include "huffman.h"
#include <cstring>
#include <cassert>
#include <iostream>

void huffman::create_node_array() {
    for (int i=0; i<MAX_SIZE; i++) {
        node_arr[i] = new huffman_node;
        node_arr[i]->id = i;
        node_arr[i]->freq = 0;
    }
}

void huffman::traverse(huffman_node* node, std::string code) {
    if (node->left == NULL && node->right == NULL) {
        node->code = code;
    } else {
        traverse(node->left, code + '0');
        traverse(node->right, code + '1');
    }
}

int huffman::binary_to_decimal(std::string& in) {
    int res = 0;
    for (int i=0; i<in.size(); i++)
        res = res * 2 + in[i] - '0';
    return res;
}

std::string huffman::decimal_to_binary(int in) {
    std::string temp = "", res = "";
    while (in) {
        temp += ('0' + in % 2);
        in /= 2;
    }
    res.append(8 - temp.size(), '0');
    for (int i=temp.size()-1; i>=0; i--) {
        res += temp[i];
    }
    return res;
}

inline void huffman::build_tree(std::string& path, char a_code) {
    huffman_node* current = root;
    for (int i=0; i<path.size(); i++) {
        if (path[i] == '0') {
            if (current->left == NULL)
                current->left = new huffman_node;
            current = current->left;
        } else if (path[i] == '1') {
            if (current->right == NULL)
                current->right = new huffman_node;
            current = current->right;
        }
    }
    current->id = a_code;
}

huffman::huffman(std::string in, std::string out) {
    in_file_name = in;
    out_file_name = out;
    create_node_array();
}

void huffman::create_pq() {
    in_file.open(in_file_name, std::ios::in);
    in_file.get(id);

    while (!in_file.eof()) {
        node_arr[id]->freq++;
        in_file.get(id);
    }
    in_file.close();
    for (int i=0; i<MAX_SIZE; i++) {
        if (node_arr[i]->freq)
            pq.push(node_arr[i]);
    }
}

void huffman::create_huffman_tree() {
    std::priority_queue<huffman_node*, std::vector<huffman_node*>, compare> temp(pq);
    while (temp.size() > 1) {
        root = new huffman_node;
        root->freq = 0;
        root->left = temp.top();
        root->freq += temp.top()->freq;
        temp.pop();
        root->right = temp.top();
        root->freq += temp.top()->freq;
        temp.pop();
        temp.push(root);
    }
}

void huffman::calculate_huffman_codes() {
    traverse(root, "");
}

void huffman::coding_save() {
    in_file.open(in_file_name, std::ios::in);
	out_file.open(out_file_name, std::ios::out | std::ios::binary);
	std::string in = "", s = "";

    in += (char)pq.size();
	std::priority_queue<huffman_node*, std::vector<huffman_node*>, compare> temp(pq);
	while (!temp.empty()) {
		huffman_node* current = temp.top();
		in += current->id;
		s.assign(127 - current->code.size(), '0');
		s += '1';
		s.append(current->code);
        std::string aaa = s.substr(0, 8);
		in += (char)binary_to_decimal(aaa);										
		for (int i = 0; i < 15; i++) {
			s = s.substr(8);
            std::string aaa = s.substr(0, 8);
			in += (char)binary_to_decimal(aaa);
		}
		temp.pop();
	}
	s.clear();

    in_file.get(id);
	while (!in_file.eof()) {
		s += node_arr[id]->code;
		while (s.size() > 8) {
            std::string aaa = s.substr(0, 8);
			in += (char)binary_to_decimal(aaa);
			s = s.substr(8);
		}
		in_file.get(id);
	}

	int count = 8 - s.size();
	if (s.size() < 8) {
		s.append(count, '0');
	}

	in += (char)binary_to_decimal(s);															//save number of 'count' at last
	in += (char)count;

	out_file.write(in.c_str(), in.size());
	in_file.close();
	out_file.close();

}

void huffman::recreate_huffman_tree() {
    in_file.open(in_file_name, std::ios::in | std::ios::binary);
	unsigned char size;																			//unsigned char to get number of node of humman tree
	in_file.read(reinterpret_cast<char*>(&size), 1);
	root = new huffman_node;
    for (int i=0; i<size; i++) {
        char a_code;
		unsigned char h_code_c[16];																//16 unsigned char to obtain the binary code
		in_file.read(&a_code, 1);
		in_file.read(reinterpret_cast<char*>(h_code_c), 16);
		std::string h_code_s = "";
        for (int i=0; i<16; i++) {
            h_code_s += decimal_to_binary(h_code_c[i]);
        }
        int j = 0;
        while (h_code_s[j] == '0') {
			j++;
		}
		h_code_s = h_code_s.substr(j + 1);
		build_tree(h_code_s, a_code);
    }
    in_file.close();
}

void huffman::decoding_save() {
    in_file.open(in_file_name, std::ios::in | std::ios::binary);
	out_file.open(out_file_name, std::ios::out);
	unsigned char size;																		//get the size of huffman tree
	in_file.read(reinterpret_cast<char*>(&size), 1);
	in_file.seekg(-1, std::ios::end);															//jump to the last one byte to get the number of '0' append to the string at last
	char count0;
	in_file.read(&count0, 1);
	in_file.seekg(1 + 17 * size, std::ios::beg);

    std::vector<unsigned char> text;
    unsigned char textseg;
    in_file.read(reinterpret_cast<char*>(&textseg), 1);
    while (!in_file.eof()) {
		text.push_back(textseg);
		in_file.read(reinterpret_cast<char*>(&textseg), 1);
	}
	huffman_node* current = root;
	std::string path;
	for (int i = 0; i < text.size() - 1; i++) {
		path = decimal_to_binary(text[i]);
		if (i == text.size() - 2)
			path = path.substr(0, 8 - count0);
		for (int j = 0; j < path.size(); j++)
		{
			if (path[j] == '0')
				current = current->left;
			else
				current = current->right;
			if (current->left == NULL && current->right == NULL)
			{
				out_file.put(current->id);
				current = root;
			}
		}
	}
	in_file.close();
	out_file.close();
}

void huffman::encode() {
    create_pq();
    create_huffman_tree();
    calculate_huffman_codes();
    coding_save();
}

void huffman::decode() {
    recreate_huffman_tree();
    decoding_save();
}