project('samples', 'cpp',
        default_options : [
          'warning_level=3',
          'cpp_std=c++17'])

CPP_ARGS=['-Wall'] # '-Werror'

progs = [['enum_template', 'enum_template.cpp'],
         ['conversions', 'conversions.cpp'],
         ['template_specialization', 'template_specialization.cpp'],
         ['type', 'type.cpp'],
         ['sfinae', 'sfinae.cpp'],
         ['tag_dispatch', 'tag_dispatch.cpp'],
         ['reverse', 'reverse.cpp']]

foreach p : progs
  e = executable(p[0], p[1], cpp_args: CPP_ARGS)
  test(p[0], e)
endforeach
