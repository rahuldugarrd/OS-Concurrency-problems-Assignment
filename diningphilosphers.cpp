/*
 * reader-writer.cpp
 *
 *  Created on: Mar 31, 2019
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

std::vector<SEMAPHORE> S(5);
mutex lol;
int phil_array[5]={0,1,2,3,4};
void* philosophy(void* num) {
	int i=*((int *)num);
	while(true) {
		lol.lock();
		S[i].W();
		S[(i+1)%5].W();
		lol.unlock();
		cout<<"Philosopher "<<i<<" is dining. YUM! YUM!"<<endl;
		sleep(2);
		S[i].S();
		S[(i+1)%5].S();
		sleep((rand())%5);
	}
}

int main() {
	srand(time(0));
	pthread_t threadnum[5];
	for(int i=0; i<5; i++)
		pthread_create(&threadnum[i],NULL,philosophy,&phil_array[i]);
	for(int i=0; i<5; i++)
		pthread_join(threadnum[i],NULL);
}
