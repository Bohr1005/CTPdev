#include "document.h"
#include <iostream>
using namespace rapidjson;

int main()
{
	const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
	Document d;
	d.Parse(json);
	std::cout << "hello" << std::endl;
	auto res = d.IsObject();
	std::cout << typeid(res).name();

}