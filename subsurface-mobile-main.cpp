// SPDX-License-Identifier: GPL-2.0
/* main.c */
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "core/dive.h"
#include "core/qt-gui.h"
#include "core/subsurfacestartup.h"
#include "core/color.h"
#include "core/qthelper.h"
#include "core/helpers.h"
#include "core/downloadfromdcthread.h"

#include <QStringList>
#include <QApplication>
#include <QLoggingCategory>
#include <QLocale>
#include <git2.h>

int main(int argc, char **argv)
{
	int i;
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
	QApplication *application = new QApplication(argc, argv);
	(void)application;
	QStringList arguments = QCoreApplication::arguments();

	subsurface_console_init();

	for (i = 1; i < arguments.length(); i++) {
		QString a = arguments.at(i);
		if (!a.isEmpty() && a.at(0) == '-') {
			parse_argument(a.toLocal8Bit().data());
			continue;
		}
	}
	git_libgit2_init();
	setup_system_prefs();
	if (QLocale().measurementSystem() == QLocale::MetricSystem)
		default_prefs.units = SI_units;
	else
		default_prefs.units = IMPERIAL_units;
	copy_prefs(&default_prefs, &prefs);
	fill_profile_color();
	fill_computer_list();

	parse_xml_init();
	taglist_init_global();
	init_ui();
	if (prefs.default_file_behavior == LOCAL_DEFAULT_FILE)
		set_filename(prefs.default_filename);
	else
		set_filename(NULL);

	// some hard coded settings
	prefs.animation_speed = 0; // we render the profile to pixmap, no animations

	// always show the divecomputer reported ceiling in red
	prefs.redceiling = 1;

	init_proxy();

	if (!quit)
		run_ui();
	exit_ui();
	taglist_free(g_tag_list);
	parse_xml_exit();
	subsurface_console_exit();
	free_prefs();
	return 0;
}
