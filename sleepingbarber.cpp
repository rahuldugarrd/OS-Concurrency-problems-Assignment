/*
 * sleepingbarber.cpp
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
SEMAPHORE barber(0);
SEMAPHORE getseats(1);
SEMAPHORE customers(0);
int freeseats=3;
void *Barber(void *b) {
	while(true) {
		customers.W();
		getseats.W();
		cout<<"Barber cutting hair now."<<endl;
		cout<<"Available free seats:"<<freeseats<<endl;
		sleep(1);
		freeseats++;
		barber.S();
		getseats.S();
	}
}
void *Customer(void *b) {
	while(true) {
		getseats.W();
		if(freeseats>0) {
			cout<<"New Customers arriving! $_$"<<endl;
			cout<<"Available free seats:"<<freeseats<<endl;
			sleep(1);
			freeseats--;
			customers.S();
			getseats.S();
			barber.W();
		} else {
			getseats.S();
		}
	}
}

int main() {
	pthread_t thread_id[5];
	for(int i=0; i<4; i++)
		pthread_create(&thread_id[i],NULL,Customer,NULL);
	pthread_create(&thread_id[4],NULL,Barber,NULL);
	for(int i=0; i<5; i++)
		pthread_join(thread_id[i],NULL);
}

