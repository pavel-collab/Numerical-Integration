## Build
Instruction how to build the project
```consol
mkdir build && cd build
cmake ..
make
```
After that you can run the executable file you need.  
You also can build each target separately:
```consol
make <target>
```
Avaliable targets:
* main
* gnuplot_call
* runUnitTests
* long_time_integration

## TEST MODE
If you want switch programm to test mode, you need to uncomment #define TEST_MODE; in files main and long time integration. Then you have to buld the project again. After that you can use bush script:

```consol
for i in ../tests/*.dat; do echo '=============================='; time cat $i | xargs ./main; done
```