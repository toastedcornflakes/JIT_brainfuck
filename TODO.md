* Compiler: write an abstraction for the x64 code that supports working only working with offsets and use that to implement relocatable code, to make the `mmap()` page grow as needed.

* Fix the RWX memory: make it RW for code generation and RX for execution (see mprotect())

* Make an option to dump the code to binary 
* Implement the read syscall for getchar()

* Add a "benchmark" target that defines a macro and enable a macro that bench compilation, interpretation and running compiled

* Implement optimization (e.g. `>>>` should result in `add rdx, 3` instead of 3 times ` add rdx, 1`)
	syscall write should be a function that you `call` and `ret` instead of duplicating the code 
	(put the function at the beginning of the code and start it with a trampoline?)

