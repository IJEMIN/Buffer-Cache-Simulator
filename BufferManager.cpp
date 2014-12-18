//
//  BufferManager.cpp
//  Buffer Cache Simulator
//
//  Created by Jemin Lee on 2014. 12. 16..
//  Copyright (c) 2014년 Jemin Lee. All rights reserved.
//

#include "BufferManager.h"

BufferManager::BufferManager()
{
    //네개의 해시 테이블.
    Q_Table = new BufferQueue[4];
}

void BufferManager::AddNewBuffer(Buffer *newBuffer)
{
    //버퍼의 블록 번호에 따라 테이블로 분류.
    int mod = (newBuffer->blockNumber % 4);
    
    //분류된 테이블 큐에 추가.
    Q_Table[mod].enqueue(newBuffer);
    
    //새로운 버퍼 공간은 아직 프로세스가 사용하기 전이므로, 프리 리스트에 등록.
    freeBufferList.append(newBuffer);
}

void BufferManager::AddNewProcess(int requireBlockNumber)
{
    
    //새로운 프로세스를 생성
    Process * newProcess = new Process(requireBlockNumber);
    
    //새로운 프로세스를 프로세스 큐에 삽입
    //새로운 프로세스는 생성되면 초기 상태가 Ready로 결정된다.
    processList.push(newProcess);
}

bool BufferManager::RunProcess(Process *runningProcess)
{
    //만약 프로세스가 준비 상태가 아니라면
    if(!runningProcess->isReady())
    {
        return false;
    }
    
    //만약 빈 버퍼가 없다면.
    if(freeBufferList.getLength() == 0)
    {
        //슬립상태로 전환하고 리턴
        runningProcess->setProcessState(runningProcess->SLEEP);
        return false;
    }
    
    
    //요청된 버퍼를 프리 리스트로부터 호출.
    Buffer * requestedBuffer = freeBufferList.getMatchedBuffer(runningProcess->requestedBlockNumber);
    
    //만약 요청된 버퍼가 프리 리스트에 존재하지 않는다면.
    if (requestedBuffer == NULL)
    {
        //만약 요청된 버퍼가 해시 테이블에는 존재하는지 확인해본다.
        
        //먼저 해시로 부터 프로세스가 요구하는 버퍼와 동일한 주소를 가진 버퍼를 찾아낸다.
        Buffer * bufferFromHash = Q_Table[runningProcess->requestedBlockNumber%4].getMatchedBuffer(runningProcess->requestedBlockNumber);
        
        
        //만약 해당 버퍼가 해시 테이블에 존재는 하지만, 프리는 아닌 경우
        if (bufferFromHash !=NULL)
        {
            //해당 프로세스의 상태를 잠자기로 돌리고 종료한다.
            runningProcess->setProcessState(runningProcess->SLEEP);
            
            return false;
            
        }
        
        ///// 이 아래 부터는 프리 리스트에도, 해시 테이블에도 원하는 블록넘버를 가진 버퍼가 없는 경우 프리 리시트에 있는 버퍼 중 하나를 자신이 원하는 블록 넘버로 갱신하는 과정이다.
        
        
        
        //(지연쓰기가 아닌 버퍼 중)버퍼 리스트 중 빈 버퍼를 받아 온다.
        Buffer * bufferBeWriten = freeBufferList.getFirstFree();
        
        //도중에 지연쓰기인 버퍼들은 빈 버퍼를 가져오는 과정에서 디스크와 내용을 동기화 해준다.
        freeBufferList.syncBuffersWithDisk();

        //만약 프리 리스트 자체가 아예 비어있다면.
        //혹은 모든 프리 리스트의 버퍼가 현재 지연 쓰기 상태라면.
        if(bufferBeWriten == NULL)
        {
            runningProcess->setProcessState(runningProcess->SLEEP);
            return false;
        }
        
        //해당 빈 버퍼의 번호가 바뀌므로 해싱된 위치를 갱신해야 한다.
        
        //일단 기존에 속했던 해시 테이블에서 제외한다.
        Q_Table[bufferBeWriten->blockNumber%4].dequeue(bufferBeWriten);
        
        //해당 프로세스를 프리리스트로 부터 제한다.
        freeBufferList.deleteIndex(bufferBeWriten->blockNumber);
        
        //해당 버퍼를 사용한다.
        bufferBeWriten->blockNumber = runningProcess->requestedBlockNumber;
        
        
        
        runningProcess->Run(bufferBeWriten);
        
        
        //해당 버퍼는 현재 BUSY상태로 전환되었다.
        
        //새로운 블록 넘버에 알맞은 해시 테이블로 옮겨준다.
 
        Q_Table[bufferBeWriten->blockNumber%4].enqueue(bufferBeWriten);
        
        
        
        
        
        return true;

    }
    
    ///요청한 버퍼가 프리 리스트에 있는 경우
    
    runningProcess->Run(requestedBuffer);
    //해당 버퍼는 현재 BUSY상태로 전환되었다.
    
    //프리 리스트에서 제한다.
    freeBufferList.deleteIndex(requestedBuffer->blockNumber);
    
    
    return true;
}

bool BufferManager::EndProcess(Process *runningProcess)
{
    if(!runningProcess->isRunning())
        return false;
    
    //프리리스트로 들어갈 버퍼.
    Buffer * exitBuffer = runningProcess->usingBuffer;
    
    //만약 액세스 중이던 버퍼가 없다면. 에러.
    if(exitBuffer==NULL)
    {
        return false;
    }
    
    //프로세스 종료.
    runningProcess->Exit(exitBuffer);
    
    //종료된 프로세스가 가지고 있던 버퍼는 사용이 끝났으므로 프리 리스트로 더한다.
    //다만 내용이 변경되었고, 디스크와 동기화 되지 않았으므로, 지연쓰기 상태이다.
    freeBufferList.append(exitBuffer);
    
    return true;
}

bool BufferManager::ExecuteProcess(Process *runningProcess)
{
    ///프로세스가 끊임없이 동작하려고 시도하는 구역
    while(1)
    {
        //버퍼를 사용할 떄 까지 계속 루프.
        if(RunProcess(runningProcess))
            break;
        else
        {
            //프리한 버퍼가 아예 없거나, 사용할 블록 넘버를 가진 버퍼가 BUSY이면
            //ELSE (SLEEP 상태로) 진입
            while(1)
            {
                //프리한 버퍼를 건져냈다면
                if(freeBufferList.getMatchedBuffer(runningProcess->requestedBlockNumber)!=NULL)
                {
                    //프로세스 꺠우기.
                    runningProcess->awakeProcess();
                    break;
                }
            }
        }
        
    }
    
    /////// 프로세스가 종료되는 구역
    
    if(EndProcess(runningProcess))
        return true;
    
    
    return false;
}



void BufferManager::RunMultiProcess()
{
    
    /*
    //프로세스 수만큼 스레드 생성
    thread * threadArray = new thread[processList.size()];
    
    queue<Process*> processListForExtraction(processList);
    
    Process * runningProcess;
    
    int turn = 0;
    while(processList.empty())
    {
        runningProcess = processList.front();
        processList.pop();
        
        threadArray[turn] = thread(&this->ExecuteProcess,runningProcess);
    }
    
    for (int i = 0;i<processList.size();i++)
    {
        threadArray[turn].join();
    }
    */
    
    return ;
}

void BufferManager::PrintFreeList()
{
    freeBufferList.printFreeList();
    
}

void BufferManager::PrintHashTable()
{
    for (int i = 0; i<4; i++)
    {
        cout << i;
        Q_Table[i].printBuffersState();
    }
}