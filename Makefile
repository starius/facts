
BUILD = debug
MODE = http
PORT = 5711
ADDRESS = 127.0.0.1
RUN_USER = www-data
RUN_GROUP = www-data
INTEGRATE_INTO = # nginx,monit
URL_PREFIX =
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

.SECONDEXPANSION:

QMAKE_TARGET = facts-$(MODE).wt
EXE = ./$(BUILD)/$(QMAKE_TARGET)
ifneq (,$(findstring install,$(MAKECMDGOALS))) # install or uninstall
BUILD = release
WT_CONFIG = $(WT_CONFIG_INSTALL)
EXE_PATH = $(bindir)/$(QMAKE_TARGET)
APPROOT = $(localstatedir)/lib/facts
DOCROOT_PARENT = $(datadir)/facts
STARTER = $(bindir)/facts
FCGI_RUN_DIR = $(VAR_RUN)
PID_FILE = $(VAR_RUN)/facts.pid
LOG_FILE = $(localstatedir)/log/facts.log
SOCKET = $(VAR_RUN)/facts.socket
else
WT_CONFIG = $(BUILD)/wt_config_$(MODE).xml
EXE_PATH = $(EXE)
APPROOT = .
DOCROOT_PARENT = .
FCGI_RUN_DIR = $(BUILD)/run
PID_FILE = $(BUILD)/facts-$(MODE).pid
SOCKET = $(BUILD)/facts-$(MODE).socket
endif
DOCROOT = $(DOCROOT_PARENT)/files
NGINX_SCRIPT_NAME = fastcgi_param	SCRIPT_NAME		/$(URL_PREFIX)

ifeq ($(MODE), http)
RUN_COMMAND = WT_CONFIG_XML=$(WT_CONFIG) $(EXE_PATH) --http-address=$(ADDRESS) --http-port=$(PORT) \
			  --docroot="$(DOCROOT)/;/resources,/img,/js,/css,/tinymce,/favicon.ico" -p $(PID_FILE)
else
RUN_COMMAND = WT_CONFIG_XML=$(WT_CONFIG) spawn-fcgi -n -f $(EXE_PATH) -P $(PID_FILE)
ifeq (,$(SOCKET))
RUN_COMMAND += -a $(ADDRESS) -p $(PORT)
else
RUN_COMMAND += -s $(SOCKET)
endif
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
	chown -R $(RUN_USER):$(RUN_GROUP) $(WT_CONFIG)
	mkdir -p $(bindir)
	cp $(EXE) $(bindir)
	mkdir -p $(APPROOT) $(DOCROOT_PARENT)
	cp -ra files locales $(DOCROOT_PARENT)
	if [ -h $(APPROOT)/locales ]; then rm $(APPROOT)/locales; fi
	ln -s $(DOCROOT_PARENT)/locales $(APPROOT)/locales
	chown -R $(RUN_USER):$(RUN_GROUP) $(APPROOT) $(DOCROOT_PARENT)
	chmod 770 $(APPROOT)
	echo '#!/bin/sh' > $(STARTER)
	echo '$(RUN_COMMAND)' >> $(STARTER)
	chmod +x $(STARTER)
	mkdir -p $(VAR_RUN)
	chown $(RUN_USER):$(RUN_GROUP) $(VAR_RUN)
	chmod 770 $(VAR_RUN)
ifneq (,$(LOG_FILE))
	mkdir -p $(dir $(LOG_FILE))
	touch $(LOG_FILE)
	chown $(RUN_USER):$(RUN_GROUP) $(LOG_FILE)
	chmod 660 $(LOG_FILE)
endif
ifneq (,$(findstring nginx,$(INTEGRATE_INTO)))
	cp --backup=numbered nginx.in $(NGINX_CONF)
	sed 's@HOST_NAME@$(HOST_NAME)@' -i $(NGINX_CONF)
	sed 's@URL_PREFIX@$(URL_PREFIX)@' -i $(NGINX_CONF)
	sed 's@DOCROOT@$(DOCROOT)@' -i $(NGINX_CONF)
ifeq ($(MODE), http)
	sed 's@BACKEND@proxy_pass http://$(ADDRESS):$(PORT)/@' -i $(NGINX_CONF)
	sed 's@PARAMS@proxy_set_header X-Forwarded-Host $$server_name@' -i $(NGINX_CONF)
else
	sed 's@PARAMS@$(NGINX_SCRIPT_NAME);\
		include fastcgi_params;\
		$(NGINX_SCRIPT_NAME);\
		if ($$document_uri ~ "^/$(URL_PREFIX)(.*)") { set $$apache_path_info /$$1; }\
		fastcgi_param PATH_INFO $$apache_path_info@' -i $(NGINX_CONF)
ifneq (,$(SOCKET))
	sed 's@BACKEND@fastcgi_pass unix:$(SOCKET)@' -i $(NGINX_CONF)
else
	sed 's@BACKEND@fastcgi_pass $(ADDRESS):$(PORT)@' -i $(NGINX_CONF)
endif
endif
	if [ -h $(NGINX_CONF2) ]; then rm $(NGINX_CONF2); fi
	ln -s $(NGINX_CONF) $(NGINX_CONF2)
endif
ifneq (,$(findstring monit,$(INTEGRATE_INTO)))
	cp --backup=numbered monit.in $(MONIT_CONF)
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

$(WT_CONFIG): /etc/wt/wt_config.xml Makefile
	mkdir -p $(dir $@)
	cp --backup=numbered $< $@
	chmod 660 $@
ifeq ($(MODE), fcgi)
	mkdir -p $(FCGI_RUN_DIR)
	sed 's@$(FCGI_RUN_DIR_ORIGINAL)@$(FCGI_RUN_DIR)@' -i $@
	sed 's@<num-threads>1</num-threads>@<num-threads>2</num-threads>@' -i $@
endif
	sed 's@<log-file></log-file>@<log-file>$(LOG_FILE)</log-file>@' -i $@
	sed 's@<behind-reverse-proxy>false</behind-reverse-proxy>@<behind-reverse-proxy>true</behind-reverse-proxy>@' -i $@
	sed 's@</properties>@<property name="approot">$(APPROOT)</property>\
		<property name="tinyMCEBaseURL">tinymce/</property>\
		<property name="adminPassword">asuperes</property>\
		</properties>@' -i $@

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

