/*
 * cigrattesmokers.cpp
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

SEMAPHORE tobaccopaper(0);
SEMAPHORE papermatches(0);
SEMAPHORE matchestobacco(0);
SEMAPHORE smoked(1);

void *agent(void *lul) {
	while(true) {
		smoked.W();
		int smoker=rand()%3;
		switch (smoker) {
		case 0:
			tobaccopaper.S();
			break;
		case 1:
			papermatches.S();
			break;
		case 2:
			matchestobacco.S();
			break;
		}
	}
}

void *SmokerA(void *lul) {
	while(true) {
		tobaccopaper.W();
		cout<<"Smoker A is smoking!"<<endl;
		sleep(2);
		smoked.S();
	}
}

void *SmokerB(void *b) {
	while(true) {
		papermatches.W();
		cout<<"Smoker B is smoking!"<<endl;
		sleep(2);
		smoked.S();
	}
}

void *SmokerC(void *lul) {
	while(true) {
		matchestobacco.W();
		cout<<"Smoker C is smoking!"<<endl;
		sleep(2);
		smoked.S();
	}
}
int main() {
	pthread_t thread_id[4];
	pthread_create(&thread_id[0],NULL,SmokerA,NULL);
	pthread_create(&thread_id[1],NULL,SmokerB,NULL);
	pthread_create(&thread_id[2],NULL,SmokerC,NULL);
	pthread_create(&thread_id[3],NULL,agent,NULL);
	for(int i=0; i<4; i++)
		pthread_join(thread_id[i],NULL);
}
