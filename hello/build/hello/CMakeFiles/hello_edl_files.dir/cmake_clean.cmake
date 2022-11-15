file(REMOVE_RECURSE
  "../_headers_/hello/Hello.edl"
  "../_headers_/hello/Hello.edl.h"
  "Hello.edl.h"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/hello_edl_files.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
