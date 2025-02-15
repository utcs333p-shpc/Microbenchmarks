# Microbenchmarks
Benchmarks to determine various architecture specifications.


This codebase benchmarks the throughput of FMA, load, store
instructions and the latency of the FMA. The codebase also can measure memory
latency, bandwidth as well as the clock frequency.


This benchmark uses `gettimeofday` to time the instructions. Hence, this
benchmark does not need root access to access the performance counters.    

This code has been derived from
[https://github.com/clamchowder/Microbenchmarks](https://github.com/clamchowder/Microbenchmarks)

