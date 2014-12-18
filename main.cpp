//
//  main.cpp
//  Buffer Cache Simulator
//
//  Created by Jemin Lee on 2014. 12. 16..
//  Copyright (c) 2014년 Jemin Lee. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include "BufferManager.h"

using namespace std;
int main() {
    
    BufferManager newBufferManager;
    
    Buffer * buffer;
    
    
    srand((unsigned int)time(NULL));
    
    queue<Process*> sleepProcessList;
    
    /////버퍼 100개추가
    for (int i = 0; i<80;i++)
    {
        buffer = new Buffer;
        buffer->blockNumber = i;
        
        newBufferManager.AddNewBuffer(buffer);
    }
    
    
    
    ///프로세스 100개 추가. 사용하는 디스크 블록은 50개
    for (int i = 0; i<100; i++)
    {
        newBufferManager.AddNewProcess(rand()%50);
        
    }
    queue<Process*> processListCopy(newBufferManager.processList);
    queue<Process*> processAleadyRunnedCopy(newBufferManager.processList);
    for (int i =0; i<100;i++)
    {
        cout << endl;
        cout << "REQUESTED BLOCK NUM: " << processListCopy.front()->requestedBlockNumber << endl;
        if(!newBufferManager.RunProcess(processListCopy.front()))
        {
            processListCopy.front()->setProcessState(processListCopy.front()->SLEEP); //만약 프로세스가 러닝에 실패하면 슬립으로 간다.
        
            //그리고 슬립 큐에 추가.
            sleepProcessList.push(processListCopy.front());
            
            
        }
        
        newBufferManager.PrintFreeList();
        newBufferManager.PrintHashTable();
        processListCopy.pop();
        cout << endl;
    }
    
    cout << "////////////////////////////" << endl << endl;
    cout << "SLEEPING PROCESSES INFO" << endl;
    
    cout << "NUMBER OF SLEEPING PROCESSES: ";
    cout << sleepProcessList.size() << endl;
    
    
    cout << "////////////////////////////" << endl << endl;
    
    cout << "FROM HERE EXIT ALL RUNNING PROCESSES" << endl << endl;
    
    
    for (int i=0; i<100;i++)
    {
        newBufferManager.EndProcess(processAleadyRunnedCopy.front());
        processAleadyRunnedCopy.pop();
        newBufferManager.PrintFreeList();
        newBufferManager.PrintHashTable();
    }
}