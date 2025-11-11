set(wxwidgets_dll_name "wxmsw330u$<$<CONFIG:Debug>:d>_vc_x64_custom.dll")
set(wxwidgets_lib_name "wxmsw33u$<$<CONFIG:Debug>:d>.lib")

add_definitions(-DwxUSE_DPI_AWARE_MANIFEST=2)
add_definitions(-DWX_PRECOMP)
add_definitions(-D__WXMSW__)
add_definitions(-DWXUSINGDLL)
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)