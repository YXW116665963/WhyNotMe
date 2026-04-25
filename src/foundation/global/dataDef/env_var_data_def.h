#pragma once

namespace why
{
	// App级env
	namespace envVar
	{
		extern const char* g_Domain;							//!< 环境变量域
		extern const char* strDeployment_dirPath;					//!< 可执行exe所在的目录

	}

	// 数据定义
	namespace envVar
	{
		extern const char* strDataDef_dirPath;					//!< 数据定义目录
	}

	// db级env
	namespace envVar
	{
		extern const char* strDataBase_dirPath;					//!< 数据库目录路径
		extern const char* strDataCenter_filePath;				//!< 数据中心文件路径
		
	}

	// UI级env
	namespace envVar
	{
		extern const char* strUIXml_dirPath;						//!< XML目录
		extern const char* strUIPicture_dirPath;					//!< 图片目录
		extern const char* strUIResourceXml_filePath;					//!< 字体背景图片等资源定义	
	}

	// 用户级env
	namespace envVar
	{
		extern const char* strUserConfig_dirPath;				//!< 用户数据目录
		extern const char* strLog_dirPath;						//!< 运行日志目录
		extern const char* strUserRecipe_dirPath;				//!< 用户配方目录
		extern const char* strDataDef_dirPath;					//!< 数据定义目录
	}



}