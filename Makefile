
BUILD = debug
MODE = http
PORT = 5711
ADDRESS = 0.0.0.0

QMAKE_TARGET = facts-$(MODE).wt
EXE = ./$(BUILD)/$(QMAKE_TARGET)
WT_CONFIG = ./$(BUILD)/wt_config_$(MODE).xml

.SECONDEXPANSION:

build: $$(EXE)

$(EXE): $(wildcard src/**) src/facts.pro
	mkdir -p $(BUILD)
	qmake "CONFIG-=release" "CONFIG-=debug" "CONFIG+=$(BUILD)" \
		"LIBS+=-lwt$(MODE)" "TARGET=$(QMAKE_TARGET)" src/facts.pro -o $(BUILD)
	$(MAKE) -C $(BUILD)

run: $(EXE) images $$(WT_CONFIG)
ifeq ($(MODE), http)
	$(EXE) --http-address=$(ADDRESS) \
	--http-port=$(PORT) --docroot="files/;/resources,/img,/js,/css,/tinymce,/favicon.ico"
else
	WT_CONFIG_XML=$(WT_CONFIG) spawn-fcgi -n -f $(EXE) -a $(ADDRESS) -p $(PORT)
endif

wt_config.xml:
	cp /etc/wt/wt_config.xml .

release/wt_config_http.xml release/wt_config_fcgi.xml debug/wt_config_http.xml: wt_config.xml
	cp $< $@

debug/wt_config_fcgi.xml: wt_config.xml
	sed < $< > $@ s@/var/run/wt@debug/run@
	mkdir -p debug/run

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

