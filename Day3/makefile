NAME = day1
CFILE = main
CPP = g++-12
CFLAGS = -O0 -std=c++23 -g -pthread
LDLIBS = `pkg-config --libs fmt`

$(NAME): $(CFILE).o
	echo $(LDLIBS)

	# link
	$(CPP) -o out/$(NAME) out/$(CFILE).o $(CFLAGS) $(LDLIBS)

	# run
	################################################
	./out/$(NAME)

$(CFILE).o: $(CFILE).cpp

	mkdir -p out

	# preprocessor out
	# $(CPP) -o out/$(CFILE)_prepro0.cpp -E $(CFILE).cpp $(CFLAGS)

	# format after preprocessor
	# clang-format  out/$(CFILE)_prepro0.cpp > out/$(CFILE)_prepro1.cpp

	# template expansion
	# -clang++ -std=c++20 -Xclang -ast-print -fsyntax-only $(CFILE).cpp 2> out/$(CFILE)_ast_stderr.log 1> out/$(CFILE)_ast.cpp

	# obj
	$(CPP) -o out/$(CFILE).o -c $(CFILE).cpp $(CFLAGS)

	# objdump
	# objdump --dwarf=decodedline -S -M intel -d out/$(CFILE).o > out/$(CFILE).objdump

