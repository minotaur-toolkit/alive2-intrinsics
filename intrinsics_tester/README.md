Intrinsic Semantic Tester
=========================

This program runs random tests on the intrinsic semantics
implemented in Alive2. It compares the semantics against
the true output of the intrinsic call, and asserts that
they must be equal. If Alive2 states that they are not equal,
then there is a semantic issue with the implementation that
must be addressed.

WARNING
-------
This tool requires AVX512 support to properly function. While 
the project may compile on a system without it, the tool requires it.

Usage
-----
Once the project is built, run `$ ./intrinsics-tester -h` from the build directory.
This will provide help on using the tool, as well as the number of testable intrinsics.

The only required argument is the number of tests to perform per intrinsic, which
must be the first argument passed to the executable. 

There is also support for specifying a range of intrinsics to use, and a debug mode, 
which, among other things, shows which intrinsic corresponds to which number.

If any mismatch is found, the program will write the failed test in Alive2 IR, as well 
as in LLVM IR. This will be written to standard out.

The progress bar, in order to separate its print functionality, is written to standard error.

### Example usage
```
$ ./intrinsics-tester 40 -d -r=20-75
```
The above will run 40 tests, for each intrinsic in the range of 20 inclusive to 
75 exclusive, as well as debug information.

```
$ ./intrinsics-tester 10000 >results.txt
```
Performs 10,000 tests on each intrinsic, and writes the results, including any mismatches, 
to a file. Note that the progress bar is not written to the file.

## Information for Project Developers

If you add support for any intrinsics in `ir/instr.h`, the tester should be able to function
properly, but you MUST add some code in exactly one place. In `intrinsics_tester/x86Intrin.cpp`,
you must add the proper `llvm::Intrinsic::ID` to the end of the list. 

The list is dependent on the ordering of the instructions in `ir/instr.h`.
This means that you should NOT make changes to the existing list, such as re-ordering existing
entries, as this will disrupt the tester completely.

# AliveTV Bugs Found

No current bugs to report.


## Historical bugs:

[2 type errors, 9 semantic errors](https://github.com/zhengyang92/alive2-x86/commit/1100832edf82a0652fac46c8ddb82e7830bca11f#diff-1e2e97eee3636d89935ab5[%E2%80%A6]8926d70fcae3f26eeb88a32eea4c)

[1 type error, 1 semantic error](https://github.com/zhengyang92/alive2-x86/commit/69e464ad1af37528cf71ea69c41fb80976d7008a)

[1 semantic error](https://github.com/zhengyang92/alive2-x86/commit/2c87fa45b3e2a4472f260d798953a4f07d543f6c)
