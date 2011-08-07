
BUILD = debug
MODE = http
PORT = 5711
ADDRESS = 127.0.0.1
RUN_USER = www-data
RUN_GROUP = www-data
INTEGRATE_INTO = nginx,monit
NGINX_PREFIX =
SERVER_NAME = facts
NGINX_CONF = /etc/nginx/sites-available/facts

prefix = /usr/local
exec_prefix = $(prefix)
datarootdir = $(prefix)/share
datadir = $(datarootdir)
sysconfdir = $(prefix)/etc
localstatedir = $(prefix)/var
bindir = $(exec_prefix)/bin

FCGI_RUN_DIR_ORIGINAL = /var/run/wt # as specified in /etc/wt/wt_config.xml
FCGI_RUN_DIR_INSTALL = $(localstatedir)/run/wt
WT_CONFIG_INSTALL = $(sysconfdir)/facts/wt_config.xml
NGINX_CONF2 = $(subst available,enabled,$(NGINX_CONF))

.SECONDEXPANSION:

QMAKE_TARGET = facts-$(MODE).wt
EXE = ./$(BUILD)/$(QMAKE_TARGET)
ifneq (,$(findstring install,$(MAKECMDGOALS))) # install or uninstall
BUILD = release
WT_CONFIG = $(WT_CONFIG_INSTALL)
EXE_PATH = $(bindir)/$(QMAKE_TARGET)
APPROOT = $(localstatedir)/lib/facts
STARTER = $(bindir)/facts
FCGI_RUN_DIR = $(FCGI_RUN_DIR_INSTALL)
else
WT_CONFIG = $(BUILD)/wt_config_$(MODE).xml
EXE_PATH = $(EXE)
APPROOT = .
FCGI_RUN_DIR = $(BUILD)/run
endif
DOCROOT_PARENT = $(datadir)/facts
DOCROOT = $(DOCROOT_PARENT)/files

ifeq ($(MODE), http)
RUN_COMMAND = WT_CONFIG_XML=$(WT_CONFIG) $(EXE_PATH) --http-address=$(ADDRESS) --http-port=$(PORT) \
			  --docroot="$(DOCROOT)/;/resources,/img,/js,/css,/tinymce,/favicon.ico"
else
RUN_COMMAND = WT_CONFIG_XML=$(WT_CONFIG) spawn-fcgi -n -f $(EXE_PATH) -a $(ADDRESS) -p $(PORT)
endif

build: $$(EXE)

$(EXE): $(wildcard src/**) src/facts.pro
	mkdir -p $(BUILD)
	qmake "CONFIG-=release" "CONFIG-=debug" "CONFIG+=$(BUILD)" \
		"LIBS+=-lwt$(MODE)" "TARGET=$(QMAKE_TARGET)" src/facts.pro -o $(BUILD)
	$(MAKE) -C $(BUILD)

run: $(EXE) images $$(WT_CONFIG)
	$(RUN_COMMAND)

install: $$(EXE) images $$(WT_CONFIG)
	mkdir -p $(bindir)
	cp $(EXE) $(bindir)
	mkdir -p $(APPROOT) $(DOCROOT_PARENT)
	cp -ra files locales $(DOCROOT_PARENT)
	if [ ! -d $(APPROOT)/locales ]; then ln -s $(DOCROOT_PARENT)/locales $(APPROOT); fi
	chown -R $(RUN_USER):$(RUN_GROUP) $(APPROOT) $(DOCROOT_PARENT)
	echo '#!/bin/sh' > $(STARTER)
	echo '$(RUN_COMMAND)' > $(STARTER)
	chmod +x $(STARTER)
ifeq ($(MODE), fcgi)
	mkdir -p $(FCGI_RUN_DIR_INSTALL)
	chown $(RUN_USER):$(RUN_GROUP) $(FCGI_RUN_DIR_INSTALL)
endif
ifneq (,$(findstring nginx,$(INTEGRATE_INTO)))
	cp --backup nginx.in $(NGINX_CONF)
	sed 's@SERVER_NAME@$(SERVER_NAME)@' -i $(NGINX_CONF)
	sed 's@NGINX_PREFIX@$(NGINX_PREFIX)@' -i $(NGINX_CONF)
	sed 's@DOCROOT_PARENT@$(DOCROOT_PARENT)@' -i $(NGINX_CONF)
	sed 's@PORT@$(PORT)@' -i $(NGINX_CONF)
	if [ ! -f $(NGINX_CONF2) ]; then ln -s $(NGINX_CONF) $(NGINX_CONF2); fi
endif

uninstall:
	rm $(EXE_PATH) $(STARTER)
	rm -r $(DOCROOT)

install-ubuntu:
	$(MAKE) install prefix=/usr sysconfdir=/etc localstatedir=/var

uninstall-ubuntu:
	$(MAKE) uninstall prefix=/usr sysconfdir=/etc localstatedir=/var

wt_config.xml:
	cp /etc/wt/wt_config.xml .

$(WT_CONFIG): wt_config.xml
	mkdir -p $(dir $@)
	cp --backup $< $@
ifeq ($(MODE), fcgi)
	mkdir -p $(FCGI_RUN_DIR)
	sed 's@$(FCGI_RUN_DIR_INSTALL)@$(FCGI_RUN_DIR)@' -i $@
endif
	sed 's@</properties>@<property name="approot">$(APPROOT)</property></properties>@' -i $@

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

