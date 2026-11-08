AC_PREREQ([2.63])
AC_INIT([growl-for-linux], m4_esyscmd([cat VERSION | tr -d '\n']), [mattn.jp@gmail.com])
AC_CONFIG_SRCDIR([gol.c])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign])

# Smart output for automake.
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

# Enable -Werror.
AC_ARG_ENABLE([werror], [AS_HELP_STRING([--enable-werror], [enable -Werror])], [werror_flag=yes])
case ${werror_flag} in
    yes) CFLAGS="-Werror ${CFLAGS}";;
esac

# Checks for libraries.

PKG_CHECK_MODULES(GLIB2, glib-2.0)
PKG_CHECK_MODULES(GTHREAD2, gthread-2.0)
PKG_CHECK_MODULES(GMODULE2, gmodule-2.0)
PKG_CHECK_MODULES(LIBCURL, libcurl)
PKG_CHECK_MODULES(LIBXML2, libxml-2.0)
PKG_CHECK_MODULES(OPENSSL, openssl)
PKG_CHECK_MODULES(SQLITE3, sqlite3)
PKG_CHECK_MODULES(NOTIFY, libnotify,
                  [ENABLE_LIBNOTIFY="display/libnotify"
                   AC_SUBST([ENABLE_LIBNOTIFY])
                   AC_CONFIG_FILES([display/libnotify/Makefile])],
                  [ENABLE_LIBNOTIFY=""])

dnl GTK+2 and GTK+3 are exclusive: exactly one of them is used.
AC_ARG_WITH(gtk3,
            AS_HELP_STRING([--with-gtk3[=@<:@no/auto/yes@:>@]],
		           [Use GTK+3 instead of GTK+2]),
              [with_gtk3=$withval],
              [with_gtk3="auto"])
REQUIRED_GTK3=3.0.12
if test x$with_gtk3 = xauto ; then
        PKG_CHECK_EXISTS([gtk+-3.0 >= $REQUIRED_GTK3],
                         with_gtk3="yes",
                         with_gtk3="no")
fi
if test x$with_gtk3 = xyes ; then
        PKG_CHECK_MODULES(GTK3, gtk+-3.0 >= $REQUIRED_GTK3)
        AC_DEFINE(USE_GTK3, [1], [Use GTK+3 instead of GTK+2])
        APPINDICATOR_MODULE=appindicator3-0.1
        AYATANA_APPINDICATOR_MODULE=ayatana-appindicator3-0.1
else
        PKG_CHECK_MODULES(GTK2, gtk+-2.0)
        APPINDICATOR_MODULE=appindicator-0.1
        AYATANA_APPINDICATOR_MODULE=ayatana-appindicator-0.1
fi
AM_CONDITIONAL(USE_GTK3, test x"$with_gtk3" = xyes)

dnl gtk-layer-shell puts popups at fixed positions on Wayland.  Without it,
dnl gol falls back to XWayland on Wayland sessions.
GTK_LAYER_SHELL_REQUIRED=0.5.0
AC_ARG_WITH(gtk-layer-shell,
            AS_HELP_STRING([--with-gtk-layer-shell[=@<:@no/auto/yes@:>@]],
                           [Show popups natively on Wayland with gtk-layer-shell]),
              [with_gtk_layer_shell=$withval],
              [with_gtk_layer_shell="auto"])
if test x$with_gtk3 != xyes ; then
        if test x$with_gtk_layer_shell = xyes ; then
                AC_MSG_ERROR([gtk-layer-shell needs --with-gtk3])
        fi
        with_gtk_layer_shell="no"
fi
if test x$with_gtk_layer_shell = xauto ; then
        PKG_CHECK_EXISTS([gtk-layer-shell-0 >= $GTK_LAYER_SHELL_REQUIRED wayland-client],
                         with_gtk_layer_shell="yes",
                         with_gtk_layer_shell="no")
fi
if test x$with_gtk_layer_shell = xyes ; then
        PKG_CHECK_MODULES(GTK_LAYER_SHELL, gtk-layer-shell-0 >= $GTK_LAYER_SHELL_REQUIRED)
        PKG_CHECK_MODULES(WAYLAND_CLIENT, wayland-client)
        AC_DEFINE(HAVE_GTK_LAYER_SHELL, [1], [Have gtk-layer-shell])
fi

