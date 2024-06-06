bin_PROGRAMS = main.out
main_out_SOURCES = main.c
main_out_LDFLAGS = -L$(abs_top_builddir)/lib/usb -lusb -L$(abs_top_builddir)/src/.libs -lptp -Wl,-rpath,$(abs_top_builddir)/lib/usb -Wl,-rpath,$(abs_top_builddir)/src/.libs
