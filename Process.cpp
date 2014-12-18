//
//  Process.cpp
//  Buffer Cache Simulator
//
//  Created by Jemin Lee on 2014. 12. 16..
//  Copyright (c) 2014년 Jemin Lee. All rights reserved.
//

#include "Process.h"

Process::Process(int requireBlockNumber)
{
    requestedBlockNumber = requireBlockNumber;
    
}

//프로세스 상태 지정
void Process::setProcessState(int status)
{
    state = status;
}

bool Process::isReady()
{
    if (state == READY)
        return true;
    else
        return false;
}

bool Process::isRunning()
{
    if (state == RUNNING)
        return true;
    else
        return false;
}

bool Process::isSleep()
{
    if (state == SLEEP)
        return true;
    else
        return false;
}

//프로세스 깨우기
void Process::awakeProcess()
{
    if (isSleep())
        setProcessState(READY);
}



//프로세스 시작
//만약 버퍼가 FREE상태가 아니라면 프로세스는 잔다.
void Process::Run(Buffer * buffer)
{
    //사용하는 버퍼의 주소를 저장한다. 나중에 외부에서 할당되었던 버퍼 주소 기록을 찾아낼 때 필요하다.
    usingBuffer = buffer;
    
    if(buffer->state==buffer->FREE)
    {
        setProcessState(RUNNING);
        buffer->state = buffer->BUSY;
    } else{
        //만약 버퍼가 사용중이거나 비갱신(지연쓰기)이라면,
        setProcessState(SLEEP);
    }
}

//프로세스 종료
void Process::Exit(Buffer * buffer)
{
    if(state != RUNNING)
        return ;
    
    setProcessState(READY);
    
    //해당 버퍼의 사용을 끝내면, 바로 풀어주지는 않고, 지연쓰기 상태로 만들어 디스크 액세스를 줄인다.
    buffer->state = buffer->DELAY;
}