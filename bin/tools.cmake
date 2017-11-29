macro(append_source_files SRC_FILES dir)
    file(GLOB_RECURSE cc_files "${dir}/*.cc")
    list(APPEND ${SRC_FILES} ${cc_files})
endmacro(append_source_files)

macro(include_and_link dir)
    include_directories(${dir}/include)
    link_directories(${dir}/lib)
endmacro(include_and_link)