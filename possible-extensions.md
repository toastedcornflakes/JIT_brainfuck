* Fix the RWX memory: make it RW for code generation and RX for execution (see mprotect (2))

* Add an option to dump the code to binary 
* Implement the read syscall for getchar()

* Implement operation combination: `>>>` should result in `add rdx, 3` instead of 3 times ` add rdx, 1`.

