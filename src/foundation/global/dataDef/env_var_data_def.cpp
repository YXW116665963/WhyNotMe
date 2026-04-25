#include "env_var_data_def.h"

namespace why
{
	namespace envVar
	{
		const char* g_Domain = "envVar";
		const char* strDeployment_dirPath = "deployment_dirPath";
	}

	// dataDef
	namespace envVar
	{
		const char* strDataDef_dirPath = "data_def_dirPath";

	}

	// 用户
	namespace envVar
	{
		const char* strUserConfig_dirPath = "user_config_dirPath";
		const char* strLog_dirPath = "log_dirPath";
		const char* strUserRecipe_dirPath = "user_recipe_dirPath";		
	}

	//db路径
	namespace envVar
	{
		const char* strDataBase_dirPath = "data_base_dirPath";
		const char* strDataCenter_filePath = "data_center_filePath";
	}


	// UI级env
	namespace envVar
	{
		const char* strUIXml_dirPath = "ui_xml_dirPath";
		const char* strUIPicture_dirPath = "ui_picture_dirPath";
		const char* strUIResourceXml_filePath = "resource_xml_filePath";
	}

}