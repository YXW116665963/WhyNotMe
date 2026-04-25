set(wxwidgets_dll_name "wxmsw330u$<$<CONFIG:Debug>:d>_vc_x64_custom.dll")
set(wxwidgets_lib_name "wxmsw33u$<$<CONFIG:Debug>:d>.lib")

set(wxwidgets_dir_path ${dependence_dir_path}/wxwidgets)

if(OS_NAME STREQUAL "windows")
	add_definitions(-DwxUSE_DPI_AWARE_MANIFEST=2)
	add_definitions(-DWX_PRECOMP)
	add_definitions(-D__WXMSW__)
	add_definitions(-DWXUSINGDLL)
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
	set(wxwidgets_inc_dir_path 
		${wxwidgets_dir_path}/include
		${wxwidgets_dir_path}/include/mswud
	)	
	set(wxwidgets_lib_dir_path ${wxwidgets_dir_path}/${BuildConfig})
else()
	#MESSAGE("Not windows platform is not set wxwidgets path")
endif()