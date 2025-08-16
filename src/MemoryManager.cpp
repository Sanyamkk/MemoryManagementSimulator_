#include "MemoryManager.hpp"
#include <algorithm>
#include <iostream>
using namespace std;
MemoryManager::MemoryManager(int size, Policy p)
    : totalSize(size), timeCounter(0), policy(p) {
    memoryMap.assign(size, -1);
}

bool MemoryManager::findContiguousFree(int need, int &startOut) const 
{
    int run=0;
    int start=-1;
    for(int i=0; i<static_cast<int>(totalSize); ++i) 
    {
        if(memoryMap[i]==-1) 
        {
            if(run==0) start=i;
            ++run;
            if(run==need) 
            {
                startOut=start;
                return true;
            }
        } 
        else 
        {
            run=0;
            start=-1;
        }
    }
    return false;
}

int MemoryManager::allocate(int processId, int size) 
{
    if(size <= 0) return -1;
    int start=-1;
    if(!findContiguousFree(size, start)) 
    {
        evictUntilFit(size);
        if(!findContiguousFree(size, start)) 
        {
            return -1;
        }
    }
    for(int i=0; i<size; ++i) 
    {
        memoryMap[start + i]=processId;
    }
    blocks[processId]={ start, size };
    if(arrival.find(processId)==arrival.end()) 
    {
        arrival[processId]=timeCounter++;
    }
    lastUsed[processId]=timeCounter++;

    return start;
}

int MemoryManager::deallocate(int processId) 
{
    auto it=blocks.find(processId);
    if(it==blocks.end()) return 0;
    int start=it->second.start;
    int size =it->second.size;
    for(int i=0; i<size; ++i) 
    {
        memoryMap[start + i]=-1;
    }
    blocks.erase(processId);
    arrival.erase(processId);
    lastUsed.erase(processId);

    defragment();

    return size;
}

void MemoryManager::useProcess(int processId) 
{
    if(blocks.find(processId) != blocks.end()) 
    {
        lastUsed[processId]=timeCounter++;
        cout<<"Used process "<<processId<<"(time "<<lastUsed[processId]<<")\n";
    } 
    else 
    {
        cout<<"Process "<<processId<<" not in memory.\n";
    }
}

void MemoryManager::evictUntilFit(int requiredSize) 
{
    int dummyStart;
    int safety=100000;
    while(safety-- && !findContiguousFree(requiredSize, dummyStart) && !blocks.empty()) 
    {
        if(policy==FIFO)
        {
            evictOneFIFO();
        } 
        else
        {
            evictOneLRU();
        }
        defragment();
    }
}

void MemoryManager::evictOneFIFO() 
{
    if(arrival.empty()) return;
    int victim=-1;
    int oldest=1e9;

    for(const auto &p : arrival) 
    {
        if(p.second<oldest) 
        {
            oldest=p.second;
            victim=p.first;
        }
    }
    if(victim != -1) 
    {
        cout<<"[FIFO] Evicting process "<<victim<<"\n";
        deallocate(victim);
    }
}

void MemoryManager::evictOneLRU() 
{
    if(lastUsed.empty()) return;
    int victim=-1;
    int least =1e9;

    for(const auto &p : lastUsed) 
    {
        if(p.second<least) 
        {
            least =p.second;
            victim=p.first;
        }
    }
    if(victim != -1) 
    {
        cout<<"[LRU] Evicting process "<<victim<<"\n";
        deallocate(victim);
    }
}

void MemoryManager::defragment() 
{
    cout<<"MemoryMap Before Defragmentation : ";
    for(int cell : memoryMap) 
    {
        if(cell==-1)
        {
            cout<<".";
        } 
        else
        {
            cout<<cell;
        }
    }
    cout<<"\n";
    vector<pair<int,int>> order;
    order.reserve(blocks.size());
    for(const auto &kv : blocks) 
    {
        order.push_back({ kv.second.start, kv.first });
    }
    sort(order.begin(), order.end());
    vector<int> newMap(totalSize, -1);
    int cursor=0;

    for(auto &pair : order) 
    {
        int pid=pair.second;
        int sz =blocks[pid].size;
        for(int i=0; i<sz; ++i) 
        {
            newMap[cursor + i]=pid;
        }
        blocks[pid].start=cursor;
        cursor += sz;
    }

    memoryMap.swap(newMap);
    cout<<"[Defrag] Memory compacted.\n";
    cout<<"MemoryMap After Defragmentation : ";
    for(int cell : memoryMap) 
    {
        if(cell==-1)
        {
            cout<<".";
        } 
        else
        {
            cout<<cell;
        }            
    }
    cout<<"\n";
}

void MemoryManager::displayMemory() const 
{
    cout<<"Memory: ";
    for(int cell : memoryMap) 
    {
        if(cell==-1)
        {
            cout<<".";
        } 
        else
        {
            cout<<cell;
        }
    }
    cout<<"\n";
}

void MemoryManager::printMemoryLayout() const 
{
    cout<<"\nMemory Layout:\n";
    int n=memoryMap.size();
    int i=0;
    while(i<n) 
    {
        int pid=memoryMap[i];
        int j=i;
        while(j<n && memoryMap[j]==pid) 
        {
            j++;
        }
        if(pid==-1) 
        {
            cout<<"[Free x"<<(j - i)<<"] ";
        } 
        else 
        {
            cout<<"[P"<<pid<<" x"<<(j - i)<<"] ";
        }
        i=j;
    }
    cout<<"\n";
}

void MemoryManager::del() 
{
    memoryMap.clear();
    blocks.clear();
    arrival.clear();
    lastUsed.clear();
}
