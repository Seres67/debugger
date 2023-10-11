project('debugger', 'c', default_options: ['warning_level=2'])
include_dir = include_directories('include')
src = ['src/main.c', 'src/debugger.c', 'src/utils.c', 'src/breakpoint.c']
replxx = dependency('replxx', required: true)
executable('debugger', src, include_directories: include_dir, dependencies: replxx)
