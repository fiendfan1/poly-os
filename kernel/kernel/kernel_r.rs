//#![crate_type="staticlib"]
//#![crate_id="myos#0.0.1"]
#![no_std]

//#![feature(intrinsics)]
#![feature(lang_items)]

/*
#![allow(improper_ctypes)]

#![deny(managed_heap_memory)]
#![deny(non_uppercase_statics)]
#![deny(unnecessary_qualification)]
#![deny(unnecessary_typecast)]
#![deny(unused_result)]
#![allow(unused_variable)]
*/

/*
extern crate core;
use core::prelude::*;
*/

//extern crate rlibc;

#[lang="sized"] pub trait Sized {}
#[lang="copy"] pub trait Copy {}

// EXTERNAL

extern {
    fn terminal_initialize();
//    fn printf(format: &str);
}

/*
extern "rust-intrinsic" {
    fn transmute<T, U>(x: T) -> U;
}

// Str helpers
#[no_stack_check]
fn as_bytes(s: &str) -> &[u8] {
    unsafe {transmute(s)}
}
*/

// POINTER ARITH
/*
#[no_stack_check]
fn offset<T>(p: *const T, offset: usize) -> *const T {
    let ptr = p as usize;
    (ptr + offset) as *const T
}
*/
#[no_stack_check]
fn offset_mut<T>(p: *mut T, offset: usize) -> *mut T {
    let ptr = p as usize;
    (ptr + offset) as *mut T
}

// ITERATOR (Not working)

/*
pub enum Option<T> {
    Some(T),
    None,
}

#[lang="iterator"] trait Iterator<A> {
    fn next(&mut self) -> Option<A>;
}

pub struct Range {
    curr: usize,
    stop: usize,
}

impl Iterator<usize> for Range {
    #[no_stack_check]
    #[no_mangle]
    fn next(&mut self) -> Option<usize> {
        if self.curr == self.stop {
            Option::None
        } else {
            let next = self.curr;
            self.curr += 1;
            Option::Some(next)
        }
    }
}

fn range(start: usize, stop: usize) -> Range {
    Range { curr: start, stop: stop }
}
*/

////

/*
pub struct VGA {
    buffer: *mut u16,
    width:  usize,
    height: usize
}
impl Copy for VGA {}

impl VGA {
    #[no_stack_check]
    pub fn putc(&mut self, ch: char, x: usize, y: usize,
                 fg: Color, bg: Color) {
        if x < self.width && y < self.height {
            let mt: *mut u16 = offset_mut(self.buffer, self.width * y + x);
            unsafe {
                *mt = make_vga_entry(ch, make_color(fg, bg));
            }
        }
    }
}

pub struct Terminal {
    x: usize,
    y: usize,
    foreground: Color,
    background: Color,
    vga: VGA
}
impl Copy for Terminal {}

impl Terminal {
    pub fn put_char(&mut self, c: char) {
//        self.vga.putc(c, self.x, self.y, self.foreground, self.background);

        self.x += 1;
        if self.x >= self.vga.width {
            self.x  = 0;
            self.y += 1;
        }
        if self.y >= self.vga.height {
            self.x = 0;
            self.y = 0;
        }
    }

    /*
    pub fn print(&mut self, s: &'static str) {
        for c in as_bytes(s).iter() {
            self.put_char(c);
        }
    }
    */
}

*/

// COLOR STUFF

pub enum Color {
    Black      = 0,
    Blue       = 1,
    Green      = 2,
    Cyan       = 3,
    Red        = 4,
    Pink       = 5,
    Brown      = 6,
    LightGray  = 7,
    DarkGray   = 8,
    LightBlue  = 9,
    LightGreen = 10,
    LightCyan  = 11,
    LightRed   = 12,
    LightPink  = 13,
    Yellow     = 14,
    White      = 15
}
impl Copy for Color {}


fn make_color(fg: Color, bg: Color) -> u8 {
    (fg as u8) | (bg as u8) << 4
}


fn make_vga_entry(c: char, color: u8) -> u16 {
    (c as u16) | (color as u16) << 8
}

#[no_stack_check]
fn term_init(foreground: Color, background: Color) {
    let color: u8 = make_color(foreground, background);
    let entry: u16 = make_vga_entry(' ', color);
    let mut i: isize = 0;
    while i < 80*25 {
        unsafe {
            *((0xB8000 + i * 2) as *mut u16) = entry;
        }
        i = i+1;
    }
}

#[no_mangle]
#[no_stack_check]
pub extern fn kernel_early() {
    unsafe {
        term_init(Color::Red, Color::Green);
//        terminal_initialize();
    }
}

#[no_mangle]
#[no_stack_check]
pub extern fn kernel_main() {
}

//#[lang = "stack_exhausted"] extern fn stack_exhausted() {}
//#[lang = "eh_personality"] extern fn eh_personality() {}
//#[lang = "panic_fmt"] fn panic_fmt() -> ! { loop {} }
