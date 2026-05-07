open_project conv1d_hls_prj
set_top stem_conv1d_hls

add_files src/conv1d_hls.cpp -cflags "-Iinclude"
add_files src/hls_quant.cpp  -cflags "-Iinclude"
add_files -tb tb/test_conv1d_hls.cpp -cflags "-Iinclude"

open_solution "solution1"
set_part {xc7a100tcsg324-1}
create_clock -period 10 -name default

csim_design
csynth_design
cosim_design

exit