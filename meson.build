project('nytl', 'cpp',
	license: 'BSL-1.0',
	version: '0.6.0',
	meson_version: '>=0.43',
	default_options: [
	  'cpp_std=c++17',
	  'warning_level=3'])

warnings = [
	# extra
	'-Wduplicated-cond',
	'-Wrestrict',
	'-Wnull-dereference',
	'-Wundef',
	'-Wcast-align=strict',
	'-Wlogical-op',

	# clang
	'-Wno-missing-braces'
]

# default arguments
cc = meson.get_compiler('cpp')
add_project_arguments(
	cc.get_supported_arguments(warnings),
	language: 'cpp')

headers = [
	'nytl/approx.hpp',
	'nytl/approxVec.hpp',
	'nytl/callback.hpp',
	'nytl/clone.hpp',
	'nytl/connection.hpp',
	'nytl/flags.hpp',
	'nytl/functionTraits.hpp',
	'nytl/fwd.hpp',
	'nytl/mat.hpp',
	'nytl/matOps.hpp',
	'nytl/math.hpp',
	'nytl/rect.hpp',
	'nytl/rectOps.hpp',
	'nytl/recursiveCallback.hpp',
	'nytl/scope.hpp',
	'nytl/simplex.hpp',
	'nytl/span.hpp',
	'nytl/tmpUtil.hpp',
	'nytl/utf.hpp',
	'nytl/vec.hpp',
	'nytl/vec2.hpp',
	'nytl/vec3.hpp',
	'nytl/vecOps.hpp'
]

fwd_headers = [
	'nytl/fwd/flags.hpp',
	'nytl/fwd/mat.hpp',
	'nytl/fwd/rect.hpp',
	'nytl/fwd/simplex.hpp',
	'nytl/fwd/span.hpp',
	'nytl/fwd/vec.hpp',
]

inc_dir = include_directories('.')
nytl_dep = declare_dependency(
	version: meson.project_version(),
	include_directories: inc_dir)

test = get_option('tests')
if test
	subdir('docs/tests')
endif

install_headers(headers, subdir: 'nytl')
install_headers(fwd_headers, subdir: 'nytl/fwd')

pkg = import('pkgconfig')
pkg_dirs = ['.', 'nytl']
pkg.generate(
	name: 'nytl',
	filebase: 'nytl',
	subdirs: pkg_dirs,
	version: meson.project_version(),
	description: 'C++17 utility headers')
