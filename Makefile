
build: debug

release debug: $(wildcard src/**)
	mkdir -p $@
	if [ ! -d $@/locales ]; then ln -s ../locales $@/locales; fi
	if [ ! -d $@/files ]; then ln -s ../files $@/files; fi
	qmake "CONFIG-=release" "CONFIG-=debug" "CONFIG+=$@" src/facts.pro -o $@
	$(MAKE) -C $@

run: run-debug

run-debug: debug
	./$</facts.wt --docroot $</files --http-address=0.0.0.0 --http-port=5711

run-release: release
	./$</facts.wt --docroot $</files --http-address=0.0.0.0 --http-port=5712

