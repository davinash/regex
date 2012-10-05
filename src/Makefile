P_NAME          :=  libregex.a
P_CXX_SRCS        :=  $(wildcard *.cpp)
P_CXX_OBJS        :=  ${P_CXX_SRCS:.cpp=.o}
P_OBJS          :=  $(P_CXX_OBJS) 
P_INCLUDE_DIRS  :=  ../inc
CPPFLAGS        +=  $(foreach includedir,$(P_INCLUDE_DIRS),-I$(includedir))
LDFLAGS         +=  $(foreach librarydir,$(P_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS         +=  $(foreach library,$(P_LIBRARIES),-l$(library))
CXX             :=  g++
CC              :=  g++ -g -Wall -Wextra -Wparentheses -pedantic++

.PHONY:         all clean
all:            $(P_NAME)
$(P_NAME):      $(P_OBJS)
		ar rcs $(P_NAME) $(P_OBJS)
clean:
		@- $(RM) $(P_NAME)
		@- $(RM) $(P_OBJS)
		@- $(RM) core*
		@- $(RM) tags

