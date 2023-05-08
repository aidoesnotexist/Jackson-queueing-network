#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <iostream>
#include <chrono>
#include <vector>

using std::cout;
using std::cin;

class element
{
	//Variables
	static int number;
	int vertex;
	int originalVertex;

	static std::vector<int> originCounter;

	//Stores all the elements' lifetime
	static std::vector<std::vector<double>> lifeTime;

	//Initializing methods
	void initVariables();
	void init_static_vectors();

public:
	//Constructors/Destructors
	element();
	element(const element&);
	element(const element&, const int&);
	
	//Accessers/Modifiers
	void setVertex(const int&);
	const int getVertex() const;

	static double getLifeTime(const int&);

	//Function to add ellapsed time to an element
	void addTime(const double&);

	//Overloaded operators
	friend bool operator==(const element&, const element&);
	friend std::ostream& operator <<(std::ostream&, const element&);
};


#endif
