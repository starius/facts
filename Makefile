
build: debug

release debug: $(wildcard src/**)
	mkdir -p $@
	if [ ! -d $@/locales ]; then ln -s ../locales $@/locales; fi
	if [ ! -d $@/files ]; then ln -s ../files $@/files; fi
	qmake "CONFIG-=release" "CONFIG-=debug" "CONFIG+=$@" src/facts.pro -o $@
	$(MAKE) -C $@


