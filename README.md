Early proof of concept for a JVM base kotlin binding for the godot engine.  
Created to test the possibilities for compile time and runtime perf vs our kotlin native binding here: https://github.com/utopia-rise/godot-kotlin  
 

Early benchmarks:  
```
Running benchmark: [name=Simple:avg, lang=ktJvm]
Results: avg=30333 op/s, median=30303 op/s, 95pc=29411 op/s, 99pc=29411 op/s

Running benchmark: [name=Simple:simple_add, lang=ktJvm]
Results: avg=348837 op/s, median=333333 op/s, 95pc=333333 op/s, 99pc=333333 op/s

Running benchmark: [name=Simple:avg, lang=kt]
Results: avg=22779 op/s, median=23255 op/s, 95pc=19607 op/s, 99pc=18518 op/s

Running benchmark: [name=Simple:simple_add, lang=kt]
Results: avg=92592 op/s, median=90909 op/s, 95pc=83333 op/s, 99pc=83333 op/s

Running benchmark: [name=Simple:simple_add, lang=gd]
Results: avg=789473 op/s, median=1000000 op/s, 95pc=500000 op/s, 99pc=500000 op/s

Running benchmark: [name=Simple:avg, lang=gd]
Results: avg=1079 op/s, median=1079 op/s, 95pc=1071 op/s, 99pc=1028 op/s
```
