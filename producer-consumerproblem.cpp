/*
 * producer-consumerproblem.cpp
 *
 *  Created on: Mar 30, 2019
 *      Author: rahul
 */

#include <bits/stdc++.h>
#include <atomic>
#include <unistd.h>
#include <pthread.h>
using namespace std;
struct SEMAPHORE {
	atomic<int> signal;
	mutex mute;
	SEMAPHORE(int start=1) :
			signal(start) {
	}
	void W() {
		while(true) {
			while(signal<=0);
			mute.lock();
			if(signal<=0) {
				mute.unlock();
				continue;
			}
			signal--;
			mute.unlock();
			break;
		}
	}
	void S() {
		signal++;
	}
};

struct item {
	int data;
};
#define BUFFER_SIZE 10
mutex lol;
SEMAPHORE full(0);
SEMAPHORE empty(BUFFER_SIZE);
item buffer[BUFFER_SIZE];
int st=0,endd=0;
int itemindex=0;

void produceItem() {
	int y=itemindex++;
	cout<<"Item produced: "<<y<<endl;
	buffer[st].data=y;
	st=(st+1)%BUFFER_SIZE;
	sleep(1);
}

void consumeitem() {
	cout<<"Item consumed: "<<buffer[endd].data<<endl;
	endd=(endd+1)%BUFFER_SIZE;
}

void Wait(atomic<int>& s) {
	while((s--)<=-1)
		s++;
}

void Signal(atomic<int>& s) {
	s++;
}

void *produce(void *lul) {
	while(true) {
		empty.W();
		lol.lock();
		produceItem();
		lol.unlock();
		full.S();
	}
}

void *consume(void *lul) {
	while(true) {
		full.W();
		lol.lock();
		consumeitem();
		lol.unlock();
		empty.S();
	}
}

int main() {
	srand(time(0));
	pthread_t prod[10];
	pthread_t cons[10];
	int noofp,noofc;
	cout<<"Enter no. of producers(preferably less than equal to 10):"<<endl;
	cin>>noofp;
	cout<<"Enter no. of consumers(preferably less than equal to 10):"<<endl;
	cin>>noofc;
	for(int i=0; i<min(10,noofp); i++)
		pthread_create(&prod[i],NULL,produce,NULL);
	for(int i=0; i<min(10,noofc); i++)
		pthread_create(&cons[i],NULL,consume,NULL);
	pthread_exit(NULL);
}
