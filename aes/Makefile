EXECUTABLES = aes-demo print-devices

all: $(EXECUTABLES)

ifdef OPENCL_INC
  CL_CFLAGS = -I$(OPENCL_INC)
endif

ifdef OPENCL_LIB
  CL_LDFLAGS = -L$(OPENCL_LIB)
endif

print-devices: print-devices.c cl-helper.c
	clang $(CL_CFLAGS) $(CL_LDFLAGS) -std=gnu99 -o$@ $^ -lOpenCL

aes-demo: aes-demo.c cl-helper.c
	clang  $(CL_CFLAGS) $(CL_LDFLAGS) -std=gnu99 -o$@ $^ -lOpenCL

clean:
	rm -f $(EXECUTABLES) *.o
