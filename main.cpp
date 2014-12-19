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
#include <Windows.h>

using namespace std;
int main() {


	BufferManager newBufferManager;

	Buffer * buffer;


	srand((unsigned int)time(NULL));

	queue<Process*> sleepProcessList;

	int numberOfBuffer = 0;
	while (1)
	{
		system("cls");
		cout << "Size of Buffer Cache: ";

		//버퍼의 수


		cin >> numberOfBuffer;

		if (numberOfBuffer < 5)
		{
			cout << "too small." << endl;
		}
		else
		{
			break;
		}
		system("pause");

	}

	//프로세스의 수
	int numberOfProccess = 0;

	while (1)
	{
		system("cls");
		cout << "Numbers of Process: ";

		//버퍼의 수


		cin >> numberOfProccess;

		if (numberOfProccess < 1)
		{
			cout << "to little." << endl;
		}
		else
		{
			break;
		}
		system("pause");
	}



	//실제 프로세스가 요구할 디스크 블록의 랜덤 범위
	int numberOfBlockSpread = 0;

	while (1)
	{
		system("cls");
		cout << "Numbers of Disk(Storage) Blocks: ";

		//버퍼의 수


		cin >> numberOfBlockSpread;

		if (numberOfBlockSpread < numberOfBuffer)
		{
			cout << "Disk Size can't be small than Buffer" << endl;
		}
		else
		{
			break;
		}
		system("pause");
	}



	/////버퍼 추가
	for (int i = 0; i < numberOfBuffer; i++)
	{
		buffer = new Buffer;
		buffer->blockNumber = i;

		buffer->qNext = NULL;
		buffer->qPrev = NULL;
		buffer->listNext = NULL;
		buffer->listPrev = NULL;


		newBufferManager.AddNewBuffer(buffer);
	}



	///프로세스 추가하면서 동시에 프로세스가 요구할 블록을 랜덤하게 지정
	for (int i = 0; i < numberOfProccess; i++)
	{
		newBufferManager.AddNewProcess(rand() % numberOfBlockSpread);

	}
	queue<Process*> processListCopy(newBufferManager.processList);
	queue<Process*> processAleadyRunnedCopy(newBufferManager.processList);
	for (int i = 0; i < numberOfProccess; i++)
	{
		system("cls");
		cout << "NUBER OF PROCESSES: " << numberOfProccess << " || " << "NUMBER OF BUFFERS: " << numberOfBuffer << endl;
		cout << "RUNNING PHASE: " << i + 1 << endl;
		cout << endl;
		cout << "REQUESTED BLOCK NUM: " << processListCopy.front()->requestedBlockNumber << endl;
		if (!newBufferManager.RunProcess(processListCopy.front()))
		{
			processListCopy.front()->setProcessState(processListCopy.front()->SLEEP); //만약 프로세스가 러닝에 실패하면 슬립으로 간다.
			cout << "Failed, PROCESS GO TO SLEEP" << endl;

			//그리고 슬립 큐에 추가.
			sleepProcessList.push(processListCopy.front());


		}

		newBufferManager.PrintFreeList();
		newBufferManager.PrintHashTable();
		processListCopy.pop();
		cout << endl;

		Sleep(400);
	}

	cout << "////////////////////////////" << endl << endl;
	cout << "NUMBER OF SLEEPING PROCESSES: ";
	cout << sleepProcessList.size() << endl;
	cout << "////////////////////////////" << endl << endl;

	cout << "FROM HERE, END ALL RUNNING PROCESSES" << endl << "AND, AWAKE TRAPED(SLEEPING) PROCESS AT THE SAMETIME" << endl;



	system("pause");

	int phaseNum = 0;

	int stuckCounter = 0;
	while (!sleepProcessList.empty())
	{
		cout << system("cls");
		cout << endl;

		cout << "Closing Processes... Awake Traped Processes SameTime..." << endl;
		cout << "Left Traped Processes: " << sleepProcessList.size() << endl;
		cout << "End Phase:" << phaseNum << endl;

		if (!processAleadyRunnedCopy.empty())
		{

			newBufferManager.EndProcess(processAleadyRunnedCopy.front());
			processAleadyRunnedCopy.pop();
		}


		if (!sleepProcessList.empty())
		{
			stuckCounter++;
			//슬립된 프로세스들을 깨운다.
			sleepProcessList.front()->awakeProcess();
			int rNum = sleepProcessList.front()->requestedBlockNumber;

			if (newBufferManager.RunProcess(sleepProcessList.front()))
			{
				cout << "SLEEP PROCESS AWAKED!" << endl;
				cout << "Requested Block: " << rNum << endl;

				sleepProcessList.pop();

				stuckCounter = 0;
			}

			//DELAY가 너무많아 STUCK된 상태,.
			if (stuckCounter > 4)
				newBufferManager.syncWithDisk();
		}

		newBufferManager.PrintFreeList();
		newBufferManager.PrintHashTable();

		Sleep(400);
	}

	cout << "Left Traped Processes: " << sleepProcessList.size() << endl;
	cout << "ALL TRAPED PROCESSES AWAKEN" << endl;

	system("pause");
}