# This file is meant to be run with the `subdir` command
# of the project's root folder's ./build.sh.

build_parallel triangle_basic.cpp
build_parallel triangle_colors.cpp
build_parallel triangle_index_buffer.cpp
build_parallel triangle_separate_shader.cpp
build_parallel triangle_uniforms.cpp
build_parallel triangle_vaos.cpp
build_parallel triangle_vaos_n_ibos.cpp

wait