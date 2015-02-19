Poly OS
===

An attempt at writing an Operating System that is written in
many languages not usually used for systems programming. Right now
it is mostly C and Rust with a bit of Assembly, but I am working on
modifying the Idris RTS to allow the integration of Idris code in the kernel

The kernel currently has a simple Memory Management Unit, allowing code
to allocate heap storage using `malloc` and free it using `free`.

Using [Meaty Skeleton](http://wiki.osdev.org/Meaty_Skeleton) as a template
