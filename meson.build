project(
    'raylib-game',
    'cpp',
    version: '0.1.0',
    default_options: [
        'cpp_std=c++23',
        'warning_level=3',
        'buildtype=debug',
    ],
)

cmake = import('cmake')
opts = cmake.subproject_options()
opts.add_cmake_defines({
    'BUILD_EXAMPLES': 'OFF',
    'BUILD_SHARED_LIBS': 'OFF',
})
raylib = cmake.subproject('raylib', options: opts)
raylib_dep = raylib.dependency('raylib')

executable(
    'raylib-game',
    'src/main.cpp',
    dependencies: raylib_dep,
    install: false,
)
