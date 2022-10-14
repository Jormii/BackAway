import os

PPSSPP_COMPILATION = True

MAKEFILE = """
TARGET = {psp_target}
OBJS = {objs}

LIBS = {libs}

INCDIR = {include_dirs}
CFLAGS = {c_flags}
CXXFLAGS = $(CFLAGS) -fno-rtti -fexceptions
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =

BUILD_PRX = 1
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = {eboot_title}

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
"""


class Makefile:

    def __init__(self):
        self.psp_target = "PSP_TARGET"
        self.eboot_title = "EBOOT"
        self.c_flags = []

        self.objs = IWhereList()
        self.libs = IWhereList()
        self.include_dirs = IWhereList()

    def create(self, output_dir):
        path = os.path.join(output_dir, "Makefile")
        with open(path, "w") as fd:
            fd.write(MAKEFILE.format(
                psp_target=self.psp_target,
                eboot_title=self.eboot_title,
                c_flags=" ".join(self.c_flags),

                objs=self.objs.where(),
                libs=self.libs.where(),
                include_dirs=self.include_dirs.where()
            ))


class IWhere:

    def where(self):
        raise NotImplementedError()

    def __repr__(self):
        return self.where()


class SourceDir(IWhere):

    def __init__(self, path):
        self.path = path
        self.files = IWhereList()

        for filename in os.listdir(self.path):
            file_path = os.path.join(self.path, filename)
            if not os.path.isfile(file_path):
                continue

            head, tail = os.path.splitext(filename)
            if tail != ".c":
                continue

            obj_filename = f"{head}.o"
            self.files.append(File(obj_filename))

    def where(self):
        where_stringify = []
        for f in self.files.elements:
            where_stringify.append(os.path.join(self.path, f.where()))

        return where_stringify


class IncludeDir(IWhere):

    def __init__(self, path):
        self.path = path

    def where(self):
        return self.path


class File(IWhere):

    def __init__(self, path):
        self.path = path

    def where(self):
        return self.path


class IWhereList(IWhere):

    def __init__(self):
        self.elements = []

    def append(self, iwhere):
        self.elements.append(iwhere)

    def where(self):
        where_stringify = []
        for e in self.elements:
            e_stringify = e.where()
            if isinstance(e_stringify, list):
                where_stringify.extend(e_stringify)
            else:
                where_stringify.append(e.where())

        return " ".join(where_stringify)


def main():
    makefile = Makefile()

    # Titles and flags
    makefile.eboot_title = "BackAway"
    makefile.c_flags = ["-g", "-G0", "-O2", "-Wall", "-Wno-unknown-pragmas"]

    if PPSSPP_COMPILATION:
        makefile.c_flags.extend(["-D", "PPSSPP"])

    # Objs
    source = [
        SourceDir("../src/utils/"),
        SourceDir("../src/geometry/"),
        SourceDir("../src/graphics/"),
        SourceDir("../src/math/"),
        SourceDir("../src/level/"),
        SourceDir("../src/states/"),
        SourceDir("../src/"),
        File("../main.o")
    ]
    for _src in source:
        makefile.objs.append(_src)

    # Libs
    libs = [
        File("-lm"),
        File("-losl"),
        ## REQUIRED BY OSLIB ##
        File("-lpspgu"),
        File("-lpspaudio"),
        File("-lpspaudiocodec"),
        File("-lpsphprm"),
        File("-lpsppower")
        # -- END --
    ]
    for _lib in libs:
        makefile.libs.append(_lib)

    # Includes
    includes = [
        IncludeDir("../include/utils/"),
        IncludeDir("../include/geometry/"),
        IncludeDir("../include/graphics/"),
        IncludeDir("../include/math/"),
        IncludeDir("../include/level/"),
        IncludeDir("../include/states/"),
        IncludeDir("../include/")
    ]
    for _inc in includes:
        makefile.include_dirs.append(_inc)

    makefile.create("./")


if __name__ == "__main__":
    main()
