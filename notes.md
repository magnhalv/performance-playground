# Performance and memory

## Presentation

- Show the physical distance from the processor to memory.
- Talk about the L1, L2, L3 cache.
- Mention Data Driven Design
    - Trying to feed the CPU as much as possible, and don't waste bandwith.

- Allocate large chunks of data
- ArenaAllocators

## Talking points

* Smaller memory footprint leads to faster processing.
* The order which you access data, can lead to faster processing.
* If the data is structured sequentually, it is easier to exploit SIMD instructions.

## Case studies

- [X] Matrix multiplication 
- [] Padding
- [] Ordering

## Questions:
- Does the compiler optimize struct order?
    - In general no, as that could destroy binary formats.
- Does high level language member variables order matter?
    - It seems so, at least for C#.

## Reading list:

- https://people.freebsd.org/~lstewart/articles/cpumemory.pdf
- https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html#optimization
- https://floooh.github.io/2018/06/17/handles-vs-pointers.html
    - https://github.com/floooh/oryol/blob/master/code/Modules/Resource/Id.h

## Videos:
- https://www.youtube.com/watch?v=rX0ItVEVjHc
- https://www.youtube.com/watch?v=IroPQ150F6c
