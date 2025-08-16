#include <iostream>
#include "MemoryManager.hpp"
using namespace std;

int main() 
{
    size_t memorySize;
    cout<<"Enter total memory size: ";
    cin >> memorySize;

    int pChoice;
    cout<<"Choose replacement policy:\n"<<"1. FIFO\n"<<"2. LRU\n"<<"Enter choice: ";          
    cin >> pChoice;

    MemoryManager::Policy policy=(pChoice==1)
        ? MemoryManager::FIFO
        : MemoryManager::LRU;

    MemoryManager manager(memorySize, policy);

    int choice=0;
    do {
        cout<<"\n===== Memory Management Simulator =====\n";
        cout<<"1. Allocate Memory\n";
        cout<<"2. Deallocate Memory\n";
        cout<<"3. Display Memory Map\n";
        cout<<"4. Use Process (for LRU)\n";
        cout<<"5. Switch Policy (FIFO/LRU)\n";
        cout<<"6. Display Map LayOut\n";
        cout<<"7. Exit\n";
        cout<<"Enter choice: ";
        cin >> choice;
        cout<<endl;

        if (choice==1) 
        {
            int pid, size;
            cout<<"Enter process ID: ";
            cin >> pid;
            cout<<"Enter memory size to allocate: ";
            cin >> size;

            int start=manager.allocate(pid, size);
            if (start != -1)
                cout<<"Allocated P"<<pid<<" at "<<start<<"\n";
            else
                cout<<"Allocation failed (even after eviction) \n";
        }
        else if (choice==2) 
        {
            int pid;
            cout<<"Enter process ID to deallocate: ";
            cin >> pid;
            int freed=manager.deallocate(pid);
            if (freed > 0)
                cout<<"Deallocated "<<freed<<" cells from P"<<pid<<"\n";
            else
                cout<<"Process not found \n";
        }
        else if (choice==3) 
        {
            manager.displayMemory();
        }
        else if (choice==4) 
        {
            int pid;
            cout<<"Enter process ID to use: ";
            cin >> pid;
            manager.useProcess(pid);
        }
        else if (choice==5) 
        {
            int newP;
            cout<<"Switch to:\n1. FIFO\n2. LRU\nEnter: ";
            cin >> newP;
            manager.setPolicy(newP==1 ? MemoryManager::FIFO : MemoryManager::LRU);
            cout<<"Policy switched \n";
        }
        else if(choice==6)
        {
            manager.printMemoryLayout();
        }
        else if (choice != 7) 
        {
            cout<<"Invalid choice \n";
        }
        cout<<endl;
    } while (choice != 7);

    manager.del();
    cout<<"Exiting \n";
    return 0;
}
