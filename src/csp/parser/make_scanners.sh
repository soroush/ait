#!/bin/bash
flexc++ ./parameters.l 
flexc++ ./constraint-parameters.l 
flexc++ ./predicate-f.l
bisonc++ --thread-safe ./parameters.y 
bisonc++ --thread-safe ./constraint-parameters.y 
bisonc++ --thread-safe ./predicate-f.y 
