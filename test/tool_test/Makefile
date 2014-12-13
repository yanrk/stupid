project_home       = .
build_dir          = $(project_home)
bin_dir            = $(project_home)/bin/linux
object_dir         = $(project_home)/.objs
stupid_home        = $(project_home)/../..
gnu_home           = $(stupid_home)/gnu



# includes of stupid headers
stupid_inc_path    = $(stupid_home)/inc
stupid_includes    = -I$(stupid_inc_path)

# includes of local headers
local_inc_path     = $(project_home)
local_includes     = -I$(local_inc_path)

# all includes that local solution needs
includes           = $(stupid_includes)
includes          += $(local_includes)



# source files of local solution
local_src_path     = $(project_home)
local_source       = $(filter %.cpp, $(shell find $(local_src_path) -depth -name "*.cpp"))



# objects of local solution
local_objects      = $(local_source:$(project_home)%.cpp=$(object_dir)%.o)



# system librarys
system_libs        = -lpthread

# open source librarys
zlib_lib_inc       = $(gnu_home)/zlib/lib/linux
openssl_lib_inc    = $(gnu_home)/openssl/lib/linux
curl_lib_inc       = $(gnu_home)/curl/lib/linux
gnu_libs           = -L$(zlib_lib_inc) -lz
gnu_libs          += -L$(openssl_lib_inc) -lcrypto -lssl
gnu_libs          += -L$(curl_lib_inc) -lcurl

# stupid librarys
stupid_lib_inc     = $(stupid_home)/lib/linux
stupid_libs        = -L$(stupid_lib_inc) -lstupid_tool -lstupid_base

# local depends librarys
depend_libs        = $(stupid_libs)
depend_libs       += $(gnu_libs)
depend_libs       += $(system_libs)



# output execution
output_exec        = $(bin_dir)/tool_test



# my g++ not support nullptr and 64bits
# so no -m64, and add a macro -Dnullptr=0

# build flags for objects
build_obj_flags    = -std=c++0x -g -Wall -O1 -pipe -fPIC -Dnullptr=0

# build flags for execution
build_exec_flags   = $(build_obj_flags)



# build targets
targets            = mkln test

# let 'build' be default target, build all targets
build    : $(targets)

mkln     :
	chmod +x ./mkln.sh
	-./mkln.sh

test     : $(local_objects)
	@echo
	@echo "@@@@@  start making tool_test  @@@@@"
	g++ $(build_exec_flags) -o $(output_exec) $^ $(depend_libs)
	@echo "@@@@@  make tool_test success  @@@@@"
	@echo

rmln     :
	chmod +x ./rmln.sh
	-./rmln.sh

cpfile   :
	@cp $(stupid_lib_inc)/* $(bin_dir)/
	@cp $(zlib_lib_inc)/* $(bin_dir)/
	@cp $(openssl_lib_inc)/* $(bin_dir)/
	@cp $(curl_lib_inc)/* $(bin_dir)/

run      : cpfile
	@export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(bin_dir) && $(output_exec)
# it same as: @env LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(bin_dir) $(output_exec)

# build all objects
$(object_dir)/%.o:$(project_home)/%.cpp
	@dir=`dirname $@`;      \
    if [ ! -d $$dir ]; then \
        mkdir -p $$dir;     \
    fi
	g++ -c $(build_obj_flags) $(includes) -o $@ $<

clean    : rmln
	rm -rf $(object_dir) $(bin_dir)/*

rebuild  : clean build
