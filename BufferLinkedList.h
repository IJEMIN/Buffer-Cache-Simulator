//
//  BufferLinkedList.h
//  Buffer Cache Simulator
//
//  Created by Jemin Lee on 2014. 12. 16..
//  Copyright (c) 2014년 Jemin Lee. All rights reserved.
//


/*
 버퍼만을 위한 링크드 리스트.
 
 처음 버퍼와 리스트 마지막 버퍼가 연결되는 순환 링크드 리스트의 형태다.
 
 버퍼 자체에 이후, 이전 포인터를 내장하고 있기 때문에,
 템플릿을 사용하여 노드 구조체를 따로 만들지 않았다.
 */

#ifndef Buffer_Cache_Simulator_BufferLinkedList_h
#define Buffer_Cache_Simulator_BufferLinkedList_h


#include "Buffer.h"
#include <iostream>
using namespace std;

class BufferLinkedList{
public:
    
    //초기화
    BufferLinkedList();
    
    ~BufferLinkedList();
    
    void makeEmpty();
    
    bool isEmpty();
    
    //리스트 길이 반환
    int getLength();
    //리스트 헤드 반환
    Buffer* getHead();
    
    //새로운 버퍼의 주소를 리스트에 추가
    void append(Buffer* newBuffer);
    /*
     해당 버퍼가 FREE의 상태를 벗어난 경우, 해당 버퍼의 주소를 인덱스에서 제외시킴.
     실제 버퍼 공간을 아예 날릴 경우는 큐에서 삭제하도록.
     */
    void deleteIndex(int blockNumber);
    
    //부가 기능,동일한 블록 넘버를 가진 버퍼를 반환해준다.
    Buffer * getMatchedBuffer(int);
    
    //프리 리스트의 버퍼 중 지연쓰기가 아닌 녀석을 내보내준다.
    Buffer * getFirstFree();
    
    //프리 리스트의 버퍼 중 지연쓰기인 녀석들을 디스크와 동기화한다.
    void syncBuffersWithDisk();
    
    //현재 프리 리스트 정보를 출력
    void printFreeList();
private:
    Buffer* head;
    int length;
};

#endif
