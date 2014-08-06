#!/bin/bash
flexc++ ./parameters.l
flexc++ ./domain.l
flexc++ ./constraint-parameters.l
flexc++ ./constraint-scope.l
flexc++ ./predicate-f.l
bisonc++ --thread-safe ./parameters.y
bisonc++ --thread-safe ./domain.y
bisonc++ --thread-safe ./constraint-parameters.y
bisonc++ --thread-safe ./constraint-scope.y
exec bisonc++ --thread-safe "./predicate-f.y"
