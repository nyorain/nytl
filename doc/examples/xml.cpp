#include <nytl/hierachy.hpp>
#include <string>
#include <iostream>

class xmlNode : public nytl::hierachyNode<xmlNode>
{
protected:
	std::string name_;

public:
	xmlNode(const xmlNode& other) = default;
	xmlNode(xmlNode&& other) = default;

	xmlNode& operator=(const xmlNode& other) = default;
	xmlNode& operator=(xmlNode&& other) = default;

	const std::string& name() const { return name_; }
};

using xmlRoot = nytl::hierachyRoot<xmlNode>;

xmlRoot parseFile(const std::string& filename); //implement it somewhere
void recurse(const xmlNode& node)
{
	std::cout << node.name() << "\n";
	for(auto* child : node)
	{
		recurse(*child);	
	}
}

int main()
{
	auto root = parseFile("somefile.xml");

	//iterate all(!) nodes
	for(auto& node : root.recursive())
	{
		std::cout << node->name() << "\n";
	};

	//manual recurse
	recurse(root);

	return 0;
}
