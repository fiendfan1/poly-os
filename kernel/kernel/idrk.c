#include <idris_rts.h>
#include <idris_bitstring.h>
#include <idris_stdfgn.h>
#include <kernel/alloc.h>
#include <kernel/tty.h>
void _idris_Main_46_fib_39_(VM*, VAL*);
void _idris_Main_46_main(VM*, VAL*);
void _idris_mkForeignPrim(VM*, VAL*);
void _idris_mkLazyForeignPrim(VM*, VAL*);
void _idris_prim_95__95_addInt(VM*, VAL*);
void _idris_prim_95__95_concat(VM*, VAL*);
void _idris_prim_95__95_subInt(VM*, VAL*);
void _idris_prim_95__95_toStrInt(VM*, VAL*);
void _idris_Prelude_46_putStr(VM*, VAL*);
void _idris_run_95__95_IO(VM*, VAL*);
void _idris_unsafePerformPrimIO(VM*, VAL*);
void _idris__123_APPLY0_125_(VM*, VAL*);
void _idris__123_EVAL0_125_(VM*, VAL*);
void _idris__123_runMain0_125_(VM*, VAL*);
void _idris_Decidable_46_Equality_46_Decidable_46_Equality_46__64_Decidable_46_Equality_46_DecEq_36_Char_58__33_decEq_58_0_58_primitiveNotEq_58_0(VM*, VAL*);
void _idris_Decidable_46_Equality_46_Decidable_46_Equality_46__64_Decidable_46_Equality_46_DecEq_36_Float_58__33_decEq_58_0_58_primitiveNotEq_58_0(VM*, VAL*);
void _idris_Decidable_46_Equality_46_Decidable_46_Equality_46__64_Decidable_46_Equality_46_DecEq_36_Int_58__33_decEq_58_0_58_primitiveNotEq_58_0(VM*, VAL*);
void _idris_Decidable_46_Equality_46_Decidable_46_Equality_46__64_Decidable_46_Equality_46_DecEq_36_Integer_58__33_decEq_58_0_58_primitiveNotEq_58_0(VM*, VAL*);
void _idris_Decidable_46_Equality_46_Decidable_46_Equality_46__64_Decidable_46_Equality_46_DecEq_36_String_58__33_decEq_58_0_58_primitiveNotEq_58_0(VM*, VAL*);
void _idris_Void_95_case(VM*, VAL*);
void _idris_Void_95_elim(VM*, VAL*);
void _idris_Main_46_fib_39_(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(2);
    ADDTOP(2);
    if (GETINT(LOC(0)) == 0) {
        RVAL = MKINT(0);
        TOPBASE(0);
        REBASE;
    } else
    if (GETINT(LOC(0)) == 1) {
        RVAL = MKINT(1);
        TOPBASE(0);
        REBASE;
    } else
    {
        LOC(1) = MKINT(1);
        LOC(1) = INTOP(-,LOC(0), LOC(1));
        RESERVE(1);
        TOP(0) = LOC(1);
        STOREOLD;
        BASETOP(0);
        ADDTOP(1);
        CALL(_idris_Main_46_fib_39_);
        LOC(1) = RVAL;
        LOC(2) = MKINT(2);
        LOC(2) = INTOP(-,LOC(0), LOC(2));
        RESERVE(1);
        TOP(0) = LOC(2);
        STOREOLD;
        BASETOP(0);
        ADDTOP(1);
        CALL(_idris_Main_46_fib_39_);
        LOC(2) = RVAL;
        RVAL = ADD(LOC(1), LOC(2));
        TOPBASE(0);
        REBASE;
    }
}

void _idris_Main_46_main(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(2);
    ADDTOP(2);
    LOC(0) = MKINT(40);
    RESERVE(1);
    TOP(0) = LOC(0);
    STOREOLD;
    BASETOP(0);
    ADDTOP(1);
    CALL(_idris_Main_46_fib_39_);
    LOC(0) = RVAL;
    LOC(0) = idris_castIntStr(vm, LOC(0));
    LOC(1) = MKSTR(vm, """\x0a""");
    LOC(0) = idris_concat(vm, LOC(0), LOC(1));
    allocCon(REG1, vm, 65637, 1, 0);
    SETARG(REG1, 0, LOC(0)); 
    RVAL = REG1;
    TOPBASE(0);
    REBASE;
}

void _idris_mkForeignPrim(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = NULL;
    TOPBASE(0);
    REBASE;
}

void _idris_mkLazyForeignPrim(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = NULL;
    TOPBASE(0);
    REBASE;
}

void _idris_prim_95__95_addInt(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = ADD(LOC(0), LOC(1));
    TOPBASE(0);
    REBASE;
}

void _idris_prim_95__95_concat(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = idris_concat(vm, LOC(0), LOC(1));
    TOPBASE(0);
    REBASE;
}

void _idris_prim_95__95_subInt(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = INTOP(-,LOC(0), LOC(1));
    TOPBASE(0);
    REBASE;
}

