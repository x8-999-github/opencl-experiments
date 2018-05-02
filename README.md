# opencl-experiments 

Code based on https://github.com/jcupitt/opencl-experiments

This repo merges opencl-experiments with the opencl paes AES core into a frankenstein experiment.
* Compared to opencl-experiments (no difference take their code)
* Compared paes: I did not care about decryption (hence broke it) and I added key derivation into the mix. The original paes core itself was not well optimized in the first place and this is therefore a mostly dead experiment.
