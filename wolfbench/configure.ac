AC_INIT([wolfbench], [0.1], [jtuck@ncsu.edu])

AC_PREFIX_DEFAULT(`pwd`/install)

AC_PATH_PROG(GCC,gcc,gcc,[$PATH$PATH_SEPARATOR/usr/ece566/install/bin/$PATH_SEPARATOR/usr/local/bin])
AC_PATH_PROG(CLANG,clang,,[$PATH$PATH_SEPARATOR/usr/ece566/install/bin/$PATH_SEPARATOR/usr/local/bin])
AC_PATH_PROG(CLANGXX,clang++,clang++,[$PATH$PATH_SEPARATOR/usr/ece566/install/bin/$PATH_SEPARATOR/usr/local/bin])
AC_PATH_PROG(OPT,opt,opt,[$PATH$PATH_SEPARATOR/usr/ece566/install/bin/$PATH_SEPARATOR/usr/local/bin])
AC_PATH_PROG(LLVM_LINK,llvm-link,llvm-link,[$PATH$PATH_SEPARATOR/usr/ece566/install/bin/$PATH_SEPARATOR/usr/local/bin])
AC_PATH_PROG(LLVM_DIS,llvm-dis,llvm-dis,[$PATH$PATH_SEPARATOR/usr/ece566/install/bin/$PATH_SEPARATOR/usr/local/bin])
AC_PATH_PROG(LLVM_CONFIG,llvm-config,llvm-config,[$PATH$PATH_SEPARATOR/usr/ece566/install/bin/$PATH_SEPARATOR/usr/local/bin])
AC_PATH_PROG(LLC,llc,llc,[$PATH$PATH_SEPARATOR/usr/ece566/install/bin/$PATH_SEPARATOR/usr/local/bin])
AC_PATH_PROG(LLVMAS,llvm-as,llvm-as,[$PATH$PATH_SEPARATOR/usr/ece566/install/bin/$PATH_SEPARATOR/usr/local/bin])

AC_SUBST(CUSTOMTOOL,cp)
AC_SUBST(PROFILER,)
AC_SUBST(CUSTOMCODEGEN,)
AC_SUBST(FAULTINJECTTOOL,)
AC_SUBST(P1TOOL,true)

AC_SUBST(DIRS,"SimpleTests Benchmarks")

AC_SUBST(DEBUG)
AC_ARG_ENABLE(debug, 
AS_HELP_STRING([--enable-debug],
               [Enables debug build (default is no)]),
[if test "$enableval" = "yes"; then AC_SUBST([DEBUG],[DEBUG=1]) fi],
)

AC_SUBST(LLVM)
AC_ARG_ENABLE(llvm, 
AS_HELP_STRING([--enable-llvm=path],
              [Use llvm, optionally at path (default is to search)]),
[if test "$enableval"; then AC_SUBST([LLVM],["$enable_llvm"]) fi],
)

AC_SUBST(DRAGONEGG)
AC_ARG_ENABLE(dragonegg, 
AS_HELP_STRING([--enable-dragonegg=path],
              [Use dragonegg with gcc, path is required]),
[if test "$enableval"; then AC_SUBST([DRAGONEGG],["$enable_dragonegg"]) fi],
)

AC_SUBST(GCC)
AC_ARG_ENABLE(gcc, 
AS_HELP_STRING([--enable-gcc=path],
              [Use gcc, optionally at path (default is to search)]),
[if test "$enableval"; then AC_SUBST([GCC],["$enable_gcc"]) fi],
)



AC_ARG_ENABLE(customtool, 
AS_HELP_STRING([--enable-customtool=path/tool],
              [Use custom tool (default is to none)]),
[if test "$enableval"; then AC_SUBST([CUSTOMTOOL],["$enable_customtool"]) fi],
)

AC_ARG_ENABLE(custom-codegen, 
AS_HELP_STRING([--enable-custom-codegen=path/tool],
              [Use custom tool for code generation (default is to none)]),
[if test "$enableval"; then AC_SUBST([CUSTOMCODEGEN],["$enable_custom_codegen"]) fi],
)

AC_ARG_ENABLE(profiler, 
AS_HELP_STRING([--enable-profiler=path/tool],
              [Use the specified profiler (default is to none)]),
[if test "$enableval"; then AC_SUBST([PROFILER],["$enable_profiler"]) fi],
)

AC_ARG_ENABLE(faultinjecttool, 
AS_HELP_STRING([--enable-faultinjecttool=path/tool],
              [Use custom tool for code generation (default is to none)]),
[if test "$enableval"; then AC_SUBST([FAULTINJECTTOOL],["$enable_faultinjecttool"]) fi],
)

AC_ARG_ENABLE(p1, 
AS_HELP_STRING([--enable-p1=path/tool],
              [Use p1 tool (default is to none)]),
[if test "$enableval"; then AC_SUBST([P1TOOL],["$enable_p1"]) AC_SUBST([DIRS],["P1Tests"]) fi],
)

AC_ARG_ENABLE(p2, 
AS_HELP_STRING([--enable-p2=path/tool],
              [Use p1 tool (default is to none)]),
[if test "$enableval"; then AC_SUBST([P2TOOL],["$enable_p2"]) AC_SUBST([DIRS],["P2Tests"]) fi],
)

AC_SUBST(EXTRA_SUFFIX)
AC_ARG_ENABLE(extrasuffix, 
AS_HELP_STRING([--enable-extrasuffix=suffix],
              [Add suffix to all binary names]),
[if test "$enableval"; then AC_SUBST([EXTRA_SUFFIX],["$enable_extrasuffix"]) fi],
)

AC_SUBST(p2cases,["\$(ece466) \$(ece566)"])
AC_ARG_ENABLE(ece466,
AS_HELP_STRING([--enable-ece466],
              [Test only 466 cases]),
[if test "$enableval"; then AC_SUBST([p2cases],["\$(ece466)"]) fi],
)

AC_CONFIG_FILES([
	Makefile.defs
	Makefile.config
	Makefile
	SimpleTests/Makefile	
	Benchmarks/Makefile	
	Benchmarks/adpcm/Makefile	
	Benchmarks/arm/Makefile	
	Benchmarks/basicmath/Makefile	
	Benchmarks/bh/Makefile	
	Benchmarks/bisort/Makefile	
	Benchmarks/bitcount/Makefile	
	Benchmarks/bwmem/Makefile	
	Benchmarks/CRC32/Makefile	
	Benchmarks/dijkstra/Makefile	
	Benchmarks/em3d/Makefile	
	Benchmarks/FFT/Makefile	
	Benchmarks/FIR/Makefile	
	Benchmarks/hanoi/Makefile	
	Benchmarks/kmp/Makefile	
	Benchmarks/l2lat/Makefile	
	Benchmarks/mst/Makefile	
	Benchmarks/patricia/Makefile	
	Benchmarks/qsort/Makefile	
	Benchmarks/sha/Makefile	
	Benchmarks/smatrix/Makefile	
	Benchmarks/susan/Makefile	
	Benchmarks/sqlite/Makefile	
	P1Tests/Makefile	
	P2Tests/Makefile	
	])


AC_OUTPUT 
