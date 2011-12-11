#!/bin/bash

# run all the tests
LIST=`ls $1 |grep test`
for I in $LIST
do
  echo "TEST => $1/$I"
  $1/$I
done

exit 0

