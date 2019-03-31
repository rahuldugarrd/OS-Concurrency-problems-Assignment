/*
 * reader-writer.cpp
 *
 *  Created on: Mar 30, 2019
 *      Author: rahul
 */
#include <bits/stdc++.h>
#include <atomic>
#include <unistd.h>
#include <mutex>
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

mutex lol;
SEMAPHORE w(1);
struct item {
	int data;
	string book;
};
#define BUFFER_SIZE 10
item Buffer[BUFFER_SIZE];
int itemindex=0;
int st=0,endd=0;
atomic<int> cnt{0};
void Write() {
	int item=itemindex++;
	int randdata=rand();
	cout<<"Data written to buffer:"<<randdata<<endl;
	sleep(1);
	Buffer[st].data=item;
	stringstream temp;
	temp<<randdata;
	temp>>Buffer[st].book;
	st=(st+1)%BUFFER_SIZE;
}
void Read() {
	string item=Buffer[endd].book;
	endd=(endd+1)%BUFFER_SIZE;
	cout<<"Data read from buffer:"<<item<<endl;
	sleep(1);
}
void *Write(void *lul) {
	while(true) {
		w.W();
		Write();
		w.S();
	}
}

void *Read(void *lul) {
	while(true) {
		lol.lock();
		cnt++;
		if(cnt==1)
			w.W();
		lol.unlock();
		Read();
		lol.lock();
		cnt--;
		if(cnt==0)
			w.S();
		lol.unlock();
	}
}
int main() {
	srand(time(NULL));
	pthread_t Reader;
	pthread_t Writer;
	pthread_create(&Writer,NULL,Write,NULL);
	pthread_create(&Reader,NULL,Read,NULL);
	pthread_exit(NULL);
}
