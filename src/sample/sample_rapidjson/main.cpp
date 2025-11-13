// PB_RapidJsonHelper.cpp : 定义控制台应用程序的入口点。
//
#include "MyClass.h"


int main()
{
	MyClass mylclass2;
	mylclass2.age = 10;
	strcpy(mylclass2.name, "tt");
	mylclass2.text = "myclass2";
	mylclass2.money = 6;
	for (unsigned i = 0; i < 10; i++)
	{
		MyClass3 tmp;
		tmp.age = 10;
		strcpy(tmp.name, "myclass3");
		tmp.text = "123456";
		tmp.money = 8;
		mylclass2.lst2.arr.push_back(tmp);
	}
	for (unsigned i = 0; i < 10; i++)
	{
		mylclass2.lst.arr.push_back(i);
	}
	//构造完成


	//Json序列化
	std::string str = mylclass2.ToJson();//序列化完成
	std::cout << str.c_str();

	MyClass mylclass4;
	MyClass::FromJson(&mylclass4, str);//反序列化完成


	return 0;
}