# hgrdecode



# Code Cleanup

I am refactoring the code to put all the configuration stuff that rarely changes into a global config struct; instead of passing pointers all over the place and probably copying data values in when passing around the stack.

# Performance

It's perfectly fine, it's taking about 55-60ms to decode one frame. Fine for a static image conversion. It is however too slow for live decoding for the emulator.

Setting a couple funcs inline, and doing -O3, I am now doing it in 6.485ms to 9ms. Whoa.
It was doing function calls inside loops. So the inline probably helped a lot.
So if we have to redraw the entire screen every frame in the emulator, this will take 1/2 of "realtime" but it will work. And of course most stuff doesn't do that.

I bet we can do some SIMD stuff on these, working on an entire scanline at a time.
