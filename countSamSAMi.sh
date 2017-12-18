#!/bin/bash
make
for f in dna200 english200 proteins200 sources200 xml200; do
  for t in 1 1-4x4 1-4x2 2 2-4x4 2-4x2; do
    for q in 4 5 6; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi $t $q 1 $f 500000 10
    done
  done
done
for t in 1 1-4x4 1-4x2 2 2-4x4 2-4x2; do
  for q in 4 5 6; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 dna200 500000 20
  done
  for q in 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 dna200 500000 20
  done
  for q in 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 3 dna200 500000 20
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 english200 500000 20
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 proteins200 500000 20
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 sources200 500000 20
  done
  for q in 4 5 6 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 xml200 500000 20
  done
  for q in 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 xml200 500000 20
  done
done
for t in 1 1-4x4 1-4x2 2 2-4x4 2-4x2; do
  for q in 4 5 6; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 dna200 500000 50
  done
  for q in 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 dna200 500000 50
  done
  for q in 16 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 3 dna200 500000 50
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 english200 500000 50
  done
  for q in 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 english200 500000 50
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 proteins200 500000 50
  done
  for q in 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 proteins200 500000 50
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 sources200 500000 50
  done
  for q in 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 sources200 500000 50
  done
  for q in 4 5 6 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 xml200 500000 50
  done
  for q in 16 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 xml200 500000 50
  done
done

for t in 1-hash 1-hash-4x4 1-hash-4x2 2-hash 2-hash-4x4 2-hash-4x2 1-hash-dense 1-hash-dense-4x4 1-hash-dense-4x2 2-hash-dense 2-hash-dense-4x4 2-hash-dense-4x2; do
  for q in 4 5 6; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 12 0.9 dna200 500000 20
  done
  for q in 8 10; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 12 0.9 dna200 500000 20
  done
  for q in 4 5 6 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 8 0.9 english200 500000 20
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 5 0.9 proteins200 500000 20
  done
  for q in 4 5 6 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 8 0.9 sources200 500000 20
  done
  for q in 4 5 6 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 8 0.9 xml200 500000 20
  done
done

for t in 1-hash 1-hash-4x4 1-hash-4x2 2-hash 2-hash-4x4 2-hash-4x2 1-hash-dense 1-hash-dense-4x4 1-hash-dense-4x2 2-hash-dense 2-hash-dense-4x4 2-hash-dense-4x2; do
  for q in 4 5 6; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 12 0.9 dna200 500000 50
  done
  for q in 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 12 0.9 dna200 500000 50
  done
  for q in 16 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 3 12 0.9 dna200 500000 50
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 8 0.9 english200 500000 50
  done
  for q in 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 8 0.9 english200 500000 50
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 5 0.9 proteins200 500000 50
  done
  for q in 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 5 0.9 proteins200 500000 50
  done
  for q in 4 5 6 8 10 12 16; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 8 0.9 sources200 500000 50
  done
  for q in 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 8 0.9 sources200 500000 50
  done
  for q in 4 5 6 8 10 12; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 1 8 0.9 xml200 500000 50
  done
  for q in 16 24 32 40; do
    sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
    taskset -c 0 ./test/countSamSAMi $t $q 2 8 0.9 xml200 500000 50
  done
done

for f in dna200 english200 proteins200 sources200 xml200; do
  for t in mpe2; do
    for l in 16 32; do
      for q in 4 5 6; do
        sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
        taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 1 $l $f 500000 10
      done
    done
  done
done
for t in mpe2; do
  for l in 16 32; do
    for q in 4 5 6; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 1 $l dna200 500000 20
    done
    for q in 8 10 12; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 2 $l dna200 500000 20
    done
    for q in 16; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 3 $l dna200 500000 20
    done
    for q in 4 5 6 8 10 12 16; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 1 $l english200 500000 20
    done
    for q in 4 5 6 8 10 12 16; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 1 $l proteins200 500000 20
    done
    for q in 4 5 6 8 10 12 16; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 1 $l sources200 500000 20
    done
    for q in 4 5 6 8 10 12; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 1 $l xml200 500000 20
    done
    for q in 16; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 2 $l xml200 500000 20
    done
  done
done
for t in mpe2; do
  for l in 16 32; do
    for q in 4 5 6; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 1 $l dna200 500000 50
    done
    for q in 8 10 12; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 2 $l dna200 500000 50
    done
    for q in 16 24 32 40; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 3 $l dna200 500000 50
    done
    for q in 4 5 6 8 10 12 16; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 1 $l english200 500000 50
    done
    for q in 24 32 40; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 2 $l english200 500000 50
    done
    for q in 4 5 6 8 10 12 16; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 1 $l proteins200 500000 50
    done
    for q in 24 32 40; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 2 $l proteins200 500000 50
    done
    for q in 4 5 6 8 10 12 16; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 1 $l sources200 500000 50
    done
    for q in 24 32 40; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 2 $l sources200 500000 50
    done
    for q in 4 5 6 8 10 12; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 1 $l xml200 500000 50
    done
    for q in 16 24 32 40; do
      sudo sh -c "sync; echo 3 > /proc/sys/vm/drop_caches"
      taskset -c 0 ./test/countSamSAMi FMHWT-$t $q 2 $l xml200 500000 50
    done
  done
done