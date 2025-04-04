# CAIAL

C simple AI algorithms for testing bprof on bare metal RISC-V roscket-chip.
Currently are implemented the the next AI algoritms Classification Tree (CT),K-nearest neighbors (KNN),Deep neural networks (DNN), Linear regresion (LR), Naive Bayes (NB), Suport Vector Machine (SVM) and K-Means (KMEANS). Their implementation is simple as possible and does not use any libraries.

## Getting Started

These instructions will get help you run these algorithm on the rocket-chip.

### Prerequisites

[Nix](https://nix.dev/install-nix.html) installed on your system.


* rocket chip
```
git clone https://github.com/Earthbert/rocket-chip
cd rocket-chip
submodule update --init --recursive
```

### Setup

* nix development env
```
# Inside rocket-chip project
# You can also enable these experimental features in "~/.config/nix/nix.conf", to shorten the command
nix develop --extra-experimental-features nix-command --extra-experimental-features flakes
```

**IMPORTANT**: You always need to have use this to have access to needed tools

* create a emulator config

```
# Inside rocket-chip project, with nix active
mill emulator[freechips.rocketchip.system.TestHarness,freechips.rocketchip.system.$CONFIG_NAME].verilator.elf
```

* listing posible configs
```
# Inside rocket-chip project, with nix active
mill resolve emulator._
```

### Installing

* Clone and build the repo
```
git clone https://github.com/Earthbert/caial
make
make simv
make strip
```

* Test the system

```
python parser.py  strip_simv.log output
```

## Running the tests

If you want to test one of the algorithms you have to do the next things:
Compile the algorithm
```
make APP=NAME_OF_THE_ALGORITHM

```
Simulate the algorithm
```
make simv APP=NAME_OF_THE_ALGORITHM

```
Delete the logs that are not necesary
```
make strip APP=NAME_OF_THE_ALGORITHM

```
Run the parser
```
python parser.py  strip_simv.log output

```


### Adding other programs

For adding another program just create directory with the program name.
Inside the directory write you code in the file with the name the same as the directory name
plus extension ".c".
Example:

```
mkdir NEWPROGRAM
cd NEWPROGRAM
touch NEWPROGRAM
...
cd ..
make APP=NEWPROGRAM
```

### Analyze the results

The results will be in the file with the name of the second argument to the parser plus
".csv" extension.

```
python parser.py  strip_simv.log output
cat output.csv
```

## Docker Deployment

If you want to use dockers. First you will have to install docker.

### Build the docker image
```
docker build -t caial docker
```

### Run docker with local git repo as a volume
```
docker run -it -v `pwd`:/project/caial caial
```

### Download docker image
```
docker pull sdcioc/bprofriscv:latest
```
Inside docker
```
cd caial
git pull
```


## Built With

* [Rocket-Chip](https://github.com/freechipsproject/rocket-chip) - The RISC-V processor used
* [Rocket-tools](https://github.com/freechipsproject/rocket-tools) - Tools for Rocket Chip and RISC-V tools
* [RISCV-TESTS](https://github.com/riscv/riscv-tests) - Tests for RISC-V cores


## Directories
All directories except docker represent an algorithm that can be used. The "docker" directory has a Dockerfile necesary for building the docker image.

## Experiment logic

TODO:


## Implementations

TODO:

## Contributing
TODO:

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

TODO:

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Ciocîrlan Ștefan-Dan** - *Algorithms and building the system work* - [sdcioc](https://github.com/sdcioc)

See also the list of [contributors](https://github.com/sdcioc/caial.git/contributors) who participated in this project.

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details

## Acknowledgments

* NUS
