# -fno-omit-frame-pointer

Don't keep the frame pointer in a register for functions that don't need one.
This avoids the instructions to save, set up and restore frame pointers;
it also makes an extra register available in many functions.
It also makes debugging impossible on some machines.

不把 frame_pointer 存到寄存器里
