EXECUTABLES = cl-demo print-devices

all: $(EXECUTABLES)

#
# clang also works of course
#
CC ?= gcc
CFLAGS=-Wall

ifdef OPENCL_INC
  CL_CFLAGS = -I$(OPENCL_INC)
endif

ifdef OPENCL_LIB
  CL_LDFLAGS = -L$(OPENCL_LIB)
endif

print-devices: print-devices.c cl-helper.c
	$(CC) $(CFLAGS) $(CL_CFLAGS) $(CL_LDFLAGS) -std=gnu99 -o$@ $^ -lOpenCL

cl-demo: cl-demo.c cl-helper.c
	$(CC) $(CFLAGS) $(CL_CFLAGS) $(CL_LDFLAGS) -std=gnu99 -o$@ $^ -lOpenCL

clean:
	rm -f $(EXECUTABLES) *.o
