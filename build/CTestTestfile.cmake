# CMake generated Testfile for 
# Source directory: /mnt/ebs1/ebs/pnl_calculator
# Build directory: /mnt/ebs1/ebs/pnl_calculator/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pnl_calculator_test "/mnt/ebs1/ebs/pnl_calculator/build/test_pnl_calculator")
set_tests_properties(pnl_calculator_test PROPERTIES  _BACKTRACE_TRIPLES "/mnt/ebs1/ebs/pnl_calculator/CMakeLists.txt;40;add_test;/mnt/ebs1/ebs/pnl_calculator/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
