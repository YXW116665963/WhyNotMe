#include "logger.h"
#include "text_util.h"
#include "file_util.h"

namespace why
{
	Logger::Logger(const std::string &strLogPath, const std::string &strLogName, int32_t nLogLevel)
	{
		if (!google::IsGoogleLoggingInitialized())
		{
			google::InitGoogleLogging(strLogName.c_str());
			google::SetStderrLogging(google::GLOG_FATAL);
			google::SetLogFilenameExtension(".log");
			std::string strLocalLogPath = UTF8ToLocal(strLogPath);

			PathAppender pathAppender;
			std::string strGlogDestination = pathAppender.
				SetSourcePath(strLocalLogPath).
				AppendChildPath(strLogName).
				GetPath();

			google::SetLogDestination(google::GLOG_INFO, strGlogDestination.c_str());
			FLAGS_colorlogtostderr = true;
			FLAGS_logbufsecs = 0;
			FLAGS_max_log_size = 50;
			FLAGS_stop_logging_if_full_disk = true;
			FLAGS_v = nLogLevel;
			LOG_INFO << "The logger initing";
		}
		else
		{
			LOG_INFO << "The logger is inited";
		}
	}

	Logger::~Logger()
	{
		if (google::IsGoogleLoggingInitialized())
		{
			LOG_INFO << "The logger is uninited";
 			google::ShutdownGoogleLogging();	//<! 与上面初始化同步注释
		}
	}
}