pushkar@pushkarPC:~/computational_materials_learning$ ./build/diagnostic_exercise
The temperature is 1000 K
The diffusivity at this temperature is 8.72707e-18 m^2/sec
The temperature is 1100 K
The diffusivity at this temperature is 1.34296e-16 m^2/sec
The temperature is 1200 K
The diffusivity at this temperature is 1.31036e-15 m^2/sec
The temperature is 1300 K
The diffusivity at this temperature is 9.00563e-15 m^2/sec
/home/pushkar/computational_materials_learning/cpp_foundations/diagnostic_exercise.cpp:12:32: runtime error: index 4 out of bounds for type 'int [4]'
/home/pushkar/computational_materials_learning/cpp_foundations/diagnostic_exercise.cpp:12:32: runtime error: load of address 0x7a5a84600030 with insufficient space for an object of type 'int'
0x7a5a84600030: note: pointer points here
 14 05 00 00  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  f5 7d cf 38 8d 1f 64 3c  bf b6 3c 21
              ^ 
=================================================================
==668297==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x7a5a84600030 at pc 0x5d2387ed968e bp 0x7ffd17689a30 sp 0x7ffd17689a20
READ of size 4 at 0x7a5a84600030 thread T0
    #0 0x5d2387ed968d in main (/home/pushkar/computational_materials_learning/build/diagnostic_exercise+0x368d) (BuildId: 9277fc260b6f1c280db469961b4b0e9ceec2f828)
    #1 0x7a5a8662a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #2 0x7a5a8662a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #3 0x5d2387ed92e4 in _start (/home/pushkar/computational_materials_learning/build/diagnostic_exercise+0x32e4) (BuildId: 9277fc260b6f1c280db469961b4b0e9ceec2f828)

Address 0x7a5a84600030 is located in stack of thread T0 at offset 48 in frame
    #0 0x5d2387ed93b8 in main (/home/pushkar/computational_materials_learning/build/diagnostic_exercise+0x33b8) (BuildId: 9277fc260b6f1c280db469961b4b0e9ceec2f828)

  This frame has 2 object(s):
    [32, 48) 'T' (line 8) <== Memory access at offset 48 overflows this variable
    [64, 96) 'D' (line 9)
HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
      (longjmp and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: stack-buffer-overflow (/home/pushkar/computational_materials_learning/build/diagnostic_exercise+0x368d) (BuildId: 9277fc260b6f1c280db469961b4b0e9ceec2f828) in main
Shadow bytes around the buggy address:
  0x7a5a845ffd80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7a5a845ffe00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7a5a845ffe80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7a5a845fff00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7a5a845fff80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x7a5a84600000: f1 f1 f1 f1 00 00[f2]f2 00 00 00 00 f3 f3 f3 f3
  0x7a5a84600080: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7a5a84600100: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7a5a84600180: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7a5a84600200: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x7a5a84600280: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==668297==ABORTING

Upon modifying the physics in diagnostic program, the sanitizer in not capable to catch the issues in physics which do change the very trend of diffusivities with temperature and need to be tested thoroughly.

