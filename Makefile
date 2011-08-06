
build: debug

run: run-debug

all: debug release images

images: files/favicon.ico files/img/logo.png

files/favicon.ico: files/favicon.svg
	inkscape -z -w 16 -e $@ $<

files/img/logo.png: files/favicon.svg
	inkscape -z -w 200 -e $@ $<

%/facts.wt: $(wildcard src/**) src/facts.pro
	mkdir -p $*
	qmake "CONFIG-=release" "CONFIG-=debug" "CONFIG+=$*" src/facts.pro -o $*
	$(MAKE) -C $*

.SECONDEXPANSION:
release debug: $$@/facts.wt

run-debug: debug images
	./$</facts.wt --docroot=files --http-address=0.0.0.0 --http-port=5711