dnl --with-appindicator selects the AppIndicator library, so that the build
dnl (for example debian/rules) shows which one is used.
dnl   ayatana-glib: libayatana-appindicator-glib.  It replaces the deprecated
dnl                 libayatana-appindicator and uses GMenu instead of GtkMenu.
dnl   ayatana:      the Ayatana AppIndicator (libayatana-appindicator).
dnl   appindicator: Canonical's libappindicator.
dnl The last two have the same API, but a different pkg-config module name and
dnl include dir.  auto tries them in this order.
AYATANA_APPINDICATOR_GLIB_MODULE=ayatana-appindicator-glib
APPINDICATOR_REQUIRED=0.4.92
AC_ARG_ENABLE(appindicator,
              AS_HELP_STRING([--enable-appindicator[=@<:@no/auto/yes@:>@]],
                             [Build support for application indicators ]),
              [enable_appindicator=$enableval],
              [enable_appindicator="auto"])
AC_ARG_WITH(appindicator,
            AS_HELP_STRING([--with-appindicator=@<:@auto/ayatana-glib/ayatana/appindicator@:>@],
                           [Select the AppIndicator library @<:@default=auto@:>@]),
              [with_appindicator=$withval],
              [with_appindicator="auto"])
case "$with_appindicator" in
auto|yes)
        appindicator_modules="$AYATANA_APPINDICATOR_GLIB_MODULE $AYATANA_APPINDICATOR_MODULE $APPINDICATOR_MODULE" ;;
ayatana-glib)
        appindicator_modules="$AYATANA_APPINDICATOR_GLIB_MODULE" ;;
ayatana)
        appindicator_modules="$AYATANA_APPINDICATOR_MODULE" ;;
appindicator)
        appindicator_modules="$APPINDICATOR_MODULE" ;;
no)
        appindicator_modules="" ;;
*)
        AC_MSG_ERROR([unknown value for --with-appindicator: $with_appindicator]) ;;
esac
if test "x$with_appindicator" = xno ; then
        if test x$enable_appindicator = xyes ; then
                AC_MSG_ERROR([--enable-appindicator conflicts with --without-appindicator])
        fi
        enable_appindicator="no"
elif test "x$with_appindicator" != xauto ; then
        if test x$enable_appindicator = xno ; then
                AC_MSG_ERROR([--with-appindicator=$with_appindicator conflicts with --disable-appindicator])
        fi
        enable_appindicator="yes"
fi
if test x$enable_appindicator != xno ; then
        AC_MSG_CHECKING([for AppIndicator library])
        found_appindicator_module=""
        for module in $appindicator_modules ; do
                PKG_CHECK_EXISTS([$module >= $APPINDICATOR_REQUIRED],
                                 [found_appindicator_module="$module"
                                  break])
        done
        if test -n "$found_appindicator_module" ; then
                AC_MSG_RESULT([$found_appindicator_module])
                enable_appindicator="yes"
                APPINDICATOR_MODULE="$found_appindicator_module"
        elif test x$enable_appindicator = xyes ; then
                AC_MSG_RESULT([no])
                AC_MSG_ERROR([no AppIndicator library is installed (tried: $appindicator_modules)])
        else
                AC_MSG_RESULT([no])
                enable_appindicator="no"
        fi
fi
if test x$enable_appindicator = xyes ; then
        PKG_CHECK_MODULES(APP_INDICATOR,
                          $APPINDICATOR_MODULE >= $APPINDICATOR_REQUIRED)
        if test "x$APPINDICATOR_MODULE" = "x$AYATANA_APPINDICATOR_GLIB_MODULE" ; then
                AC_DEFINE(HAVE_AYATANA_APP_INDICATOR_GLIB, [1], [Use Ayatana AppIndicator for GLib])
        elif test "x$APPINDICATOR_MODULE" = "x$AYATANA_APPINDICATOR_MODULE" ; then
                AC_DEFINE(HAVE_AYATANA_APP_INDICATOR, [1], [Use Ayatana AppIndicator])
        fi
        AC_DEFINE(HAVE_APP_INDICATOR, [1], [Have AppIndicator])
fi
AM_CONDITIONAL(HAVE_APP_INDICATOR, test x"$enable_appindicator" = xyes)

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h memory.h netdb.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([inet_ntoa memset socket strcasecmp strchr strncasecmp strndup strpbrk strstr strtol])

AC_CONFIG_FILES([Makefile
                 plugins/Makefile
                 data/Makefile
                 display/balloon/Makefile
                 display/fog/Makefile
                 display/nico2/Makefile
                 subscribe/tweets/Makefile
                 subscribe/rhythmbox/Makefile
                 data/gol.desktop])
AC_OUTPUT
