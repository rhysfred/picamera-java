project('picamera-java', 'c', 'cpp',
        meson_version : '>= 0.64.0',
        version : '1.0.0',
        default_options : [
            'werror=true',
            'warning_level=3',
            'cpp_std=c++17',
            'c_std=c11',
            'buildtype=release',
        ])

rpicam_java_src = []

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

libcamera_dep = dependency('libcamera', required : true)
libcamera_app_dep = cxx.find_library('camera_app', required : true)
boost_dep = dependency('boost', modules : ['program_options'], required : true)

libcamera_app_lib_dir = '/usr/local/lib/aarch64-linux-gnu'
libcamera_app_header_dir = '/usr/local/include/rpicam-apps' 
libcamera_app_lib = 'libcamera_app'

summary({
            'libcamera location' : libcamera_dep.get_variable('libdir'),
            'libcamera version' : libcamera_dep.version(),
            'libcamera_app location' : libcamera_app_lib_dir,
        },
        section : 'libraries')

subdir('lib')
subdir ('output')

add_project_arguments(cpp_arguments, language : 'cpp')

rpicam_java = library(
    'rpicam_java',
    rpicam_java_src,
    build_rpath : libcamera_app_lib_dir,
    install_rpath : libcamera_app_lib_dir,
    soversion : meson.project_version(),
    include_directories : [ include_directories('.'), include_directories(libcamera_app_header_dir) ],
    install : true,
    name_prefix : '',
    dependencies : [libcamera_app_dep, libcamera_dep, boost_dep],
)
