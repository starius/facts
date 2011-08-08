
BUILD = debug
MODE = http
PORT = 5711
ADDRESS = 127.0.0.1
RUN_USER = www-data
RUN_GROUP = www-data
INTEGRATE_INTO = # nginx,monit
NGINX_PREFIX =
HOST_NAME = facts
NGINX_CONF = /etc/nginx/sites-available/facts
MONIT_CONF = /etc/monit/conf.d/facts

prefix = /usr/local
exec_prefix = $(prefix)
datarootdir = $(prefix)/share
datadir = $(datarootdir)
sysconfdir = $(prefix)/etc
localstatedir = $(prefix)/var
bindir = $(exec_prefix)/bin

FCGI_RUN_DIR_ORIGINAL = /var/run/wt# as specified in /etc/wt/wt_config.xml
VAR_RUN = $(localstatedir)/run/facts
WT_CONFIG_INSTALL = $(sysconfdir)/facts/wt_config.xml
NGINX_CONF2 = $(subst available,enabled,$(NGINX_CONF))
PID_FILE = $(VAR_RUN)/facts.pid

.SECONDEXPANSION:

QMAKE_TARGET = facts-$(MODE).wt
EXE = ./$(BUILD)/$(QMAKE_TARGET)
ifneq (,$(findstring install,$(MAKECMDGOALS))) # install or uninstall
BUILD = release
WT_CONFIG = $(WT_CONFIG_INSTALL)
EXE_PATH = $(bindir)/$(QMAKE_TARGET)
APPROOT = $(localstatedir)/lib/facts
STARTER = $(bindir)/facts
FCGI_RUN_DIR = $(VAR_RUN)
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
			  --docroot="$(DOCROOT)/;/resources,/img,/js,/css,/tinymce,/favicon.ico" -p $(PID_FILE)
else
RUN_COMMAND = WT_CONFIG_XML=$(WT_CONFIG) spawn-fcgi -n -f $(EXE_PATH) -a $(ADDRESS) -p $(PORT) -P $(PID_FILE)
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
	chmod 770 $(APPROOT)
	echo '#!/bin/sh' > $(STARTER)
	echo '$(RUN_COMMAND)' >> $(STARTER)
	chmod +x $(STARTER)
	mkdir -p $(VAR_RUN)
	chown $(RUN_USER):$(RUN_GROUP) $(VAR_RUN)
	chmod 770 $(VAR_RUN)
ifneq (,$(findstring nginx,$(INTEGRATE_INTO)))
	cp --backup nginx.in $(NGINX_CONF)
	sed 's@HOST_NAME@$(HOST_NAME)@' -i $(NGINX_CONF)
	sed 's@NGINX_PREFIX@$(NGINX_PREFIX)@' -i $(NGINX_CONF)
	sed 's@DOCROOT@$(DOCROOT)@' -i $(NGINX_CONF)
	sed 's@PORT@$(PORT)@' -i $(NGINX_CONF)
	sed 's@ADDRESS@$(ADDRESS)@' -i $(NGINX_CONF)
ifeq ($(MODE), http)
	sed 's@fastcgi_pass@proxy_pass@' -i $(NGINX_CONF)
	sed 's@include fastcgi_params;@proxy_set_header X-Forwarded-Host $$server_name;@' -i $(NGINX_CONF)
	sed 's@$(ADDRESS):$(PORT)@http://$(ADDRESS):$(PORT)/@' -i $(NGINX_CONF)
endif
	if [ ! -f $(NGINX_CONF2) ]; then ln -s $(NGINX_CONF) $(NGINX_CONF2); fi
endif
ifneq (,$(findstring monit,$(INTEGRATE_INTO)))
	cp --backup monit.in $(MONIT_CONF)
	sed 's@PID_FILE@$(PID_FILE)@' -i $(MONIT_CONF)
	sed 's@STARTER@$(STARTER)@' -i $(MONIT_CONF)
	sed 's@RUN_USER@$(RUN_USER)@' -i $(MONIT_CONF)
	sed 's@RUN_GROUP@$(RUN_GROUP)@' -i $(MONIT_CONF)
	sed 's@ADDRESS@$(ADDRESS)@' -i $(MONIT_CONF)
	sed 's@PORT@$(PORT)@' -i $(MONIT_CONF)
endif

uninstall:
	rm $(EXE_PATH) $(STARTER)
	rm -r $(DOCROOT)

install-ubuntu:
	$(MAKE) install prefix=/usr sysconfdir=/etc localstatedir=/var

uninstall-ubuntu:
	$(MAKE) uninstall prefix=/usr sysconfdir=/etc localstatedir=/var

wt_config.xml: /etc/wt/wt_config.xml
	cp $< $@

$(WT_CONFIG): wt_config.xml
	mkdir -p $(dir $@)
	cp --backup $< $@
ifeq ($(MODE), fcgi)
	mkdir -p $(FCGI_RUN_DIR)
	sed 's@$(FCGI_RUN_DIR_ORIGINAL)@$(FCGI_RUN_DIR)@' -i $@
	sed 's@<num-threads>1</num-threads>@<num-threads>2</num-threads>@' -i $@
endif
	sed 's@<behind-reverse-proxy>false</behind-reverse-proxy>@<behind-reverse-proxy>true</behind-reverse-proxy>@' -i $@
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

