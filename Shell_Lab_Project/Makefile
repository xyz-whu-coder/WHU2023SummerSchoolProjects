.PHONY: run clean
EXENAME = main args check
run:
	make clean
	chmod 755 main_sol
	rm -rf output || true
	mkdir output
	make -C src/ all
	g++ -o args args.cpp -std=c++11
	g++ -o check check.cpp -std=c++11
	./check
clean:
	make -C src/ clean
	-rm -rf $(EXENAME) || true
	rm output/*.txt || true

