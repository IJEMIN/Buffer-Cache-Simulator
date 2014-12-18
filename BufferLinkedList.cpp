//
//  BufferLinkedList.cpp
//  Buffer Cache Simulator
//
//  Created by Jemin Lee on 2014. 12. 16..
//  Copyright (c) 2014년 Jemin Lee. All rights reserved.
//

#include "BufferLinkedList.h"

BufferLinkedList::BufferLinkedList()
{
    
        head = new Buffer;
        //헤드 버퍼를 잠금
        head->state = head->BUSY;
        
        //리스트의 헤드에 쓰레기값 투척
        head->blockNumber = 6666;
        
        //헤드의 리스트 포인터가 순환하여 스스로를 가리키게함.
        head->listNext = head;
        head->listPrev = head;
        
        //리스트 길이 초기화
        length = 0;
        

}

BufferLinkedList::~BufferLinkedList()
{
    delete head;
    
}

void BufferLinkedList::makeEmpty()
{
    //헤드의 리스트 포인터가 순환하여 스스로를 가리키게함.
    head->listNext = head;
    head->listPrev = head;
}

bool BufferLinkedList::isEmpty()
{
        if (length == 0)
            return true;
        else
            return false;
}

int BufferLinkedList::getLength()
{
    return length;
}

Buffer * BufferLinkedList::getHead()
{
        return head;
}

void BufferLinkedList::append(Buffer* newBuffer)
{
    
    Buffer * tmpHeadNext = head->listNext;
    
    //링크 작업
    
    //리스트 헤더 바로 다음에 새로운 버퍼 삽입.
    head->listNext = newBuffer;
    newBuffer->listPrev = head;
    
    //원래 리스트 헤더의 바로 다음은, 새로운 버퍼의 뒤로 밀려난다.
    newBuffer->listNext = tmpHeadNext;
    tmpHeadNext->listPrev = newBuffer;
    
    length++;
}

void BufferLinkedList::deleteIndex(int blockNumber)
{
    //탐색의 처음 지점은 바로 헤더 다음.
    Buffer * currentBuffer = head->listNext;
    
    //순환하면서 검색. 만약 헤더로 돌아오면 순환이 끝난것이며 해당하는 블록이 존재 하지 않는 것이다.
    
    while (currentBuffer != head)
    {
        
        if (currentBuffer->blockNumber == blockNumber)
        {
            //만약 제외하려는 버퍼를 찾았다면
            
            //버퍼 제외 및 제외할 버퍼의 양옆 버퍼들 연결 하기 시작.
            
            //해당 버퍼의 전 후 버퍼 주소를 가져온다.
            Buffer* currentsNext = currentBuffer->listNext;
            Buffer* currentsPrev = currentBuffer->listPrev;
            
            //제외할 버퍼의 다음 버퍼의 이전은 제외할 버퍼의 이전 버퍼가 된다.
            currentsNext->listPrev = currentsPrev;
            //제회할 버퍼의 이전 버퍼의 다음은 제외할 버퍼의 다음 버퍼가 된다.
            currentsPrev->listNext = currentsNext;
            
            //연결 끝.
            return ;
        }
        else
        {
            //만약 제외하려는 버퍼가 현재 버퍼가 아니라면
            
            currentBuffer = currentBuffer->listNext;
            
        }
        
        //만약 헤더로 돌아온다면 순환은 종료된다.
    }
}

Buffer * BufferLinkedList::getMatchedBuffer(int blockNumber)
{
    //탐색의 처음 지점은 바로 헤더 다음.
    Buffer * currentBuffer = head->listNext;
    
    while (currentBuffer != head)
    {
        
        if (currentBuffer->blockNumber == blockNumber)
        {
            return currentBuffer;
        }
        else
        {
            //만약 현재 버퍼가 아니라면
            
            currentBuffer = currentBuffer->listNext;
            
        }
        
        //만약 헤더로 돌아온다면 순환은 종료된다.
    }
    
    //만약 못찾았다면.
    return NULL;

}

Buffer * BufferLinkedList::getFirstFree()
{
    //탐색의 처음 지점은 바로 헤더 다음.
    Buffer * currentBuffer = head->listNext;
    
    while (currentBuffer != head)
    {
        //빈 버퍼가 맞다면
        if (currentBuffer->state == currentBuffer->FREE)
        {
            return currentBuffer;
        }
        else
        {
            //만약 현재 버퍼가 아니라면
            
            currentBuffer = currentBuffer->listNext;
            
        }
        
        //만약 헤더로 돌아온다면 순환은 종료된다.
    }
    
    //만약 못찾았다면.
    return NULL;
}

void BufferLinkedList::syncBuffersWithDisk()
{
    //탐색의 처음 지점은 바로 헤더 다음.
    Buffer * currentBuffer = head->listNext;
    
    while (currentBuffer != head)
    {
        //만약 지연 상태 버퍼라면
        if (currentBuffer->state == currentBuffer->DELAY)
        {
            //쓰기 상태로 바꾼다.
            currentBuffer->state = currentBuffer->WRITE;
            
            //쓰기 상태와 지연 상태 해제.
            currentBuffer->state = currentBuffer->FREE;
        }

        //다음 아이템으로 넘어간다.
            currentBuffer = currentBuffer->listNext;
        
        //만약 헤더로 돌아온다면 순환은 종료된다.
    }
    
}

void BufferLinkedList::printFreeList()
{
    cout << "FREE BUFFER LIST: ";
    if (length ==0 )
    {
        cout << "NONE";
        return;
    }
    
    Buffer* currentBuffer = head->listNext;
    
    while (currentBuffer != head)
    {
        cout << "<" << currentBuffer->blockNumber << ">";
        
            
            currentBuffer = currentBuffer->listNext;
            
        }
        
        //만약 헤더로 돌아온다면 순환은 종료된다.

    cout << endl;
}
