#include <vector>
#include <unordered_map>

class MemoryManager 
{
public:
    enum Policy { FIFO, LRU };
    struct Block 
    {
        int start;
        int size; 
    };
    MemoryManager(int size, Policy p);
    int allocate(int processId, int size);
    int deallocate(int processId);
    void useProcess(int processId);
    void setPolicy(Policy p) { policy = p; }
    void displayMemory() const;
    void printMemoryLayout() const;
    void del();

private:

    std::vector<int> memoryMap;
    int totalSize;
    std::unordered_map<int, Block> blocks;  
    std::unordered_map<int, int> arrival;    
    std::unordered_map<int, int> lastUsed;    
    int timeCounter;          
    Policy policy;

    bool findContiguousFree(int need, int &startOut) const;
    void evictUntilFit(int requiredSize);
    void evictOneFIFO();
    void evictOneLRU();
    void defragment();
};


// for compile 
// g++ src/main.cpp src/MemoryManager.cpp -I include -o memory_manager

// for run
// memory_manager