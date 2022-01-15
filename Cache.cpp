#include "Cache.h"
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;
using std::string;
using std::vector;

Cache::Cache(unsigned numSets, unsigned numBlocks, unsigned numBytes, string aType, string wType, string eType) {
    for (unsigned index = 0; index < numSets; index++) {
        for (unsigned j = 0; j < numBlocks; j++) {
            Block block;
            block.dirty = false;
            block.accessTimestamp = 0;
            block.loadTimestamp = j;
            block.tag = "";
            cacheSets[index].set.push_back(block);
        }
    }
    numberOfSets = numSets;
    blocksPerSet = numBlocks;
    bytesPerBlock = numBytes;
    loadHits = 0;
    loadMisses = 0;
    storeHits = 0;
    storeMisses = 0;
    totalCycles = 0;
    allocType = aType;
    writeType = wType;
    evictType = eType;
}

void Cache::load(const string address) {
    unsigned index = 0;
    string tag = "";
    this->get_index_and_tag(address, index, tag);
    vector<Block> &set = cacheSets[index].set;
    std::map<string, Block *>::iterator it;
    it = cacheSets[index].index.find(tag);
    if (it != cacheSets[index].index.end()) { // was there a load hit?
        Block *b = it->second;
        b->accessTimestamp = cacheSets[index].count;
        cacheSets[index].count++;
        loadHits++;
        totalCycles++;
    } else {
        loadMisses++;
        totalCycles += (bytesPerBlock / 4) * 100;
        if (evictType == "lru") {
            unsigned lruIndex = this->get_lru_index(set);
            this->perform_lru_updates(set, index, lruIndex, tag);
            if (set.at(lruIndex).dirty && writeType == "write-back") {
                totalCycles += (bytesPerBlock / 4) * 100;
            }
            set.at(lruIndex).dirty = false;
        } else {
            unsigned fifoIndex = this->get_fifo_index(set);
            this->perform_fifo_updates(set, index, fifoIndex, tag);
            if (set.at(fifoIndex).dirty && writeType == "write-back") {
                totalCycles += (bytesPerBlock / 4) * 100;
            }
            set.at(fifoIndex).dirty = false;
        }
    }
}

void Cache::store(const string address) {
    string tag = "";
    unsigned index = -1;
    this->get_index_and_tag(address, index, tag);
    vector<Block> &set = cacheSets[index].set;
    std::map<string, Block *>::iterator it;
    it = cacheSets[index].index.find(tag);
    if (it != cacheSets[index].index.end()) {
        Block *b = it->second;
        b->accessTimestamp = cacheSets[index].count;
        cacheSets[index].count++;
        storeHits++;
        (*b).dirty = true;
        if (writeType == "write-through") {
            totalCycles += 100;
        } else {
            totalCycles++;
        }
    } else {
        storeMisses++;
        if (allocType == "no-write-allocate") {
            totalCycles += 100; // immediately write to memory
        } else /* write-allocate */ {
            if (evictType == "lru") {
                if (writeType == "write-through") {
                    totalCycles += 100;
                }
                unsigned lruIndex = this->get_lru_index(set);
                this->perform_lru_updates(set, index, lruIndex, tag);
                if (set.at(lruIndex).dirty && writeType == "write-back") {
                    totalCycles += (bytesPerBlock / 4) * 100;
                    set.at(lruIndex).dirty = false;
                }
                totalCycles += (bytesPerBlock / 4) * 100;
            } else {
                unsigned fifoIndex = this->get_fifo_index(set);
                this->perform_fifo_updates(set, index, fifoIndex, tag);
                if (set.at(fifoIndex).dirty && writeType == "write-back") {
                    totalCycles += (bytesPerBlock / 4) * 100;
                    set.at(fifoIndex).dirty = false;
                }
                totalCycles += (bytesPerBlock / 4) * 100;
            }
        }
    }
}

void Cache::get_index_and_tag(const string address, unsigned &index, string &tag) {
    unsigned numOffsetBits = std::log2(bytesPerBlock);
    unsigned numIndexBits = std::log2(numberOfSets);
    unsigned numTagBits = 32 - numOffsetBits - numIndexBits;
    string indexString = "";
    for (unsigned i = 0; i < numTagBits + numIndexBits; i++) {
        if (i < numTagBits) {
            tag += address.at(i);
        } else {
            indexString += address.at(i);
        }
    }
    if (indexString == "") { // fully-associative cache case:
        index = 0;
    } else {
        index = stoi(indexString, 0, 2);
    }
}

void Cache::perform_lru_updates(std::vector<Block> &set, const unsigned index, const unsigned lruIndex, const string tag) {
    set.at(lruIndex).accessTimestamp = cacheSets[index].count;
    cacheSets[index].count++;
    cacheSets[index].index[tag] = &(set.at(lruIndex));
    cacheSets[index].index.erase(set.at(lruIndex).tag);
    set.at(lruIndex).tag = tag;
}

void Cache::perform_fifo_updates(std::vector<Block> &set, const unsigned index, const unsigned fifoIndex, const string tag) {
    set.at(fifoIndex).loadTimestamp = blocksPerSet + cacheSets[index].fifoCount;
    cacheSets[index].fifoCount++;
    cacheSets[index].index[tag] = &(set.at(fifoIndex));
    cacheSets[index].index.erase(set.at(fifoIndex).tag);
    set.at(fifoIndex).tag = tag;
}

unsigned Cache::get_lru_index(std::vector<Block> &set) {
    unsigned min = UINT16_MAX;
    unsigned j = 0;
    for (unsigned i = 0; i < set.size(); i++) {
        if (set.at(i).accessTimestamp == 0) { // a 0 timestamp means the block has not been used yet
            return i; // return its index so that it will get used instead of causing another block to be evicted
        } else if (set.at(i).accessTimestamp < min) {
            min = set.at(i).accessTimestamp;
            j = i;
        }
    }
    return j;
}

unsigned Cache::get_fifo_index(std::vector<Block> &set) {
    unsigned min = UINT16_MAX;
    unsigned j = 0;
    for (unsigned i = 0; i < set.size(); i++) {
        if (set.at(i).loadTimestamp == 0) {
            return i;
        } else if (set.at(i).loadTimestamp < min) {
            min = set.at(i).loadTimestamp;
            j = i;
        }
    }
    return j;
}

void Cache::print_data() {
    cout << "Total loads: " << loadHits + loadMisses << endl;
    cout << "Total stores: " << storeHits + storeMisses << endl;
    cout << "Load hits: " << loadHits << endl;
    cout << "Load misses: " << loadMisses << endl;
    cout << "Store hits: " << storeHits << endl;
    cout << "Store misses: " << storeMisses << endl;
    cout << "Total cycles: " << totalCycles << endl;
    cout << "Hit rate: " << (loadHits + storeHits) / (double)(loadMisses + storeMisses + loadHits + storeHits) << endl;
}
