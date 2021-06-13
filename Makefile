CC=gcc
CFLAGS=-Wall -Wextra -Werror -g


.PHONY: all
all: main_norm main_audit main_tamper pld.so

main_norm: main.c write.h
	# Need a DYN binary that doesn't have pie flag set but has an entry?
	$(CC) $(CFLAGS) -fPIE -pie -o $@ $<

main_audit: main.c write.h modelf
	$(CC) $(CFLAGS) -fPIE -pie -Wl,--audit,./$@,-E -o $@ $<
	./modelf $@

main_tamper: main.c write.h
	$(CC) $(CFLAGS) -fPIE -pie -Wl,-E -o $@ $<

pld.so: preload.c write.h
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $<

modelf: modelf.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: run
run: all
	@echo -e "\nNormal:"
	LD_PRELOAD="./pld.so" ./main_norm
	@echo -e "\nSelf Auiting:"
	LD_PRELOAD="./pld.so" ./main_audit
	@echo -e "\nTampered:"
	LD_PRELOAD="./pld.so" ./main_tamper

.PHONY: clean
clean:
	rm -f main_*
	rm -f pld.so
	rm -f modelf
