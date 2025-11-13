#include "RapidjsonHelper.h"
#include "MyClass3.h"

using namespace PBLIB::RapidJsonHelper;

class MyClass :public JsonBase
{
public:
	MyClass() {
		memset(name, 0, ARRAY_SIZE(name));
	}
	~MyClass() {}

	int age;
	char name[100];
	std::string text;
	double money;
	JsonArray<int> lst;
	JsonArray<MyClass3> lst2;

	void ToWrite(Writer<StringBuffer>& writer)
	{
		RapidjsonWriteBegin(writer);
		RapidjsonWriteString(text);
		RapidjsonWriteChar(name);
		RapidjsonWriteInt(age);
		RapidjsonWriteDouble(money);
		RapidjsonWriteClass(lst);
		RapidjsonWriteClass(lst2);
		RapidjsonWriteEnd();
	}

	void ParseJson(const Value& val)
	{
		RapidjsonParseBegin(val);
		RapidjsonParseToString(text);
		RapidjsonParseToInt(age);
		RapidjsonParseToDouble(money);
		RapidjsonParseToChar(name);
		RapidjsonParseToClass(lst);
		RapidjsonParseToClass(lst2);
		RapidjsonParseEnd();
	}


};