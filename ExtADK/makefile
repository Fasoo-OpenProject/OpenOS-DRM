LIB_TARGET = libfasooextadk
OBJS = *.o
SRCS = $(OBJS:.o=.cpp)
$(LIB_TARGET):$(SRCS)
					g++ -fPIC -c -fvisibility=hidden $(SRCS) -g
					g++ -shared -Wl,-soname,$(LIB_TARGET).so -o $(LIB_TARGET).so $(OBJS) -ldl -lpthread -L/usr/local/fasoo -lf_Trace -lConfigJson

clean:
			rm -f *.o $(LIB_TARGET).*
	
