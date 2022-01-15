#ifndef CSF_ASSIGN03_CACHE_H
#define CSF_ASSIGN03_CACHE_H

#include <string>
#include <map>
#include <vector>

using std::string;

struct Block {
    string tag;
    bool dirty;
    unsigned loadTimestamp;
    unsigned accessTimestamp;
};

struct Set {
    std::vector<Block> set;
    std::map<string, Block*> index;
    unsigned count = 1;
    unsigned fifoCount = 0;
};

class Cache {
    public:
        Cache(unsigned numSets, unsigned numBlocks, unsigned numBytes, 
                string aType, string wType, string eType);

        void store(const string address);
        void load(const string address);
        void print_data();
        
    private:
        std::map<unsigned, Set> cacheSets;
        unsigned bytesPerBlock;
        unsigned numberOfSets;
        unsigned blocksPerSet;
        unsigned loadHits;
        unsigned loadMisses;
        unsigned storeHits;
        unsigned storeMisses;
        unsigned totalCycles;
        string allocType;
        string writeType;
        string evictType;

        void get_index_and_tag(const string address, unsigned& index, string& tag);
        unsigned get_lru_index(std::vector<Block>& set);
        void perform_lru_updates(std::vector<Block>& set, const unsigned index, const unsigned lruIndex, const string tag);
        unsigned get_fifo_index(std::vector<Block> &set);
        void perform_fifo_updates(std::vector<Block> &set, const unsigned index, const unsigned fifoIndex, const string tag);
};

#endif //CSF_ASSIGN03_CACHE_H
