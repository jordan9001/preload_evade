# Self Auditing Elf
An fun way to escape LD_PRELOAD sandboxes

`ld.so` will load any audit modules before any preload modules get loaded. In this repo we make a small binary that registers itself as an audit module to execute code before a preloaded module has a chance to load. This can be used to escape weak sandboxes that use `LD_PRELOAD` to enforce anything.

`ld.so` normally will refuse to load a PIE binary as an audit module, so we have to scrub the `DT_FLAGS_1` for it to work.

An interesting thing this test also points out is that preloaded modules can have their constructors redefined by the binary they are preloaded into, if the symbols match.

To run the test, run `make run`
```sh
gcc -fPIE -pie -o main_norm main.c
gcc -o modelf modelf.c
gcc -fPIE -pie -Wl,--audit,./main_audit,-E -o main_audit main.c
./modelf main_audit
Found DT_FLAGS_1 =          8000000
Modified main_audit
gcc -fPIE -pie -Wl,-E -o main_tamper main.c
gcc -fPIC -shared -o pld.so preload.c

Normal:
LD_PRELOAD="./pld.so" ./main_norm
preload.c: preload_init
preload.c: init
main.c: init
main.c: main

Self Auiting:
LD_PRELOAD="./pld.so" ./main_audit
main.c: init
main.c: la_version
preload.c: preload_init
main.c: init
main.c: init
main.c: main

Tampered:
LD_PRELOAD="./pld.so" ./main_tamper
preload.c: preload_init
main.c: init
main.c: init
main.c: main
```
_(using ld version 2.32 and gcc version 10.21)_

