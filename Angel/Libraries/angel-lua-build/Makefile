CC = gcc
CFLAGS = -O2 -Wall -DLUA_USE_LINUX -DMISSING_POSIX_SPAWN -Wl,-E
LDFLAGS = -ldl -lreadline -lhistory -lncurses -lm
AR = ar rcu
RANLIB = ranlib
LIBNAME = liblua.a
EXENAME = lua
LUA_DIR    := ../lua-5.2.1/src
LUA_FS_DIR := ../lua-libs/luafilesystem-1.5.0/src

INCLUDE = 				\
	-I$(LUA_DIR)			\
	-I$(LUA_FS_DIR)			\

EXESRCS = 				\
	$(LUA_DIR)/lua.c

LIBSRCS =				\
	$(LUA_DIR)/lapi.c		\
	$(LUA_DIR)/lauxlib.c		\
	$(LUA_DIR)/lbaselib.c		\
	$(LUA_DIR)/lbitlib.c		\
	$(LUA_DIR)/lcode.c		\
	$(LUA_DIR)/lcorolib.c		\
	$(LUA_DIR)/lctype.c		\
	$(LUA_DIR)/ldblib.c		\
	$(LUA_DIR)/ldebug.c		\
	$(LUA_DIR)/ldo.c		\
	$(LUA_DIR)/ldump.c		\
	$(LUA_DIR)/lfunc.c		\
	$(LUA_DIR)/lgc.c		\
	$(LUA_DIR)/linit.c		\
	$(LUA_DIR)/liolib.c		\
	$(LUA_DIR)/llex.c		\
	$(LUA_DIR)/lmathlib.c		\
	$(LUA_DIR)/lmem.c		\
	$(LUA_DIR)/loadlib.c		\
	$(LUA_DIR)/lobject.c		\
	$(LUA_DIR)/lopcodes.c		\
	$(LUA_DIR)/loslib.c		\
	$(LUA_DIR)/lparser.c		\
	$(LUA_DIR)/lstate.c		\
	$(LUA_DIR)/lstring.c		\
	$(LUA_DIR)/lstrlib.c		\
	$(LUA_DIR)/ltable.c		\
	$(LUA_DIR)/ltablib.c		\
	$(LUA_DIR)/ltm.c		\
	$(LUA_DIR)/lundump.c		\
	$(LUA_DIR)/lvm.c		\
	$(LUA_DIR)/lzio.c		\
	$(LUA_FS_DIR)/lfs.c		

all: $(LIBNAME) $(EXENAME)

LIBOBJS = $(patsubst %.c, %.o, $(LIBSRCS))
EXEOBJS = $(patsubst %.c, %.o, $(EXESRCS))

%.o: %.c
	$(CC) -c $(INCLUDE) $(CFLAGS) -o $@ $^

$(LIBNAME): $(LIBOBJS)
	$(AR) $@ $?
	$(RANLIB) $@

$(EXENAME): $(LIBNAME) $(EXEOBJS)
	$(CC) -o $@ $(EXEOBJS) $(LIBNAME) $(LDFLAGS)

clean:
	rm -f $(LIBOBJS) $(EXEOBJS) $(LIBNAME) $(EXENAME)


