project('nytl', 'cpp',
	license: 'BSL-1.0',
	version: '0.4.0',
	default_options: ['cpp_std=c++1z', 'warning_level=3'])

add_project_arguments('-Wno-missing-braces', language: 'cpp')

headers = [
	'nytl/approx.hpp',
	'nytl/approxVec.hpp',
	'nytl/callback.hpp',
	'nytl/clone.hpp',
	'nytl/connection.hpp',
	'nytl/convert.hpp',
	'nytl/flags.hpp',
	'nytl/functionTraits.hpp',
	'nytl/fwd.hpp',
	'nytl/mat.hpp',
	'nytl/matOps.hpp',
	'nytl/nonCopyable.hpp',
	'nytl/observe.hpp',
	'nytl/rect.hpp',
	'nytl/rectOps.hpp',
	'nytl/referenced.hpp',
	'nytl/scalar.hpp',
	'nytl/scope.hpp',
	'nytl/simplex.hpp',
	'nytl/span.hpp',
	'nytl/tmpUtil.hpp',
	'nytl/tuple.hpp',
	'nytl/typemap.hpp',
	'nytl/utf.hpp',
	'nytl/vec.hpp',
	'nytl/vecOps.hpp']

fwd_headers = [
	'nytl/fwd/flags.hpp',
	'nytl/fwd/mat.hpp',
	'nytl/fwd/rect.hpp',
	'nytl/fwd/referenced.hpp',
	'nytl/fwd/simplex.hpp',
	'nytl/fwd/span.hpp',
	'nytl/fwd/typemap.hpp',
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

install_headers(headers + fwd_headers, subdir: 'nytl')

pkg = import('pkgconfig')
pkg_dirs = ['.', 'nytl']
pkg.generate(
	name: 'nytl',
	filebase: 'nytl',
	subdirs: pkg_dirs,
	version: meson.project_version(),
	description: 'C++17 utility headers')
