//
//  Buffer.h
//  Buffer Cache Simulator
//
//  Created by Jemin Lee on 2014. 12. 16..
//  Copyright (c) 2014년 Jemin Lee. All rights reserved.
//


/*
 버퍼는 메모리 블록 넘버와 디스크에서 해당 블록이 가지는 데이터와 동기화된 데이터를 가지고 있다.
 프로세스는 디스크에 직접 액세스 하기 전 먼저 버퍼 리스트를 순환하면서 자신이 액세스하려는 블록이 이미 버퍼 리스트에 있는지 확인한다.
 이미 버퍼 리스트에 자신이 액세스 하려는 블록을 가지고 있는 버퍼가 있다면, 디스크가 아닌 해당 버퍼로 부터 데이터를 읽는다.
 
 없다면 디스크에서 원하는 블록을 읽은 후 버퍼 리스트에는 빈 버퍼 혹은 사용하지 않는 버퍼에 자신이 액세스 했던 디스크 블록 정보를 덮어씌운다.
 
 
 */


#ifndef Buffer_Cache_Simulator_Buffer_h
#define Buffer_Cache_Simulator_Buffer_h



struct Buffer{
    
    //동기화된 디스크 상의 블록 넘버
    int blockNumber = 0;
    
    //버퍼 상태
    int state = 0;
    
    //버퍼 상태 상수
    
    //버퍼가 사용중이지 않은 상태
    const int FREE = 0;
    
    // 프로세스에 의해 버퍼가 사용중인 상태
    const int BUSY = 1;
    
    //액세스 횟수를 줄이기 위해 버퍼의 내용과 디스크의 내용이 서로 다른 상태임에도 동기화 하지 않은 상태.
    const int DELAY = 2;
    
    //버퍼의 내용을 디스크에 동기화 시키고 있는 상태.
    const int WRITE = 3;
    
    //해시큐 포인터
    Buffer * qNext;
    Buffer * qPrev;
    
    //프리 리스트 포인터
    Buffer * listNext;
    Buffer * listPrev;
};


#endif