void _idris_prim_95__95_toStrInt(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = idris_castIntStr(vm, LOC(0));
    TOPBASE(0);
    REBASE;
}

void _idris_Prelude_46_putStr(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    putStr(GETSTR(LOC(0)));
    TOPBASE(0);
    REBASE;
}

void _idris_run_95__95_IO(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    LOC(1) = NULL_CON(0);
    RESERVE(2);
    TOP(0) = LOC(0);
    TOP(1) = LOC(1);
    SLIDE(vm, 2);
    TOPBASE(2);
    TAILCALL(_idris__123_APPLY0_125_);
}

void _idris_unsafePerformPrimIO(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = NULL;
    TOPBASE(0);
    REBASE;
}

void _idris__123_APPLY0_125_(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    switch(TAG(LOC(0))) {
    case 65637:
        PROJECT(vm, LOC(0), 2, 1);
        RESERVE(2);
        TOP(0) = LOC(2);
        TOP(1) = LOC(1);
        SLIDE(vm, 2);
        TOPBASE(2);
        TAILCALL(_idris_Prelude_46_putStr);
        break;
    default:
        RVAL = NULL;
        TOPBASE(0);
        REBASE;
        break;
    }
}

void _idris__123_EVAL0_125_(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    switch(TAG(LOC(0))) {
    default:
        RVAL = LOC(0);
        TOPBASE(0);
        REBASE;
        break;
    }
}

void _idris__123_runMain0_125_(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(2);
    ADDTOP(2);
    STOREOLD;
    BASETOP(0);
    CALL(_idris_Main_46_main);
    LOC(0) = RVAL;
    LOC(1) = NULL_CON(0);
    RESERVE(2);
    TOP(0) = LOC(0);
    TOP(1) = LOC(1);
    STOREOLD;
    BASETOP(0);
    ADDTOP(2);
    CALL(_idris__123_APPLY0_125_);
    LOC(0) = RVAL;
    RESERVE(1);
    TOP(0) = LOC(0);
    SLIDE(vm, 1);
    TOPBASE(1);
    TAILCALL(_idris__123_EVAL0_125_);
}

void _idris_Decidable_46_Equality_46_Decidable_46_Equality_46__64_Decidable_46_Equality_46_DecEq_36_Char_58__33_decEq_58_0_58_primitiveNotEq_58_0(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = NULL;
    TOPBASE(0);
    REBASE;
}

void _idris_Decidable_46_Equality_46_Decidable_46_Equality_46__64_Decidable_46_Equality_46_DecEq_36_Float_58__33_decEq_58_0_58_primitiveNotEq_58_0(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = NULL;
    TOPBASE(0);
    REBASE;
}

void _idris_Decidable_46_Equality_46_Decidable_46_Equality_46__64_Decidable_46_Equality_46_DecEq_36_Int_58__33_decEq_58_0_58_primitiveNotEq_58_0(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = NULL;
    TOPBASE(0);
    REBASE;
}

void _idris_Decidable_46_Equality_46_Decidable_46_Equality_46__64_Decidable_46_Equality_46_DecEq_36_Integer_58__33_decEq_58_0_58_primitiveNotEq_58_0(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = NULL;
    TOPBASE(0);
    REBASE;
}

void _idris_Decidable_46_Equality_46_Decidable_46_Equality_46__64_Decidable_46_Equality_46_DecEq_36_String_58__33_decEq_58_0_58_primitiveNotEq_58_0(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = NULL;
    TOPBASE(0);
    REBASE;
}

void _idris_Void_95_case(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = NULL;
    TOPBASE(0);
    REBASE;
}

void _idris_Void_95_elim(VM* vm, VAL* oldbase) {
    INITFRAME;
    RESERVE(1);
    ADDTOP(1);
    RVAL = NULL;
    TOPBASE(0);
    REBASE;
}

#include "idris_opts.h"
#include "idris_stats.h"
#include "idris_rts.h"
#include "idris_gmp.h"
// The default options should give satisfactory results under many circumstances.
RTSOpts opts = { 
    .init_heap_size = 4096000,
    .max_stack_size = 4096000,
    .show_summary   = 0
};

int idr_main(int argc, char* argv[]) {
    parse_shift_args(&opts, &argc, &argv);

    __idris_argc = argc;
    __idris_argv = argv;

    VM* vm = init_vm(opts.max_stack_size, opts.init_heap_size, 1);
    init_threadkeys();
    init_threaddata(vm);
    init_gmpalloc();

    init_nullaries();

    _idris__123_runMain0_125_(vm, NULL);

#ifdef IDRIS_DEBUG
    if (opts.show_summary) {
        idris_gcInfo(vm, 1);
    }
#endif

    Stats stats = terminate(vm);

    if (opts.show_summary) {
        print_stats(&stats);
    }

    free_nullaries();
    return EXIT_SUCCESS;
}

void kernel_early(void) {
    memory_init((void*)(0x100000 + 16385), 16385);
    terminal_initialize();
}

void kernel_main(void) {
    idr_main(0, 0);
}
