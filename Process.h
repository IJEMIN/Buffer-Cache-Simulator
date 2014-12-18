//
//  Process.h
//  Buffer Cache Simulator
//
//  Created by Jemin Lee on 2014. 12. 16..
//  Copyright (c) 2014년 Jemin Lee. All rights reserved.
//

#ifndef Buffer_Cache_Simulator_Process_h
#define Buffer_Cache_Simulator_Process_h


#include <iostream>
#include "Buffer.h"

using namespace std;

class Process
{
private:
    //프로세스 상태
    int state = 0;
    
public:
    //프로세스 식별자
    int id = 0;
    
    //프로세스 상태별 상수
    const int READY = 0;
    const int RUNNING = 1;
    const int SLEEP = 2;
    
    Buffer* usingBuffer;
    
    
    //프로세스가 요구하는 디스크 블록
    int requestedBlockNumber = 0;
    
    Process(int requireBlockNumber);
    //프로세스 상태 지정
    void setProcessState(int status);
    
    //프로세스 상태
    bool isReady();
    bool isRunning();
    bool isSleep();
    
    //프로세스 깨우기
    void awakeProcess();
    
    //프로세스 시작
    void Run(Buffer * buffer);
    
    //프로세스 종료
    void Exit(Buffer * buffer);
    
};

#endif
