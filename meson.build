project(
  'rdf-store',
  'cpp',
  version : '0.0.1',
  license : 'GPLV2',
  default_options:[
    'warning_level=3',
    'werror=True'
  ])

add_global_arguments('--std=c++20', language : 'cpp')

#Dependencies
catch = dependency('catch2-with-main')
soup = dependency('thesoup')
raptor2 = dependency('raptor2')

# Sources
inc = include_directories('headers')

subdir('src')
subdir('tst')

# Set up pkgconfig
pkg = import('pkgconfig')
