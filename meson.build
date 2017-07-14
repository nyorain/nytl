project('nytl', 'cpp',
    license: 'Boost',
		version: '0.4.0',
    default_options: ['cpp_std=c++1z'])

version = '0.4.0'

add_global_arguments(
	'-Wall',
	'-Wextra',
	'-Wpedantic',
	'-Wno-missing-braces',
	language: 'cpp')

headers = [
	'nytl/approx.hpp',
	'nytl/approxVec.hpp',
	'nytl/assure.hpp',
	'nytl/callback.hpp',
	'nytl/clone.hpp',
	'nytl/connection.hpp',
	'nytl/convert.hpp',
	'nytl/dynVec.hpp',
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

install_headers(headers + fwd_headers, subdir: 'nytl')

inc_dir = include_directories('.')
nytl_dep = declare_dependency(
	version: meson.project_version(),
	include_directories: inc_dir)

test = get_option('test')
if test
	subdir('doc/tests')
endif