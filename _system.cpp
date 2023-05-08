#include "_system.h"

_system::_system() : totalTime{ 0 }, targetVertex{ 0 },
targetQueueSize{ -1 }, statioanaryStep{ 0 }
{
	initVariables();
}

_system::_system(const double& iterations) : _system()
{
	this->iterations = iterations;
	isTimeTarget = true;
}

_system::_system(const int& vertex, 
	const int& queueSize) : _system()
{
	this->targetVertex = vertex;
	this->targetQueueSize = queueSize;
}

void _system::initVariables()
{
	std::random_device rd;
	this->random_seed = std::mt19937(rd());

	for (int i = 0; i < params.size(); ++i)
	{
		if (i < params.size() / 2)
		{
			streams.push_back(std::exponential_distribution<>(params[i]));
			distributions.push_back(getStream(i));
		}
		else
		{
			operators.push_back(std::exponential_distribution<>(params[i]));
			distributions.push_back(getOperator(i - params.size() / 2));
		}
	}
	for (int i = 0; i < limit; ++i)
		sizes[i] = 0;

	iterations = 1'000'00;
	isTimeTarget = false;
}

double _system::getStream(const int& vertex)
{
	return streams[vertex].operator()(this->random_seed);
}

double _system::getOperator(const int& vertex)
{
	return operators[vertex].operator()(this->random_seed);
}

const _system* _system::Engine()
{
	do
	{
		std::vector<int> noQueue;
		for (int i = 0; i < queues.size(); ++i)
			if (queues[i].size() == 0) 
				noQueue.push_back(i + limit);
		const double step = min(&noQueue);
		this->totalTime += step;
			lower(step, &noQueue);
		int index = getZeroIndex();

		if (targetVertex > 0 && targetQueueSize > -1 &&
			queues[targetVertex - 1].size() == targetQueueSize)
				this->statioanaryStep += step;

		if(targetVertex <= 0 && !isTimeTarget)
			checkSizes();

		if (isTimeTarget)
		{
			for (int i = 0; i < queues.size(); ++i)
				for (std::list<element>::iterator it = queues[i].begin(); it != queues[i].end(); ++it)
				{
					if (i == index && it == prev(queues[i].end()))
						continue;
					it->addTime(step);
				}
		}

		if (index >= 0 && index < limit)
		{
			element elem;
			elem.setVertex(index + 1);
			elem.addTime(step);
			arriveToQueue(elem);
			distributions[index] = getStream(index);
		}
		else if (index >= limit && queues[index - limit].size() != 0)
		{			
			operate(*(queues[index - limit].begin()), step, getRandInt);
			distributions[index] = getOperator(index - limit);
		}
	} while (totalTime <= iterations);
	return this;
#ifdef LOG
	cout << this->totalTime << endl;
#endif
}

void _system::arriveToQueue(element& elem)
{
	queues[elem.getVertex() - 1].push_back(elem);
}

void _system::operate(element& elem, const double& time, const int(*probability)())
{
	elem.addTime(time);
	switch (elem.getVertex())
	{
	case 1:
		if (probability() <= 25)							erase(elem);
		else if (probability() > 25 && probability() <= 50) move(elem, 2);
		else												move(elem, 4);
		break;
	case 2:
		if (probability() <= 33)							erase(elem);
		else if (probability() > 33 && probability() <= 66) move(elem, 3);
		else												move(elem, 1);
		break;
	case 3:
															move(elem, 1);
		break;
	case 4:
		if (probability() <= 50)							erase(elem);
		else												move(elem, 3);
		break;
	}
}

void _system::erase(element& elem)
{
	const int vertexNum = elem.getVertex() - 1;
	std::list<element>::iterator it = find(elem);

	if (it != this->queues[vertexNum].end())
		this->queues[vertexNum].erase(it);

	else throw std::exception("_system::erase crash\n");
}

void _system::move(element& elem, const int& vertexNum)
{
	queues[vertexNum - 1].push_back(element(elem, vertexNum));
	erase(elem);
}

void _system::checkSizes()
{
	for (auto i = 0; i < queues.size(); ++i)
		if (queues[i].size() > sizes[i])
			sizes[i] = queues[i].size();
}

const double _system::min(const std::vector<int>* arr)
{
	double minimum = 0;
	int d_size = distributions.size();
	if (d_size != 0)
	{
		minimum = distributions[0];
		for (int i = 0; i < d_size; ++i)
			if (distributions[i] < minimum && !ifContains(i, arr))
				minimum = distributions[i];
	}
	return minimum;
}

void _system::lower(const double& value, const std::vector<int>* arr)
{
	for (auto& item : distributions)
		if(!ifContains(&item - &distributions[0], arr))
			item -= value;
}

const int _system::getZeroIndex()
{
	for (int i = 0; i < distributions.size(); ++i)
		if (distributions[i] == 0) return i;

	return -1;
}

std::list<element>::iterator _system::find(const element& elem)
{
	std::list<element>::iterator end = queues[elem.getVertex() - 1].end();
	for (std::list<element>::iterator it = queues[elem.getVertex() - 1].begin(); it != end; ++it)
	{
		if (*it == elem) 
			return it;

		else if (it == prev(end)) 
			return end;
	}
}

void _system::print()
{
	for (int i = 0; i < limit; ++i)
	{
		cout << "VERTEX_" << i + 1 << endl;
		for (std::list<element>::iterator it = queues[i].begin(); it != queues[i].end(); ++it)
			cout << *it << endl;

		cout << "-----------" << endl;
	}
	cout << "------------" << endl;
}

const double _system::getStatioanaryProb() const
{
	return this->statioanaryStep / this->iterations;
}

const int _system::getMaxSize(int index) const
{
	return sizes[--index];
}

const double _system::getAverageLifeTime(const int& vertex) const
{
	return element::getLifeTime(vertex);
}

template<typename T1, typename T2>
const bool _system::ifContains(const T1& value, const T2* arr) const
{
	for (const auto& item : *arr)
		if (value == item) 
			return true;

	return false;
}

inline const int getRandInt()
{
	return rand() % 100 + 1;
}
