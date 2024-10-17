project('rpicam-astreampps', 'c', 'cpp',
        meson_version : '>= 0.64.0',
        version : '1.0.0',
        default_options : [
            'werror=true',
            'warning_level=3',
            'cpp_std=c++17',
            'c_std=c11',
            'buildtype=release',
        ])

cpp_arguments = ['-pedantic', '-Wno-unused-parameter', '-faligned-new']

# Needed for file sizes > 32-bits.
cpp_arguments += '-D_FILE_OFFSET_BITS=64'

cxx = meson.get_compiler('cpp')
cpu = host_machine.cpu()
neon = get_option('neon_flags')

if cxx.get_id() == 'gcc'
    cpp_arguments += '-Wno-psabi'
endif

if cpu == 'aarch64' or neon == 'arm64'
    cpp_arguments += '-ftree-vectorize'
elif neon == 'armv8-neon'
    cpp_arguments += ['-mfpu=neon-fp-armv8', '-ftree-vectorize']
endif

dl_dep = dependency('dl', required : true)
libcamera_dep = dependency('libcamera', required : true)
rpicam_java_dep = dependency('libcamera_app', required: true)

summary({
            'location' : libcamera_dep.get_variable('libdir'),
            'version' : libcamera_dep.version()
        },
        section : 'libcamera')

subdir('lib')
subdir ('output')

add_project_arguments(cpp_arguments, language : 'cpp')

rpicam_stream = library(
    'rpicam_java',
    rpicam_java_src,
    soversion : meson.project_version(),
    include_directories : include_directories('.'),
    install : true,
    link_with: libcamera_app,
    name_prefix : '',
    dependencies : rpicam_java_dep,
)
