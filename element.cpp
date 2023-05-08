#include "element.h"

std::vector<std::vector<double>> element::lifeTime;
std::vector<int> element::originCounter{0, 0, 0, 0};
int element::number = 0;

element::element()
{
	initVariables();
	++number;
}

element::element(const element& elemOld) : vertex{ elemOld.vertex }, 
	originalVertex {elemOld.originalVertex}
{
	initVariables();
}
element::element(const element& elemOld, const int& vertexNum) : vertex{ vertexNum },
	originalVertex{ elemOld.originalVertex }
{
	initVariables();
}

void element::initVariables()
{
	
}

void element::setVertex(const int& vertex)
{
	this->vertex = vertex;
	this->originalVertex = vertex;
	++originCounter[vertex - 1];
	
}

const int element::getVertex() const
{
	return this->vertex;
}

void element::init_static_vectors()
{
	for (int i = 0; i < 4; ++i) 
		lifeTime.push_back(std::vector<double>());
}

void element::addTime(const double& time)
{
	static bool is_vec_initialized = false; 
	if (!is_vec_initialized)
	{
		init_static_vectors();
		is_vec_initialized = true;
	}

	lifeTime[originalVertex - 1].push_back(time);
}

double element::getLifeTime(const int& vertex_num)
{
	double sum = 0;
	for (const auto& item : lifeTime[vertex_num - 1])
		sum += item;

	return sum / (double)originCounter[vertex_num - 1];
}

bool operator==(const element& left, const element& right)
{
	return (left.vertex == right.vertex &&
		left.originalVertex == right.originalVertex);
}

std::ostream& operator<<(std::ostream& stream, const element& elem)
{
	stream << " V." << elem.vertex << 
		" originV" << elem.originalVertex;
	return stream;
}
