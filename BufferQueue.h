//
//  BufferQueue.h
//  Buffer Cache Simulator
//
//  Created by Jemin Lee on 2014. 12. 16..
//  Copyright (c) 2014년 Jemin Lee. All rights reserved.
//
/*
 버퍼만을 위한 큐. 버퍼 내부에 이전 이후 포인터가 존재하므로 따로 노드를 제작하지 않는다.
 */

#ifndef Buffer_Cache_Simulator_BufferQueue_h
#define Buffer_Cache_Simulator_BufferQueue_h


#include "Buffer.h"
#include <iostream>

using namespace std;

class BufferQueue
{
public:
	BufferQueue();
	~BufferQueue();

	void makeEmpty();



	bool isEmpty();

	bool isFull();

	//새로운 버퍼 주소를 Q 리스트의 마지막 지점에 추가.
	void enqueue(Buffer* newBuffer);

	//리스트 가장 앞쪽에 위치한 버퍼 주소를 삭제
	void dequeue();

	//지정된 버퍼를 삭제
	void dequeue(Buffer* delBuffer);

	//아예 동일한 버퍼가 있는지 반환.
	Buffer * getMatchedBuffer(Buffer* refBuffer);

	//블록 넘버가 같은 버퍼를 반환.
	Buffer * getMatchedBuffer(int);

	//헤드 반환... 헤드임을 알리는 쓰레기 값이 들어있고, 실질적인 버퍼는 front->qNext 부터.
	Buffer * getFront();

	//버퍼들 목록과 상태를 죄다 프린트
	void printBuffersState();
private:
	Buffer * front;
	int length = 0;

};


#endif
