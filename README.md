# opencl-experiments 

This is for experimenting with OpenCL to see how easy it would be to add an
opencl backend to libvips.

### Install

For Intel GPUs you need:

    sudo apt-get install beignet-dev

To test:

    curl https://codeload.github.com/hpc12/tools/tar.gz/master | tar xvfz -
    cd tools-master
    make
    ./print-devices
    ./cl-demo 100000 10

The demo should print `GOOD` at the end. My Dell is a Broadwell GT2 and gets
about 12GB/s on that demo.

### TODO

* make a simple image processing program and try tuning it
* try on a discrete GPU as well, memory behaviour will be very different
* try with an AMD and nvidia GPU as well
* work units, sizing, scheduling ... we will have 128x128 pixel units, how many
  will we need to schedule to keep the GPU busy? maybe subdivide further?
* how many threads needed to keep GPU fed? perhaps have thread0 in the vips
  workpool as the GPU feeder?
* use the cpu as well, time cpu against orc
