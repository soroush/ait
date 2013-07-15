#!/bin/bash
flexc++  ./parameters.l ./predicate-f.l 
bisonc++ --thread-safe ./parameters.y ./predicate-f.y 