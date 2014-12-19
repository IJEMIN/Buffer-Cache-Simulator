//
//  BufferQueue.cpp
//  Buffer Cache Simulator
//
//  Created by Jemin Lee on 2014. 12. 16..
//  Copyright (c) 2014년 Jemin Lee. All rights reserved.
//

#include "BufferQueue.h"

BufferQueue::BufferQueue(){
	front = new Buffer;

	//헤드 잠금
	front->state = front->BUSY;

	//쓰레기값 투척
	front->blockNumber = 6666;

	front->qNext = NULL;
	front->qPrev = NULL;


}


BufferQueue::~BufferQueue()
{


}

void BufferQueue::makeEmpty()
{
	length = 0;
}



bool BufferQueue::isEmpty()
{
	if (length == 0)
	{
		return true;
	}
	return false;
}

bool BufferQueue::isFull()
{
	if (length >= 30)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//새로운 버퍼 주소를 Q 리스트의 마지막 지점에 추가.
void BufferQueue::enqueue(Buffer* newBuffer)
{
	if (isFull())
		return;

	int indexForQ = length;

	//링크드 리스트 큐의 제일 마지막 지점 까지 이동하여, 새로운 버퍼 주소를 삽입

	//링크드 리스트의 마지막 지점.
	Buffer * tmpListLast = front->qNext;
	//링크드 리스트의 마지막 지점 바로 앞.
	Buffer * pointerBeforeLast = front;

	//마지막 지점 까지 이동
	while (indexForQ > 0)
	{
		tmpListLast = tmpListLast->qNext;
		pointerBeforeLast = pointerBeforeLast->qNext;
		indexForQ--;
	}

	//마지막 지점에 추가된 버퍼와 연결하기
	tmpListLast = newBuffer;
	pointerBeforeLast->qNext = tmpListLast;
	tmpListLast->qPrev = pointerBeforeLast;


	length++;
}

//리스트 가장 앞쪽에 위치한 버퍼 주소를 삭제
void BufferQueue::dequeue()
{
	if (isEmpty())
		return;

	//가장 첫지점과 헤드와의 연결을 끊는다. 그리고 두번째 지점과 헤드를 연결
	front->qNext = front->qNext->qNext;
	front->qNext->qPrev = front;

	length--;
}

void BufferQueue::dequeue(Buffer *delBuffer)
{
	int indexForQ = length;

	//큐의 헤드 바로 이후부터 큐 끝까지 훓는 이터레이터
	Buffer * iterator = front->qNext;

	//이터레이터 바로 앞 지점.
	Buffer * beforeIterator = front;


	if (length == 1)
	{
		length = 0;
		return;
	}


	//마지막 지점 까지 이동
	while (indexForQ > 0)
	{
		//만약 삭제할 버퍼를 찾았다면,
		if (iterator == delBuffer)
		{
			//주소를 제하고 앞 뒤를 연결.
			beforeIterator->qNext = iterator->qNext;

			if (iterator->qNext != NULL)
			{
				iterator->qNext->qPrev = beforeIterator;
			}

			length--;
			return;
		}

		iterator = iterator->qNext;
		beforeIterator = beforeIterator->qNext;
		indexForQ--;
	}
}


Buffer * BufferQueue::getMatchedBuffer(Buffer * refBuffer){
	int indexForQ = length;


	//큐의 헤드 바로 이후부터 큐 끝까지 훓는 이터레이터
	Buffer * iterator = front->qNext;

	//이터레이터 바로 앞 지점.
	Buffer * beforeIterator = front;


	//마지막 지점 까지 이동
	while (indexForQ > 0)
	{
		//만약 해당 버퍼를 찾았다면
		if (iterator == refBuffer)
		{
			return iterator;
		}

		iterator = iterator->qNext;
		beforeIterator = beforeIterator->qNext;
		indexForQ--;
	}

	return NULL;
}

Buffer * BufferQueue::getMatchedBuffer(int blockNumber)
{
	int indexForQ = length;


	//큐의 헤드 바로 이후부터 큐 끝까지 훓는 이터레이터
	Buffer * iterator = front->qNext;

	//이터레이터 바로 앞 지점.
	Buffer * beforeIterator = front;


	//마지막 지점 까지 이동
	while (indexForQ > 0)
	{
		//만약 해당 버퍼를 찾았다면
		if (iterator->blockNumber == blockNumber)
		{
			return iterator;
		}

		iterator = iterator->qNext;
		beforeIterator = beforeIterator->qNext;
		indexForQ--;
	}

	return NULL;


}

//헤드 반환... 헤드 내부에는 헤드임을 알리는 쓰레기 값이 들어있고, 실질적인 버퍼는 front->qNext 부터.
Buffer * BufferQueue::getFront()
{
	return front;
}

void BufferQueue::printBuffersState()
{
	int indexForQ = length;


	//큐의 헤드 바로 이후부터 큐 끝까지 훓는 이터레이터
	Buffer * iterator = front->qNext;

	//이터레이터 바로 앞 지점.
	Buffer * beforeIterator = front;

	cout << " TABLE BUFFERS STATE:";

	//마지막 지점 까지 이동
	while (indexForQ > 0)
	{
		cout << "<" << iterator->blockNumber << ":";

		if (iterator->state == iterator->FREE)
		{
			cout << "FREE";
		}
		else{
			if (iterator->state == iterator->BUSY)
			{
				cout << "BUSY";
			}
			else{
				if (iterator->state == iterator->DELAY)
				{
					cout << "DELAY";
				}
				else{
					if (iterator->state == iterator->WRITE)
					{
						cout << "WRITE";
					}
				}
			}
		}

		cout << ">";
		iterator = iterator->qNext;
		beforeIterator = beforeIterator->qNext;
		indexForQ--;
	}

	cout << endl;
	return;
}