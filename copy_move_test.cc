// C++ Program to optimize the usage
// of C++ Vectors to reduce the cost
// of copying elements with the
// reserve() method

#include <iostream>
#include <vector>
using namespace std;

// Class
class GFG {

public:
	float x, y;

	// Parameterized Constructor
	GFG(float x, float y)
		: x(x), y(y)
	{
	}

	// Copy Constructor
	GFG(const GFG& GFG)
		: x(GFG.x), y(GFG.y)
	{
		cout << "Copied" << endl;
	}

    // Move Constructor
    GFG(GFG&& GFG) noexcept
        : x(GFG.x), y(GFG.y)
    {
        GFG.x = 0;
        GFG.y = 0;
        cout << "Moved" << endl;
    }
};

// Driver Code
int main()
{
	// Create object of vector in
	// the main() method
	vector<GFG> vertices;

	// Reserve three elements using
	// reserve()
	vertices.reserve(3);

	// Add elements to the vector
	// object
	vertices.push_back(std::move(GFG(1, 2)));
	cout << endl;

	vertices.push_back(std::move(GFG(4, 5)));
	cout << endl;

	vertices.push_back(std::move(GFG(7, 8)));
	cout << endl;

	return 0;
}
