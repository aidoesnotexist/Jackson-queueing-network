#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "element.h"

#include <iostream>
#include <list>
#include <vector>
#include <random>
#include <chrono>
#include <array>

//#define LOG
#define limit 4

using std::cout;
using std::cin;
using std::endl;

class _system
{
	//Variables
	std::mt19937 random_seed;
	double totalTime;
	double iterations;

	//Exponential distributed streams/Operators
	std::vector<std::exponential_distribution<>> streams;
	std::vector<std::exponential_distribution<>> operators;

	//Input parameters
	std::array<double, 2 * limit> params { 1., 1., 1., 1., //<- [5.4 | 2.4 | 3.64 | 3.7]
											(38./7.)*7., (33./14.)*4., (51./14.)*5., (26./7.)*4.};

	//Arrays
	std::array<std::list<element>, limit> queues;
	std::vector<double> distributions;
	std::array<int, limit> sizes;

	int targetVertex;
	int targetQueueSize;
	double statioanaryStep;

	bool isTimeTarget;

	//Private methods

	void initVariables();

	//Generates time for a Stream next element occurrence/Operating time
	double getStream(const int&);
	double getOperator(const int&);

	/*Function: simulates getting into a queue

		-Adds an element to the queue dependin' on vertex number
	*/
	void arriveToQueue(element&);

	/*
		-Randomly moves the element to a certain vertex or
		 removes it from system
	*/
	void operate(element&, const double&, const int(*probability)());

	//Will erase an element from its vertex
	void erase(element&);

	//Will move the element to the vertex
	void move(element&, const int&);

	//Checks the sizes of the queues
	void checkSizes();

	//Finds the minimum among streams and operators
	//Returns minimum and where it's been found, 0 - streams 1 - operators
	const double min(const std::vector<int>*);
	//Decreases all current_streams
	void lower(const double&, const std::vector<int>*);

	//Returns the index of zero in current_streams
	//It can only be one zero at a time
	const int getZeroIndex();

public:
	//Constructors/Destructors
	_system();
	_system(const double&);
	_system(const int&, const int&);
	~_system() {};

	//Main algorithm
	const _system* Engine();

	//Finds the element's list<element>::iterator
	std::list<element>::iterator find(const element&);

	void print();

	//Returns the stationary probability of a vertex 
	//being in a certain state (queue length)
	const double getStatioanaryProb() const;
	//Returns max size of a queue
	const int getMaxSize(int) const;
	//Returns the average lifetime of an element depending on
	//which vertex it originaly came in
	const double getAverageLifeTime(const int&) const;

	template<typename T1, typename T2>
	const bool ifContains(const T1&, const T2*) const;
};

inline const int getRandInt();
#endif
