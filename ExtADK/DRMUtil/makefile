LIB_TARGET = libofficedrmutil
OBJS = *.o
SRCS = $(OBJS:.o=.c)
$(LIB_TARGET):$(SRCS)
					gcc -fPIC -c -fvisibility=hidden $(SRCS) -g
					gcc -shared -Wl,-soname,$(LIB_TARGET).so -o $(LIB_TARGET).so $(OBJS) -ldl

clean:
			rm -f *.o $(LIB_TARGET).*
	
