#include <stdexcept>
#include <ios>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <chrono>

typedef short sym;
typedef std::vector<sym> seq;
typedef std::pair<sym, sym> sympair;
typedef std::map<sym, sympair> codebook;
typedef std::string str;
typedef std::vector<int> freqmap;
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::milliseconds ms;


void replace_pair(seq &sequence, sympair mcp, sym current_code)
{
    int n = sequence.size();
    int i = 0;    
    int j = 0;
    while (i < n-1) {
        if (sequence[i] == mcp.first && sequence[i+1] == mcp.second) {
            sequence[j] = current_code;
            i += 2;
        } else {
            sequence[j] = sequence[i];
            i += 1;
        }
        j++;
    }
    if (i == n-1) {
        sequence[j] = sequence[i];
    }
    
    sequence.erase(sequence.begin()+j, sequence.end());
}

int to_index(sympair s, int n){
    return (int)s.first*n + (int)s.second;
}

std::pair<sympair, int> get_max2(freqmap f, int n) {
    int best_index = -1;
    int best_count = -1;
    for(int i = 0; i < (n+1)*(n+1); i++) {
        if (f[i] > best_count) {
            best_count = f[i];
            best_index = i;
        }
    }
    return {{best_index / n, best_index % n} , best_count};
}

sympair compute_mcp(seq &sequence, int current_code)
{
    //freqmap f((current_code+1)*(current_code+1), 0);
    static freqmap f(11000*11000, 0); //max vocab with a healthy margin
    std::fill(f.begin(), f.begin() + (current_code+1)*(current_code+1), 0);
    
    for(int i = 0; i < sequence.size()-1; i++) {
        sympair s(sequence[i], sequence[i+1]);
        f[to_index(s, current_code)]++;
    }
    auto mcpc = get_max2(f, current_code);
    
    if (mcpc.second < 2) {
        throw std::runtime_error("most frequent pair only appears twice");
    } else {
        return mcpc.first;
    }
}

void do_one_round(seq &sequence, codebook &codes, sym &current_code)
{
    sympair mcp = compute_mcp(sequence, current_code);
    replace_pair(sequence, mcp, current_code);
    codes.emplace(current_code, mcp);
    current_code++;
}

void make_vocab_size(seq &sequence, codebook &codes, int n)
{
    sym current_code = 300;
    int num_unique = std::set<sym>(sequence.begin(), sequence.end()).size();
    int m = n-num_unique;
    for(int i = 0; i < m; i++){

        auto t0 = Time::now();
        do_one_round(sequence, codes, current_code);
        auto t1 = Time::now();        
        ms d = std::chrono::duration_cast<ms>(t1-t0);
        
        printf("round %d of %d completed in %d ms with %d symbols\n", i+1, m, (int)d.count(), sequence.size());
    }
}

seq read_bytes(str filename)
{
    std::ifstream ifs(filename, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char> result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&result[0], pos);

    seq sequence(result.begin(), result.end());

    //now do some weird thing where we convert from signed 'ascii' to unsigned
    for(int i = 0; i < sequence.size(); i++) {
        if(sequence[i] < 0) {
            sequence[i] += 256;
        }
    }
    printf("\n");
    
    return sequence;
}

void write_encoded(seq &sequence, str filename)
{
    std::ofstream ofs(filename, std::ios::binary);
    std::for_each(sequence.begin(), sequence.end(), [&] (auto &sym) -> void {
            ofs << sym << std::endl;
        });
}

void write_codes(codebook &codes, str filename)
{
    std::ofstream ofs(filename, std::ios::binary);
    std::for_each(codes.begin(), codes.end(), [&] (auto &code) -> void {
            ofs << code.first << " " << code.second.first << " " << code.second.second << std::endl;
        });
}

void open_encode_and_save(
    str input_file,
    str output_file,
    str code_file,
    int vocab_size)
{
    codebook codes;
    seq sequence = read_bytes(input_file);
    make_vocab_size(sequence, codes, vocab_size);
    write_encoded(sequence, output_file);
    write_codes(codes, code_file);
}

int main(int argc, char *argv[])
{
    str input_file("enwik8");
    str output_file("encoded");
    str code_file("codes");
    int vocab_size = 10000;
    open_encode_and_save(input_file, output_file, code_file, vocab_size);
}
