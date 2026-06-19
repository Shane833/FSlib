#!/bin/bash
for i in tests/*exe; # get each of the executable
    do ./$i; # run the executables
done

