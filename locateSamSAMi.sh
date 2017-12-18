#!/bin/bash
make
for f in dna200 english200 proteins200 sources200 xml200; do
  for t in 1 1-4x4 1-4x2; do
    for q in 4 5 6; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/locateSamSAMi $t $q 1 $f 10000 10
    done
  done
done
for t in 1 1-4x4 1-4x2; do
  for q in 4 5 6; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 1 dna200 10000 20
  done
  for q in 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 2 dna200 10000 20
  done
  for q in 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 3 dna200 10000 20
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 1 english200 10000 20
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 1 proteins200 10000 20
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 1 sources200 10000 20
  done
  for q in 4 5 6 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 1 xml200 10000 20
  done
  for q in 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 2 xml200 10000 20
  done
done
for t in 1 1-4x4 1-4x2; do
  for q in 4 5 6; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 1 dna200 10000 50
  done
  for q in 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 2 dna200 10000 50
  done
  for q in 16 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 3 dna200 10000 50
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 1 english200 10000 50
  done
  for q in 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 2 english200 10000 50
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 1 proteins200 10000 50
  done
  for q in 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 2 proteins200 10000 50
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 1 sources200 10000 50
  done
  for q in 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 2 sources200 10000 50
  done
  for q in 4 5 6 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 1 xml200 10000 50
  done
  for q in 16 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/locateSamSAMi $t $q 2 xml200 10000 50
  done
done