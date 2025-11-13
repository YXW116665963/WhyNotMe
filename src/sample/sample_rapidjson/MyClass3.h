#pragma once
#include "RapidjsonHelper.h"
using namespace PBLIB::RapidJsonHelper;

class MyClass3 :public JsonBase
{
public:
	int age;
	char name[100];
	std::string text;
	double money;

	MyClass3() {
		memset(name, 0, ARRAY_SIZE(name));
	}
	~MyClass3() {}

	void ToWrite(Writer<StringBuffer>& writer)
	{
		RapidjsonWriteBegin(writer);
		RapidjsonWriteString(text);
		RapidjsonWriteChar(name);
		RapidjsonWriteInt(age);
		RapidjsonWriteDouble(money);
		RapidjsonWriteEnd();
	}

	void ParseJson(const Value& val)
	{
		RapidjsonParseBegin(val);
		RapidjsonParseToString(text);
		RapidjsonParseToInt(age);
		RapidjsonParseToDouble(money);
		RapidjsonParseToChar(name);
		RapidjsonParseEnd();
	}

};