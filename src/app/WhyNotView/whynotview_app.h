#include "wx/wx.h"
#include "logger.h"

#include "main_frame.h"
namespace why
{
    // 应用程序类
    class WhyNotViewApp : public wxApp
    {
    public:
        WhyNotViewApp();
        ~WhyNotViewApp() {};
        virtual bool OnInit() wxOVERRIDE;
        int OnExit() wxOVERRIDE;

    private:
        bool LoadwhyBladeData();//test
    private:        
        bool LoadUIResource();
    private:
        // 初始化顺序，按序调用
		bool InitLogger();      
        bool InitDataCenter();  // 初始化app的执行路径和数据路径,初始化数据中心
        void InitEnvVar();      // 初始化环境变量
		bool InitGlobalTimer(); // 初始化全局定时器

	private:
        MainFrame* m_pMainFrame;
    };
}
