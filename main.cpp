#include "_system.h"
#include "math.h"

void statioProb(const int& vertex, const int& queueSize)
{
	cout << _system(vertex, queueSize).Engine()->getStatioanaryProb() << endl;;
}

const int indicator(const _system& im, 
	const int& vertex, const int& queueLimit)
{
	return im.getMaxSize(vertex) <= queueLimit ? 1 : 0;
}

void queueMax(const int& iterations,
	const int& vertex, const int& queueLimit)
{
	std::vector<int> probability;
	static int sum = 0;

	for (int i = 0; i < iterations; ++i)
		probability.push_back(indicator(*_system().Engine(), vertex, queueLimit));

	for (const auto& item : probability)
		sum += item;

	cout << "P(queue <= " << queueLimit << ") = " << double(sum) / double(iterations) << endl;
}

void averageLifeTime(const double& iterations, const int& vertex)
{
	cout << _system(iterations).Engine()->getAverageLifeTime(vertex) << endl;
}

int main()
{
	//lambda = [38/7, 33/14, 51/14, 26/7]
	//lambda = [5.4 | 2.4 | 3.6 | 3.7]
	srand(static_cast<int>(time(NULL)));

	statioProb(1, 3);
	//queueMax(10, 1, 15);
	//averageLifeTime(1000, 2);

	system("pause");
	return 0;
}