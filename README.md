Poly OS
===

An attempt at writing an Operating System that is written in
many languages not usually used for systems programming.

Right now it's mostly C, Assembly, and Rust, with Idris
almost working (using a modified RTS).

Languages I might try using
===

- Rust
    - Fairly easy to run on bare metal
- Idris
    - Requires modifications to RTS
- D
    - Fairly easy to run on bare metal
- Nim
    - Fairly easy to run on bare metal (?)
- ATS
    - Fairly easy to run on bare metal (?)
    - Really ugly syntax, might want to write a preprocessor / new parser
- Haskell
    - Modify JHC RTS to remove unsupported operations
    - Modify GHC LLVM RTS (? This might be too difficult)
- Mercury
    - Efficient (low heap usage?) because of linear types
    - Has a "high-level" C code generator that might be better suited to writing low-level code
- Clean
    - Efficient (low heap usage?) because of linear types
    - Modify RTS
- Coq
    - Via [coq-compile](https://github.com/coq-ext-lib/coq-compile). It would be pretty cool to have Coq code in a kernel...
        - coq-compile relies on an old version of LLVM (3.1), so I have to either rewrite coq-compile or use an old version of LLVM
- Java / Scala
    - Via [RoboVM](https://github.com/robovm/robovm) (might be similar projects that are better)

Using [Meaty Skeleton](http://wiki.osdev.org/Meaty_Skeleton) as a template
