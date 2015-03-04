#![feature(no_std)]
#![no_std]

#![feature(lang_items)]
#![feature(intrinsics)]

#![feature(core)]
extern crate core;

use core::iter::Iterator;
use core::mem::transmute;
use core::ptr::PtrExt;
use core::slice::*;
use core::str::{StrExt, next_code_point};
use core::marker::Copy;


#[lang="stack_exhausted"]
extern fn stack_exhausted() {
    unsafe {
        printf("Rust Error: Stack Exhausted".as_ptr() as *mut u8);
    }
}
#[lang="eh_personality"] extern fn eh_personality() {}
#[lang="panic_fmt"] fn panic_fmt() -> ! { loop {} }

extern {
    fn memory_init(heap_start: *mut u8, heap_size: usize);

    fn malloc(size: usize) -> *mut u8;
    fn free(ptr: *mut u8);

    fn printf(fmt: *mut u8);

    fn terminal_initialize();
}

pub struct VGA {
    buffer: *mut u16,
    width:  usize,
    height: usize
}
impl Copy for VGA {}

impl VGA {
    pub fn get_entry(&self, x: usize, y: usize) -> u16 {
        unsafe {
            *self.buffer.offset((self.width * y + x) as isize)
        }
    }

    pub fn put_entry(&self, x: usize, y: usize, entry: u16) {
        unsafe {
            *self.buffer.offset((self.width * y + x) as isize) = entry;
        }
    }

    pub fn putc(&self, ch: char, x: usize, y: usize,
                 fg: Color, bg: Color) {
        if x < self.width && y < self.height {
            self.put_entry(x, y, make_vga_entry(ch, make_color(fg, bg)));
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
    pub fn scroll(&mut self) {
        for y in (0..self.vga.height) {
            for x in (0..self.vga.width) {
                if y == self.vga.width-1 {
                    self.vga.put_entry(x, y+1,
                        make_vga_entry(' ',
                            make_color(self.foreground, self.background)));
                } else {
                    self.vga.put_entry(x, y+1, self.vga.get_entry(x, y));
                }
            }
        }
        self.x = 0;
        self.y = self.vga.height - 1;
    }

    pub fn put_char(&mut self, c: char) {
        // Handle newline
        if c == '\n' {
            for x in (self.x .. self.vga.width) {
                self.vga.putc(' ', x, self.y,
                              self.foreground, self.background);
            }
            self.x = 0;
            self.y = self.y + 1;

            return;
        }

        self.vga.putc(c, self.x, self.y, self.foreground, self.background);
        self.x += 1;

        if self.x >= self.vga.width && self.y >= self.vga.height {
            self.scroll();
            return;
        }
        if self.x >= self.vga.width {
            self.x  = 0;
            self.y += 1;
        }
        if self.y >= self.vga.height {
            self.x = 0;
            self.y = 0;
        }
    }

    pub fn print(&mut self, s: &'static str) {
        // for c in s.chars() isn't working
        // (weird undefined references)
        for c in s.bytes() {
            self.put_char(c as char);
        }
    }
}

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

unsafe fn write_cstr<'a>(cstr: *mut u8, rstr: &'a str) {
    let rasc = rstr.as_ptr() as *mut u8;
    for i in 0..(rstr.len()) {
        *cstr.offset(i as isize) = *rasc.offset(i as isize);
    }
    *cstr.offset(rstr.len() as isize) = '\0' as u8;
}

unsafe fn as_heap_cstr<'a>(rstr: &'a str) -> *mut u8 {
    let cstr: *mut u8 = malloc(rstr.len());
    write_cstr(cstr, rstr);
    cstr
}

fn printf_viac<'a>(rstr: &'a str) {
    /*
    unsafe {
        let cstr: *mut u8 = rstr.as_ptr() as *mut u8;
        printf(cstr);
    }
    */
    unsafe {
        let cstr: *mut u8 = as_heap_cstr(rstr);
        printf(cstr);
        free(cstr);
    }
}

#[no_mangle]
pub extern fn kernel_early() {
    unsafe {
        terminal_initialize();
        memory_init(1265985 as *mut u8, 131072);
//        memory_init((0x100000 + 131072) as *mut u8, 131072);
    }
    term_init(Color::Red, Color::Green);
}

#[no_mangle]
pub extern fn kernel_main() {
    let buf: *mut u16 = 0xB8000 as *mut u16;
    let mut term: Terminal = Terminal {
            x: 0, y: 0,
            foreground: Color::Red, background: Color::Green,
            vga: VGA {buffer: buf, width: 80, height: 25}
        };
    for i in (0..100) {
        term.print("Hello, World!\nHello, World 2!");
    }
    /*
    let init = 123 * 321 * 15;
    if init >= 0xE0 {
        terminal_initialize();
    }

    if (init << 12) | 3 > 0 {
        terminal_initialize();
    }
    */

    printf_viac("Hello, World!!!!!!");
}
