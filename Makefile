CC=gcc

clean:
	rm -f minijvm.o mjava

test1: mjava
	./mjava testfiles/test1.mclass

test2: mjava
	./mjava testfiles/test2.mclass

test3: mjava
	./mjava testfiles/test3.mclass

test4: mjava
	./mjava testfiles/test4.mclass

test5: mjava
	./mjava testfiles/test5.mclass

test6: mjava
	./mjava testfiles/test6.mclass

test7: mjava
	./mjava testfiles/test7.mclass

test8: mjava
	./mjava testfiles/test8.mclass

test: test1 test2 test3 test4 test5 test6 test7 test8

