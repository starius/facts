
build: debug

run: run-debug

all: debug release images


%/facts.wt: $(wildcard src/**) src/facts.pro
	mkdir -p $*
	qmake "CONFIG-=release" "CONFIG-=debug" "CONFIG+=$*" src/facts.pro -o $*
	$(MAKE) -C $*

.SECONDEXPANSION:
release debug: $$@/facts.wt

run-debug: debug images
	./$</facts.wt --http-address=0.0.0.0 \
	--http-port=5711 --docroot="files/;/resources,/img,/js,/css,/tinymce,/favicon.ico"

images: files/favicon.ico files/img/logo.png files/img/update.png \
	files/img/right-arrow.png files/img/left-arrow.png files/img/up-arrow.png files/img/down-arrow.png

files/favicon.ico: files/favicon.svg
	inkscape -z -w 16 -e $@ $<

files/img/logo.png: files/favicon.svg
	inkscape -z -w 200 -e $@ $<

files/img/update.png: files/img/update.svg
	inkscape -z -w 64 -e $@ $<

files/img/right-arrow.png: files/img/right-arrow.svg
	inkscape -z -w 32 -e $@ $<
files/img/left-arrow.png: files/img/right-arrow.png
	convert $< -rotate 180 $@
files/img/up-arrow.png: files/img/right-arrow.png
	convert $< -rotate -90 $@
files/img/down-arrow.png: files/img/right-arrow.png
	convert $< -rotate 90 $@


