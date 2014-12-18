//
//  BufferManager.h
//  Buffer Cache Simulator
//
//  Created by Jemin Lee on 2014. 12. 16..
//  Copyright (c) 2014년 Jemin Lee. All rights reserved.
//
/*
 버퍼 매니저는 버퍼를 생성하고, 이들 중 빈 버퍼를 프로세스들이 이용하면서 사용후 버퍼의 데이터를 수정함을 가정하여, 지연쓰기로 지정한 뒤, 디스크 데이터와 일치하도록 동기화 한 후 빈 버퍼로 만드는 일체의 과정을 두개의 프로세스가 동시에 실행되는 멀티 프로세스 처리를 기반으로 시뮬레이션 하는 클래스이다. 인터페이스는 제외되어 있다.
 
 
 */

#ifndef __Buffer_Cache_Simulator__BufferManager__
#define __Buffer_Cache_Simulator__BufferManager__

#include <iostream>
#include "Process.h"
#include "BufferLinkedList.h"
#include "BufferQueue.h"
#include <list>
#include <queue>
#include <thread>

using namespace std;


class BufferManager{
private:

    
    //큐 테이블
    BufferQueue * Q_Table;
    
    //빈 버퍼의 주소를 모아담는 리스트.
    BufferLinkedList freeBufferList;
    
    //총 버퍼의 수. (빈 버퍼의 수가 아니다)
    int length;
    
public:
    
    //실행할 프로세스들의 큐 목록
    queue<Process*> processList;
    
    //생성자
    BufferManager();
    
    //초기화 과정.
    //새로운 버퍼 '공간'을 추가.
    void AddNewBuffer(Buffer* newBuffer);
    
    //새로운 프로세스를 큐에 더한다.
    //새로운 프로세스가 요구할 블럭 넘버를 입력으로 받는다.
    void AddNewProcess(int requireBlock);
    
    //하나의 프로세스를 실행한다.
    //해당 프로세스가 요청하는 블록을 가진 버퍼가 사용중이거나, 프리 리스트가 아예 없다면
    //Sleep으로 빠지고 false를 리턴한다.
    bool RunProcess(Process * runningProcess);
    
    //프로세스를 종료한다. 버퍼를 Delay 상태로 지정하고 프리 리스트로 돌린다.
    bool EndProcess(Process * runningProcess);
    
    //
    bool ExecuteProcess(Process * runningProcess);
    
    bool static NoEndingExecution(Process * runnningProcess);
    
    
    // 여러 프로세스를 동시 실행한다.
    // while문을 돌려, 프로세스가 sleep상태에 빠지지는 않았는지 체크하며
    // true를 반환하여 완벽하게 종료된 것으로 파악된 프로세스는 종료후,
    // 그 자리에 큐 다음번 프로세스를 할당하여 동작시킨다.
    void RunMultiProcess();
    
    //프리한 버퍼 리스트를 출력
    void PrintFreeList();
    
    void PrintHashTable();
    
    
};
#endif /* defined(__Buffer_Cache_Simulator__BufferManager__) */
